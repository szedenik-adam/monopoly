#include "user.h"
#include "../websock/service/ws_user.h"
using namespace std;

User::~User()
{
	if(wsocket) wsocket->UnlinkUser();
}

void User::SetWSock(WSAuth* wsocket)
{
	if(this->wsocket != 0 && this->wsocket != wsocket)
		this->wsocket->UnlinkUser();
	this->wsocket = wsocket;
}
void User::ClearWSock(WSAuth* wsocket)
{
	if(this->wsocket == wsocket) this->wsocket = 0;
}
// Use this to unlink a user-websocket connection.
void User::ClearWSock() {if(this->wsocket) this->wsocket->UnlinkUser(); }
