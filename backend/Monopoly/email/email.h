
// Toggle email sending here.
#define EMAIL 0

#include <string>
#include <cstring>
#include <stdlib.h>

void SendEmail(const char* to, const char* subject, const char* message);

std::string CreateEmailContent(const std::string& token, const char* status);

void SetEmailServerHost(char* host);


#if EMAIL

#include <list>
#include <windows.h>

size_t EmailContentReader(char* buffer, size_t size, size_t nitems, void* userdata);

class EmailContent
{
public:
	char* message, *msgbase;
	size_t remaining_bytes;

	// Creates a deep copy of the message.
	EmailContent(const char* message): remaining_bytes(strlen(message))
	{
		this->message = (char*)malloc(remaining_bytes+1);
		memmove(this->message, message, remaining_bytes);
		this->message[remaining_bytes] = '\0';
		this->msgbase = this->message;
	}
	~EmailContent()
	{
		free(this->msgbase);
	}
};

class EmailRequest
{
	static std::list<EmailRequest*> requests;
	static bool email_thread_up;
	static HANDLE mutex; 
	static char* host;

	static DWORD EmailSender( LPVOID lpThreadParameter );
public:
	EmailContent content;
	char* to;

	EmailRequest(const char* to, const char* message): content(message)
	{
		size_t to_size = strlen(to);
		this->to = (char*)malloc(to_size+1);
		memmove(this->to, to, to_size);
		this->to[to_size]='\0';
	}
	~EmailRequest()
	{
		free(to);
	}

	static void Add(const char* to, const char* message);
	static void SetHost(char* host) {EmailRequest::host = host;}
};

#endif