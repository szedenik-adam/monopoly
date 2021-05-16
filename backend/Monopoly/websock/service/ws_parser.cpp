#include "ws_parser.h"
#include "connpool.h"
#include "../../ContentGenerator.h"
#include "../../rapidjson/Helper.h"
#include "../../email/email.h"
using namespace rapidjson;

WSParser::Command WSParser::ws_cmds[] =
	{ 
		{    "login", ContentGenerator::WSLogin  },
		{ "continue", ContentGenerator::Continue },

		{ "createroom", ContentGenerator::CreateRoom },
		{       "join", ContentGenerator::JoinRoom   },
		{      "leave", ContentGenerator::LeaveRoom  },
		{  "startgame", ContentGenerator::StartGame  },

		{      "roll", ContentGenerator::Roll },
		{       "end", ContentGenerator::EndTurn },
		{  "buyfield", ContentGenerator::BuyField },
		{ "sellfield", ContentGenerator::SellField },
		{ "leavejailwithcard",   ContentGenerator::LeaveJailWCard },
		{ "leavejailwithpaying", ContentGenerator::LeaveJailWPaying },

		{ "bid", ContentGenerator::Bid },

		{ 0, 0 } // Terminating zero, don't remove.
	};

void WSParser::ParseFrame(WSAuth* session, char* input, size_t n)
{
	rapidjson::StringBuffer s_self, s_group, s_all;
	rapidjson::Writer<StringBuffer> w_self(s_self);
	rapidjson::Writer<StringBuffer> w_group(s_group);
	rapidjson::Writer<StringBuffer> w_all(s_all);

	try {
		Document d; fprintf(stderr, "Received: %s\n",input);
		if (d.ParseInsitu(input).HasParseError()) {
			fprintf(stderr, "Parse error\n");
			throw rapidjson_exception();
		}
		if(!d.HasMember("cmd")) {throw std::string("no_cmd_param");}
		const char* cmd  = ReadString(d,"cmd");
		
		WSParser::Command* cmdi = ws_cmds;
		while(cmdi->cmd) {
			if(strcmp(cmdi->cmd, cmd) == 0) { // Strings are equal.
				w_self.StartObject(); w_group.StartObject(); w_all.StartObject(); // Prepare JSON writers.
				cmdi->executor(session, d, w_self, w_group, w_all); // Execute the command handler.
				goto CMD_EXECUTED;
				// Using goto to jump in 1 direction doesn't make the code spaghetti.
				// And if this makes the code harder to understand for you then maybe you are bad at programming.
			}
			cmdi++;
		}
		return; // No string match were found, exit.

CMD_EXECUTED:
		size_t self_size  =  s_self.GetSize();
		size_t group_size = s_group.GetSize();
		size_t all_size   =   s_all.GetSize();

		if(self_size > 1)
		{
			w_self.EndObject();

			frame_buffer_t* f = WSAuth::CreateFrame(s_self.GetString(), self_size+1);
			session->SendFrame(f);
			WSAuth::DestroyFrame(f);
		}
		if(all_size > 1)
		{
			w_all.EndObject();

			frame_buffer_t* f = WSAuth::CreateFrame(s_all.GetString(), all_size+1);
			for(int i=0;i<sessions.size();i++) {
				WSAuth* s = sessions[i];
				if(s->user)	s->SendFrame(f);
			}
			WSAuth::DestroyFrame(f);
		}
		if(group_size > 1)
		{
			w_group.EndObject();

			//session -> player -> map -> players -> sessions
			frame_buffer_t* f = WSAuth::CreateFrame(s_group.GetString(), group_size+1);
			Player* pl = session->player;
			if(!pl) printf("Error: WS group send failed (no player in session).\n");
			else {
				Map* map = pl->GetMap();
				if(!map) printf("Error: WS group send failed (no map in player's session).\n");
				else {
					for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
					{
						Player* p = map->GetPlayer(i);
						if(p) {
							WSAuth* pws = p->GetUser()->GetWebsocket();
							if(pws) pws->SendFrame(f); else
							{
								// Other player is offline, send email.
								char* pos = strstr(f->data, "\"type\":\"status\"");
								if(pos && (pos - f->data) < 20) {
									const std::string& token = p->GetContinueToken();
									std::string content = CreateEmailContent(token, f->data);
									SendEmail(p->GetUser()->GetEmail(),"Monopoly", content.c_str());
								}
							}
						}
					} // Look at that infinite amount of closing braces (4 out of 8 are needed for nullptr handling).
				}
			}
			WSAuth::DestroyFrame(f);
		}
	}
	catch (rapidjson_exception& e) {printf("rapidjson exception\n");}
	catch(...) {printf("other exception\n");}
}