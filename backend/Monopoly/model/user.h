#ifndef _USER_H_
#define _USER_H_

#include <vector>
#include "map.h"
#include "../util/random_string.h"

class WSAuth;

class User
{
	unsigned long id;
	std::string name, email, pass, session;
	std::vector<Map> games;
	WSAuth* wsocket;

	friend class DataStore;
public:
	User():id(0),wsocket(0)
	{}
	~User();

	bool operator==(const User& u) {return this->id == u.id;}
	bool operator!=(const User& u) {return this->id != u.id;}

	unsigned long GetId()const{return id;}
	const char* GetName()const{return name.c_str();}
	const char* GetEmail()const{return email.c_str();}
	WSAuth* GetWebsocket()const{return wsocket;}

	const char* Login(const char* pass)
	{
		if(this->pass != pass) return 0;
		//if(session.size() != 0) session.clear();
		
		RandomString(this->session,10);
//		printf("User login (new session: %s)\n",session.c_str());
		return session.c_str();
	}

	// Only use these 2 procedures from the websocket's connection object.
	void SetWSock(WSAuth* wsocket);
	void ClearWSock(WSAuth* wsocket);
	// Use this to unlink a user-websocket connection.
	void ClearWSock();
};

#define InvalidUser 0

#endif