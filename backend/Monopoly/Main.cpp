/*
  A trivial static http webserver using Libevent's evhttp.
  This is not the best code in the world, and it does some fairly stupid stuff
  that you would never want to do in a production webserver. Caveat hackor!
 */

/* Compatibility for possible missing IPv6 declarations */
//#include "../util-internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	//#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
	#pragma comment (lib, "event.lib")
	#pragma comment (lib, "event_core.lib")
//#pragma comment(lib, "wsock32.lib")

	// Memory leak finder.
	#define _CRTDBG_MAP_ALLOC
	#include <iostream>
	#include <crtdbg.h>
	#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
	#endif
#else // linux
	#include <sys/types.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>  // getnameinfo
	#include <unistd.h> // close
//#define ZeroMemory bzero
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include "add_file.h"
#include "websock/service/connpool.h"
#include "Routing.h"
#include "email/email.h"
#include "datastore/datastore.h"

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif

char uri_root[512];

static const struct table_entry {
	const char *extension;
	const char *content_type;
} content_type_table[] = {
	{ "html", "text/html" },
	{ "css", "text/css" },
	{ "txt", "text/plain" },
	{ "c", "text/plain" },
	{ "h", "text/plain" },
	{ "htm", "text/htm" },
	{ "gif", "image/gif" },
	{ "jpg", "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png", "image/png" },
	{ "pdf", "application/pdf" },
	{ "ps", "application/postsript" },
	{ NULL, NULL },
};

/* Try to guess a good content-type for 'path' */
static const char *
guess_content_type(const char *path)
{
	const char *last_period, *extension;
	const struct table_entry *ent;
	last_period = strrchr(path, '.');
	if (!last_period || strchr(last_period, '/'))
		goto not_found; /* no exension */
	extension = last_period + 1;
	for (ent = &content_type_table[0]; ent->extension; ++ent) {
		if (!evutil_ascii_strcasecmp(ent->extension, extension))
			return ent->content_type;
	}

not_found:
	return "application/misc";
}

/* Callback used for the /dump URI, and for every non-GET request:
 * dumps all information to stdout and gives back a trivial 200 ok */
static void
dump_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
	    cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
	    header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	buf = evhttp_request_get_input_buffer(req);
	puts("Input data: <<<");
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[128];
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>");

	evhttp_send_reply(req, 200, "OK", NULL);
}

/* This callback gets invoked when we get any http request that doesn't match
 * any other callback.  Like any evhttp server callback, it has a simple job:
 * it must eventually call evhttp_send_error() or evhttp_send_reply().
 */
static void
send_document_cb(struct evhttp_request *req, void *arg)
{
	struct evbuffer *evb = NULL;
	const char *docroot = (char*)arg;
	const char *uri = evhttp_request_get_uri(req);
	struct evhttp_uri *decoded = NULL;
	const char *path;
	char *decoded_path;
	char *whole_path = NULL;
	size_t len;
	int fd = -1;
	struct stat st;

	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
		dump_request_cb(req, arg);
		return;
	}

//	printf("Got a GET request for <%s>\n",  uri);

	/* Decode the URI */
	decoded = evhttp_uri_parse(uri);
	if (!decoded) {
		printf("It's not a good URI. Sending BADREQUEST\n");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	/* Let's see what path the user asked for. */
	path = evhttp_uri_get_path(decoded);
	if (!path) path = "/";
	if (strcmp(path,"/")==0) path="/index.html";

	/* We need to decode it, to see what path the user really wanted. */
	decoded_path = evhttp_uridecode(path, 0, NULL);
	if (decoded_path == NULL)
		goto err;
	/* Don't allow any ".."s in the path, to avoid exposing stuff outside
	 * of the docroot.  This test is both overzealous and underzealous:
	 * it forbids aceptable paths like "/this/one..here", but it doesn't
	 * do anything to prevent symlink following." */
	if (strstr(decoded_path, ".."))
		goto err;

	len = strlen(decoded_path)+strlen(docroot)+2;
	if (!(whole_path = (char*)malloc(len))) {
		perror("malloc");
		goto err;
	}
	evutil_snprintf(whole_path, len, "%s%s", docroot, decoded_path);

	if (stat(whole_path, &st)<0) {
		goto err;
	}

	/* This holds the content we're sending. */
	evb = evbuffer_new();

	if (S_ISDIR(st.st_mode)) {
		/* If it's a directory, read the comments and make a little
		 * index page */
#ifdef _WIN32
		HANDLE d;
		WIN32_FIND_DATAA ent;
		char *pattern;
		size_t dirlen;
#else
		DIR *d;
		struct dirent *ent;
#endif
		const char *trailing_slash = "";

		if (!strlen(path) || path[strlen(path)-1] != '/')
			trailing_slash = "/";

#ifdef _WIN32
		dirlen = strlen(whole_path);
		pattern = (char*)malloc(dirlen+3);
		memcpy(pattern, whole_path, dirlen);
		pattern[dirlen] = '\\';
		pattern[dirlen+1] = '*';
		pattern[dirlen+2] = '\0';
		d = FindFirstFileA(pattern, &ent);
		free(pattern);
		if (d == INVALID_HANDLE_VALUE)
			goto err;
#else
		if (!(d = opendir(whole_path)))
			goto err;
#endif

		evbuffer_add_printf(evb,
                    "<!DOCTYPE html>\n"
                    "<html>\n <head>\n"
                    "  <meta charset='utf-8'>\n"
		    "  <title>%s</title>\n"
		    "  <base href='%s%s'>\n"
		    " </head>\n"
		    " <body>\n"
		    "  <h1>%s</h1>\n"
		    "  <ul>\n",
		    decoded_path, /* XXX html-escape this. */
		    path, /* XXX html-escape this? */
		    trailing_slash,
		    decoded_path /* XXX html-escape this */);
#ifdef _WIN32
		do {
			const char *name = ent.cFileName;
#else
		while ((ent = readdir(d))) {
			const char *name = ent->d_name;
#endif
			evbuffer_add_printf(evb,
			    "    <li><a href=\"%s\">%s</a>\n",
			    name, name);/* XXX escape this */
#ifdef _WIN32
		} while (FindNextFileA(d, &ent));
#else
		}
#endif
		evbuffer_add_printf(evb, "</ul></body></html>\n");
#ifdef _WIN32
		FindClose(d);
#else
		closedir(d);
#endif
		evhttp_add_header(evhttp_request_get_output_headers(req),
		    "Content-Type", "text/html");
	} else {
		/* Otherwise it's a file; add it to the buffer to get
		 * sent via sendfile */
		const char *type = guess_content_type(decoded_path);
#ifdef _WIN32
		int binary_mode = ((strstr(type, "text")==0)?(O_BINARY):(0)); //printf("binary mode:%d\n",binary_mode);
#else
		int binary_mode = 0;
#endif
		if ((fd = open(whole_path, binary_mode | O_RDONLY)) < 0) {
			perror("open");
			goto err;
		}

		if (fstat(fd, &st)<0) {
			/* Make sure the length still matches, now that we
			 * opened the file :/ */
			perror("fstat");
			goto err;
		}
		evhttp_add_header(evhttp_request_get_output_headers(req),
		    "Content-Type", type);

// Cross Origin Policy
evhttp_add_header(evhttp_request_get_output_headers(req), "Access-Control-Allow-Origin", "*");
evhttp_add_header(evhttp_request_get_output_headers(req), "Access-Control-Allow-Methods", "GET, POST, DELETE, PUT");

		evbuffer_add_file2(evb, fd, 0, st.st_size);
	}

	evhttp_send_reply(req, 200, "OK", evb);
	goto done;
err:
	evhttp_send_error(req, 404, "Document was not found");
	if (fd>=0)
		close(fd);
done:
	if (decoded)
		evhttp_uri_free(decoded);
	if (decoded_path)
		// If this makes a runtime assertion failed on windows
		// then the libevent is linked dynamically and uses a different MSVC runtime than the program.
		// Link the correct libevent (debug or release)
		//   or rebuild the libevent with the correct runtime
		//   or just link libevent statically. 
		free(decoded_path); 
	if (whole_path)
		free(whole_path);
	if (evb)
		evbuffer_free(evb);
}

/*static void
syntax(void)
{
	fprintf(stdout, "Syntax: http-server <docroot>\n");
}*/
void TestGameMethods()
{
#ifdef _DEBUG
	User* u1 = DataStore::AddUser("AAA","AAA","AAA");
	User* u2 = DataStore::AddUser("BBB","BBB","BBB");
	Map* map = DataStore::CreateMap(u1,"test_map");
	map->AddUser(u2);
	map->Start();
	Player* p1 = map->GetPlayer(0);

	FEvent events = map->Roll(1,2);
	if(events & FEvent::Roll) printf("roll");
	if(events & FEvent::BuyField) printf("buy");
	events = map->BuyField();
	SendEmail("real_email_address_goes_here","Server started", "Monopoly Server Started!\r\n Ádám");
#endif

	User* uu1 = DataStore::AddUser("a","","a");
	User* uu2 = DataStore::AddUser("b","","b");
	User* uu3 = DataStore::AddUser("c","","c");
	User* uu4 = DataStore::AddUser("d","","d");
}

int main(int argc, char **argv)
{
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

	unsigned short port = 80;
#ifdef _WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif

	struct stat st;
	/*if (stat("./Main.cpp", &st)<0) {
		printf(".....");
	}*/

	TestGameMethods();

#ifdef _DEBUG
	char* docroot = "../../frontend/dist";
#else
	char* docroot = ".";
#endif
	if (argc >= 2) {
		// Param 1 is the docroot.
		docroot = argv[1];
	}
	if (argc >= 3) {
		// Param 2 is the server address (needed for the email continue url).
		SetEmailServerHost(argv[2]);
	}
	if(argc < 2) {
		printf("Usage: monopoly [docroot] [host (for email only)]\n");
	}

	base = event_base_new();

	/*struct timeval ten_sec = {2,0};
	event_base_loopexit(base, &ten_sec);
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );*/

	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}

	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "Couldn't create evhttp. Exiting.\n");
		return 1;
	}

	/* The /dump URI will dump all requests to stdout and say 200 ok. */
	evhttp_set_cb(http, "/dump", dump_request_cb, NULL);

	evhttp_set_cb(http, "/register", Routing::Register, NULL);
	evhttp_set_cb(http, "/login",    Routing::Login, NULL);

	/* We want to accept arbitrary requests, so we need to set a "generic"
	 * cb.  We can also add callbacks for specific paths. */
	evhttp_set_gencb(http, send_document_cb, docroot);

	/* Now we tell the evhttp what port to listen on */
	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
	if (!handle) {
		fprintf(stderr, "Couldn't bind to port %d. Exiting.\n",
		    (int)port);
		return 1;
	}

	{
		/* Extract and display the address we're listening on. */
		struct sockaddr_storage ss;
		evutil_socket_t fd;
		ev_socklen_t socklen = sizeof(ss);
		char addrbuf[128];
		void *inaddr;
		const char *addr;
		int got_port = -1;
		fd = evhttp_bound_socket_get_fd(handle);
		memset(&ss, 0, sizeof(ss));
		if (getsockname(fd, (struct sockaddr *)&ss, &socklen)) {
			perror("getsockname() failed");
			return 1;
		}
		if (ss.ss_family == AF_INET) {
			got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
			inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
		} else if (ss.ss_family == AF_INET6) {
			got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
			inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
		} else {
			fprintf(stderr, "Weird address family %d\n",
			    ss.ss_family);
			return 1;
		}
		addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf,
		    sizeof(addrbuf));
		if (addr) {
			printf("Listening on %s:%d\n", addr, got_port);
			evutil_snprintf(uri_root, sizeof(uri_root),
			    "http://%s:%d",addr,got_port);
		} else {
			fprintf(stderr, "evutil_inet_ntop failed\n");
			return 1;
		}
	}

	evconnlistener* ws = InitWebsocket(base, 81);

	event_base_dispatch(base);

	evconnlistener_free(ws);
	evhttp_free(http);
	event_base_free(base);

	return 0;
}