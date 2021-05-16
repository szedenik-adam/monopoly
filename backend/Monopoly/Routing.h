#include <event2/http.h>

#define CROSS_ORIGIN 1

// Provides callbacks which can serve specific requests.
// Does translations between the HTTP server and the ContentGenerator.
class Routing
{
	static char readbuf[];
	static char* ReadInput(struct evhttp_request *req, size_t limit);
public:
	// Serves the /register webserver calls.
	static void Register(struct evhttp_request *req, void *arg);

	// Serves the /login calls.
	static void Login(struct evhttp_request *req, void *arg);
};