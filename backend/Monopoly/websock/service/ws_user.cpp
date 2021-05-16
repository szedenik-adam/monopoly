#include "ws_user.h"
#include "ws_parser.h"

extern vector<WSAuth*> sessions;

WSAuth::WSAuth():id(0),wscon(ws_conn_new()),msg(""), user(0),player(0) {
}

WSAuth::~WSAuth() {
	if(this->user) user->ClearWSock(this);
}

void WSAuth::LinkUser(User* user) {
	if(this->user != 0) this->user->ClearWSock(this);
	this->user = user;
	user->SetWSock(this);
}

void WSAuth::UnlinkUser() {
	if(this->user == 0) return;
	this->user->ClearWSock(this);
	this->user = 0;
}

// Sends a frame on this connection.
void WSAuth::SendFrame(frame_buffer_t* fb)
{
	send_a_frame(this->wscon, fb);
}

// Copies msg to the frame buffer.
// Call DestroyFrame after sending the frame.
frame_buffer_t* WSAuth::CreateFrame(const char* msg, uint32_t len)
{
	printf("Sending: %s.\n",msg);
	return frame_buffer_new(1, 1, len, msg);
}

void WSAuth::DestroyFrame(frame_buffer_t* f) { frame_buffer_free(f); }


void destroy_session(WSAuth* session) {
	if (session) {
		if (session->wscon) {
			ws_conn_free(session->wscon);
		}
		delete session;
	}
}

void frame_recv_cb(void *arg) {
	WSAuth* user = (WSAuth*)arg;
	if (user->wscon->frame->payload_len > 0) {
		user->msg += string(user->wscon->frame->payload_data, user->wscon->frame->payload_len);
	}
	if (user->wscon->frame->fin == 1) {
		LOG("%s", user->msg.c_str());

		// Alternative: user->msg.c_str() !
		WSParser::ParseFrame(user, user->wscon->frame->payload_data, user->wscon->frame->payload_len );

		/*frame_buffer_t* fb = frame_buffer_new(1, 1, user->wscon->frame->payload_len, user->wscon->frame->payload_data);

		if (fb) {
			//send to other users
			for (int32_t i = 0; i < sessions.size(); ++i) {
				if (
				1) {
				//sessions[i] != user) {
#if 1
					if (send_a_frame(sessions[i]->wscon, fb) == 0) {
						LOG("i send a message");
					}
#endif
				}
			}

			frame_buffer_free(fb);
		}*/

		user->msg = "";
	}
}
