
#include "email.h"

#if EMAIL

#define EMAIL_SMTP_URL "smtp://smtp.gmail.com:587"
#define EMAIL_USERNAME "...@gmail.com"
#define EMAIL_PASSWORD "..."

#include <curl/curl.h>
#include <sstream>
#include "../websock/base64.h"
using namespace std;

#ifdef WIN32
#pragma comment (lib, "LIBCURL.lib")
#endif

std::list<EmailRequest*> EmailRequest::requests;
bool EmailRequest::email_thread_up = false;
HANDLE EmailRequest::mutex = 0; 
char* EmailRequest::host = "monopoly";

void SetEmailServerHost(char* host){EmailRequest::SetHost(host);}

void EmailRequest::Add(const char* to, const char* message)
{
	if (!to || to[0] == '\0') { return; } // Don't send email to empty address.

	if(EmailRequest::mutex == 0) {
	   EmailRequest::mutex = CreateMutex( 
		  NULL,              // default security attributes
		  FALSE,             // initially not owned
		  NULL);             // unnamed mutex
	}

	DWORD dwWaitResult = WaitForSingleObject( EmailRequest::mutex, INFINITE);  // no time-out interval

	EmailRequest::requests.push_back(new EmailRequest(to, message));

	if(EmailRequest::email_thread_up == false) {
	   EmailRequest::email_thread_up = true;

	   DWORD ThreadID;

	   HANDLE th = CreateThread( 
           NULL,       // default security attributes
           0,          // default stack size
           (LPTHREAD_START_ROUTINE) EmailRequest::EmailSender, 
           NULL,       // no thread function arguments
           0,          // Immediately run the thread
           &ThreadID); // receive thread identifier
	}

	ReleaseMutex( EmailRequest::mutex );
}

DWORD EmailRequest::EmailSender( LPVOID lpThreadParameter )
{
	CURL* curl = 0;
	DWORD dwWaitResult = WaitForSingleObject( EmailRequest::mutex, INFINITE);  // no time-out interval
	while( !EmailRequest::requests.empty() )
	{
		EmailRequest* req = EmailRequest::requests.front();
		EmailRequest::requests.pop_front();

		ReleaseMutex( EmailRequest::mutex );
		if(curl==0) {
			curl = curl_easy_init();

			curl_easy_setopt(curl, CURLOPT_URL, EMAIL_SMTP_URL);
			curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
			curl_easy_setopt(curl, CURLOPT_USERNAME, EMAIL_USERNAME);
			curl_easy_setopt(curl, CURLOPT_PASSWORD, EMAIL_PASSWORD);
			curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<" EMAIL_USERNAME ">");
		}
		struct curl_slist* recps = NULL;
		recps = curl_slist_append(recps, req->to);
	//	recps = curl_slist_append(recps, CC);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recps);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, EmailContentReader);
		curl_easy_setopt(curl, CURLOPT_READDATA, &(req->content));
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		CURLcode res = curl_easy_perform(curl);

		if(req) delete req;
		curl_slist_free_all(recps);
		dwWaitResult = WaitForSingleObject( EmailRequest::mutex, INFINITE);
	}
	
	curl_easy_cleanup(curl);

	EmailRequest::email_thread_up = false;
	ReleaseMutex( EmailRequest::mutex );

	return 0;
}

size_t EmailContentReader(char* buffer, size_t size, size_t nitems, void* userdata)
{
	EmailContent* ec = (EmailContent*)userdata;
	size_t sendB = size * nitems;
	if(ec->remaining_bytes < sendB) sendB = ec->remaining_bytes;

	memmove(buffer, ec->message, sendB);
	ec->remaining_bytes -= sendB;
	ec->message += sendB;

	return sendB;
}

#define FROM "From: Monopoly Game"
#define TO   "\r\nTo: "
#define SUBJ "\r\nSubject: "
#define MIMV "\r\nMIME-Version: 1.0"
#define CTEN "\r\nContent-Transfer-Encoding: BASE64"
#define CTYP "\r\nContent-Type: text/plain;charset=ISO-8859-1\r\n"

void SendEmail(const char* to, const char* subject, const char* message)
{
	std::stringstream msg;
	msg << FROM << TO << '<' << to << '>' << SUBJ << subject << MIMV << CTEN << CTYP;
	msg << base64_encode((const unsigned char*)message,strlen(message));

	EmailRequest::Add(to, msg.str().c_str());
}

std::string CreateEmailContent(const std::string& token, const char* status)
{
	std::stringstream buf;
	std::string url("http://monopoly/#/continue?token="); url += token;
	buf << "Monopoly game notification!\r\n";
	buf << "Continue the game at: "<< url << " .\r\n";
	buf << status;
	return buf.str();
}

#else

// Stubs

void SendEmail(const char* to, const char* subject, const char* message){}

std::string CreateEmailContent(const std::string& token, const char* status){return std::string();}

void SetEmailServerHost(char* host){}

#endif