/**
 *
 * filename: user.h
 * summary:
 * author: caosiyang
 * email: csy3228@gmail.com
 *
 */
#ifndef USER_H
#define USER_H

#include "../connection.h"
#include "../../model/user.h"
#include "../../model/player.h"

class WSAuth
{
public:
	uint32_t id;
	ws_conn_t *wscon;
	string msg;

	User* user;
	Player* player;

	WSAuth();
	~WSAuth();
	void LinkUser(User* user);
	void UnlinkUser();

	// Sends a frame on this connection.
	void SendFrame(frame_buffer_t* fb);
	// Copies msg to the frame buffer.
	// Call DestroyFrame after sending the frame.
	static frame_buffer_t* CreateFrame(const char* msg, uint32_t len);
	static void DestroyFrame(frame_buffer_t* f);
}; 


WSAuth* create_session();


void destroy_session(WSAuth* session);


void frame_recv_cb(void *arg);


#endif
