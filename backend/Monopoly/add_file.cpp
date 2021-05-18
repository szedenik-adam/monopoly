#include "add_file.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>

#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef _EVENT_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef _EVENT_HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef _EVENT_HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef _EVENT_HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifdef _EVENT_HAVE_SYS_SENDFILE_H
#include <sys/sendfile.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _EVENT_HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef _EVENT_HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <limits.h>

/* some systems do not have MAP_FAILED */
#ifndef MAP_FAILED
#define MAP_FAILED	((void *)-1)
#endif


/* TODO(niels): maybe we don't want to own the fd, however, in that
 * case, we should dup it - dup is cheap.  Perhaps, we should use a
 * callback instead?
 */
/* TODO(niels): we may want to add to automagically convert to mmap, in
 * case evbuffer_remove() or evbuffer_pullup() are being used.
 */
int
evbuffer_add_file2(struct evbuffer *outbuf, int fd,
    ev_off_t offset, ev_off_t length)
{
#if defined(USE_SENDFILE) || defined(_EVENT_HAVE_MMAP)
	struct evbuffer_chain *chain;
	struct evbuffer_chain_fd *info;
#endif
#if defined(USE_SENDFILE)
	int sendfile_okay = 1;
#endif
	int ok = 1;

	/*if (offset < 0 || length < 0 ||
	    ((ev_uint64_t)length > EVBUFFER_CHAIN_MAX) ||
	    (ev_uint64_t)offset > (ev_uint64_t)(EVBUFFER_CHAIN_MAX - length))
		return (-1);*/

#if defined(USE_SENDFILE)
	{//if (use_sendfile) {
//		EVBUFFER_LOCK(outbuf);
		int sendfile_okay = outbuf->flags & EVBUFFER_FLAG_DRAINS_TO_FD;
//		EVBUFFER_UNLOCK(outbuf);
	}

	if (/*use_sendfile &&*/ sendfile_okay) {
		chain = evbuffer_chain_new(sizeof(struct evbuffer_chain_fd));
		if (chain == NULL) {
//			event_warn("%s: out of memory", __func__);
			return (-1);
		}

		chain->flags |= EVBUFFER_SENDFILE | EVBUFFER_IMMUTABLE;
		chain->buffer = NULL;	/* no reading possible */
		chain->buffer_len = length + offset;
		chain->off = length;
		chain->misalign = offset;

		info = EVBUFFER_CHAIN_EXTRA(struct evbuffer_chain_fd, chain);
		info->fd = fd;

//		EVBUFFER_LOCK(outbuf);
		if (outbuf->freeze_end) {
			mm_free(chain);
			ok = 0;
		} else {
			outbuf->n_add_for_cb += length;
			evbuffer_chain_insert(outbuf, chain);
		}
	} else
#endif
/*#if defined(_EVENT_HAVE_MMAP)
	if (use_mmap) {
		void *mapped = mmap(NULL, length + offset, PROT_READ,
#ifdef MAP_NOCACHE
		    MAP_NOCACHE |
#endif
#ifdef MAP_FILE
		    MAP_FILE |
#endif
		    MAP_PRIVATE,
		    fd, 0);
		// some mmap implementations require offset to be a multiple of
		// the page size.  most users of this api, are likely to use 0
		// so mapping everything is not likely to be a problem.
		// TODO(niels): determine page size and round offset to that
		// page size to avoid mapping too much memory.
		//
		if (mapped == MAP_FAILED) {
			event_warn("%s: mmap(%d, %d, %zu) failed",
			    __func__, fd, 0, (size_t)(offset + length));
			return (-1);
		}
		chain = evbuffer_chain_new(sizeof(struct evbuffer_chain_fd));
		if (chain == NULL) {
			event_warn("%s: out of memory", __func__);
			munmap(mapped, length);
			return (-1);
		}

		chain->flags |= EVBUFFER_MMAP | EVBUFFER_IMMUTABLE;
		chain->buffer = mapped;
		chain->buffer_len = length + offset;
		chain->off = length + offset;

		info = EVBUFFER_CHAIN_EXTRA(struct evbuffer_chain_fd, chain);
		info->fd = fd;

		EVBUFFER_LOCK(outbuf);
		if (outbuf->freeze_end) {
			info->fd = -1;
			evbuffer_chain_free(chain);
			ok = 0;
		} else {
			outbuf->n_add_for_cb += length;

			evbuffer_chain_insert(outbuf, chain);

			// we need to subtract whatever we don't need
			evbuffer_drain(outbuf, offset);
		}
	} else
#endif*/
	{
		/* the default implementation */
		struct evbuffer *tmp = evbuffer_new();
		ev_ssize_t read;

		if (tmp == NULL)
			return (-1);

#ifdef WIN32
#define lseek _lseeki64
#endif
		if (lseek(fd, offset, SEEK_SET) == -1) {
			evbuffer_free(tmp);
			return (-1);
		}

		/* we add everything to a temporary buffer, so that we
		 * can abort without side effects if the read fails.
		 */
		while (length) {
			ev_ssize_t to_read = length > EV_SSIZE_MAX ? EV_SSIZE_MAX : (ev_ssize_t)length;
			read = evbuffer_readfile(tmp, fd, to_read); 
			if (read == -1) {
				evbuffer_free(tmp);
				return (-1);
			}
			if(read == 0) break;

			length -= read;
		}

//EVLOCK_LOCK((outbuf)->lock, 0); //EVBUFFER_LOCK(outbuf);
/*		if (outbuf->freeze_end) {
			evbuffer_free(tmp);
			ok = 0;
		} else*/ {
			evbuffer_add_buffer(outbuf, tmp);
			evbuffer_free(tmp);

#ifdef WIN32
#define close _close
#endif
			close(fd);
		}
	}

/*	if (ok)
		evbuffer_invoke_callbacks(outbuf);*/
//EVLOCK_UNLOCK((outbuf)->lock, 0);	//EVBUFFER_UNLOCK(outbuf);

	return ok ? 0 : -1;
}

#ifdef WIN32
static int
evbuffer_readfile(struct evbuffer *buf, evutil_socket_t fd, ev_ssize_t howmuch)
{
	int result;
	int nchains, n;
	struct evbuffer_iovec v[2];

	/*if (buf->freeze_end) {
		result = -1;
		goto done;
	}*/

	if (howmuch < 0)
		howmuch = 16384;


	/* XXX we _will_ waste some space here if there is any space left
	 * over on buf->last. */
	nchains = evbuffer_reserve_space(buf, howmuch, v, 2);
	if (nchains < 1 || nchains > 2) {
		result = -1;
		goto done;
	}
	n = read((int)fd, v[0].iov_base, (unsigned int)v[0].iov_len);
	if (n <= 0) {
		result = n;
		goto done;
	}
	v[0].iov_len = (size_t) n; /* XXXX another problem with big n.*/
	if (nchains > 1) {
		n = read((int)fd, v[1].iov_base, (unsigned int)v[1].iov_len);
		if (n <= 0) {
			result = (unsigned long) v[0].iov_len;
			evbuffer_commit_space(buf, v, 1);
			goto done;
		}
		v[1].iov_len = n;
	}
	evbuffer_commit_space(buf, v, nchains);

	result = n;
done:
	return result;
}
#endif