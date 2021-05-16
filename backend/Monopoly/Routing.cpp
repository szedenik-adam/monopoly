#include "Routing.h"
#include "ContentGenerator.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <stdio.h>
using namespace rapidjson;

// Only 1 read buffer (the server is single-threaded).
char Routing::readbuf[1025];

// Reads the request's content to a char array.
char* Routing::ReadInput(struct evhttp_request *req, size_t limit)
{
	evbuffer* buf = evhttp_request_get_input_buffer(req);
	size_t n = evbuffer_get_length(buf);
	if(n == 0) { return "";}
	if(n > limit) {evbuffer_drain(buf,n); return "";}

	n = evbuffer_remove(buf, readbuf, sizeof(readbuf));
	readbuf[n]='\0';
	return readbuf;
}

// Serves the /register webserver calls.
void Routing::Register(struct evhttp_request *req, void *arg)
{
	if(evhttp_request_get_command(req) != EVHTTP_REQ_POST) { evhttp_send_reply(req, 403, "Forbidden", NULL); return; }
	const char* uri = evhttp_request_get_uri(req);

	struct evkeyvalq* i_hdrs = evhttp_request_get_input_headers(req);
	for (struct evkeyval* h = i_hdrs->tqh_first; h; h = h->next.tqe_next) {
		printf("  %s: %s\n", h->key, h->value);
	}

	char* input = Routing::ReadInput(req, 1020);

	rapidjson::StringBuffer s;
	rapidjson::Writer<StringBuffer> writer(s);

	  int reply_code;
	char* reply_reason;
	ContentGenerator::Register(input, writer, &reply_code, &reply_reason);

	evbuffer* result = evbuffer_new();
	evbuffer_add(result, s.GetString(), s.GetSize());

	struct evkeyvalq* o_hdrs = evhttp_request_get_output_headers(req);
	evhttp_add_header(o_hdrs, "Content-Type", "application/json");
#if CROSS_ORIGIN
	evhttp_add_header(o_hdrs, "Access-Control-Allow-Origin", "*");
	evhttp_add_header(o_hdrs, "Access-Control-Allow-Methods", "GET, POST, DELETE, PUT");
#endif
	evhttp_send_reply(req, reply_code, reply_reason, result);
	
//	evhttp_send_reply(req, reply_code, reply_reason, NULL);
}

// Serves the /login calls.
void Routing::Login(struct evhttp_request *req, void *arg)
{
	if(evhttp_request_get_command(req) != EVHTTP_REQ_POST) { evhttp_send_reply(req, 403, "Forbidden", NULL); return; }

	char* input = Routing::ReadInput(req, 1020);

	rapidjson::StringBuffer s;
	rapidjson::Writer<StringBuffer> writer(s);
	
	  int reply_code;
	char* reply_reason;
	ContentGenerator::Login(input, writer, &reply_code, &reply_reason);

	evbuffer* result = evbuffer_new();
	evbuffer_add(result, s.GetString(), s.GetSize());

	struct evkeyvalq* o_hdrs = evhttp_request_get_output_headers(req);
	evhttp_add_header(o_hdrs, "Content-Type", "application/json");
#if CROSS_ORIGIN
	evhttp_add_header(o_hdrs, "Access-Control-Allow-Origin", "*");
	evhttp_add_header(o_hdrs, "Access-Control-Allow-Methods", "GET, POST, DELETE, PUT");
#endif
	evhttp_send_reply(req, reply_code, reply_reason, result);
}