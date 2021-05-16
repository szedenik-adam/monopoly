#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>
#include <event2/bufferevent_struct.h>
#include <event2/thread.h>
#include <event2/event-config.h>

int
evbuffer_add_file2(struct evbuffer *outbuf, int fd,
    ev_off_t offset, ev_off_t length);

#ifdef WIN32
static int evbuffer_readfile(struct evbuffer *buf, evutil_socket_t fd,
    ev_ssize_t howmuch);
#else
#define evbuffer_readfile evbuffer_read
#endif