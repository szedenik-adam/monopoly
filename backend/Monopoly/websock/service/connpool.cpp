#include "connpool.h"


static struct event_base *base = NULL;
static evconnlistener *listener = NULL;
vector<WSAuth*> sessions;


static const uint32_t WS_REQ_ONCE_READ = 1;
//static const uint32_t MAX_WS_REQ_LEN = 10240;
//static const uint64_t MAX_MSG_LEN = 1024000;


//#define LOG(format, args...) fprintf(stdout, format"\n", ##args)
#define LOG printf


void user_disconnect(WSAuth* session) {
	if (session) {
		//update user list
		for (vector<WSAuth*>::iterator iter = sessions.begin(); iter != sessions.end(); ++iter) {
			if (*iter == session) {
				sessions.erase(iter);
				break;
			}
		}
		destroy_session(session);
	}
	LOG("now, %d users connecting", sessions.size());
}


void user_disconnect_cb(void *arg) {
	LOG("%s", __FUNCTION__); // __func__ was before
	WSAuth* session = (WSAuth*)arg;
	user_disconnect(session);
}


void listencb(struct evconnlistener *listener, evutil_socket_t clisockfd, struct sockaddr *addr, int len, void *ptr) {
	struct event_base *eb = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(eb, clisockfd, BEV_OPT_CLOSE_ON_FREE);
	LOG("a user logined in, socketfd = %d", bufferevent_getfd(bev));

	//create a user
	WSAuth* session = new WSAuth();
	session->wscon->bev = bev;
	sessions.push_back(session);
	//ws_conn_setcb(wscon, HANDSHAKE, testfunc, (void*)"haha");
	ws_conn_setcb(session->wscon, FRAME_RECV, frame_recv_cb, session);
	ws_conn_setcb(session->wscon, CLOSE, user_disconnect_cb, session);

	ws_serve_start(session->wscon);
}


evconnlistener* InitWebsocket(event_base* base, unsigned short port) {
	//SIGPIPE ignore
	/*struct sigaction act;
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &act, NULL) == 0) {
		LOG("SIGPIPE ignore");
	}*/
	assert(base);

	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = INADDR_ANY;
	srvaddr.sin_port = htons(port);

	listener = evconnlistener_new_bind(base, listencb, NULL, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 500, (const struct sockaddr*)&srvaddr, sizeof(struct sockaddr));
	assert(listener);

	char addrbuf[128];
	const char *addr = evutil_inet_ntop(srvaddr.sin_family, &(srvaddr.sin_addr), addrbuf, sizeof(addrbuf));
	if (addr) {
		printf("Websocket on %s:%d\n", addr, port);
	} else {
		fprintf(stderr, "WS: evutil_inet_ntop failed\n");
	}
	//evconnlistener_free(listener);
	return listener;
}
