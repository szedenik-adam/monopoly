#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "websock/service/ws_user.h"

// Dynamic content generator.
// Contains static functions used by the HTTP and websocket server to create response for the requests.
class ContentGenerator
{
	static inline void WriteRoomInfo_Detailed(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map);
	static inline void    WriteRoomInfo_Short(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map);
	static void WriteRoomInfo(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map, bool write_name);

	static inline void WriteGameInfoDetailed(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map);
	static inline void    WriteGameInfoShort(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map);
	static void WriteGameInfo(rapidjson::Writer<rapidjson::StringBuffer>& w, Map* map, bool write_name);
public:
	// Allows the registration of new users.
	static void Register(char* input, rapidjson::Writer<rapidjson::StringBuffer>& w, int* reply_code = 0, char** reply_reason = 0);

	// Handles the user login (result is a session associated with the user).
	static void Login(char* input, rapidjson::Writer<rapidjson::StringBuffer>& w, int* reply_code = 0, char** reply_reason = 0);
	
	// Associates a websocket connection with a user.
	static void WSLogin(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Handles the continue requests coming from the emailed links.
	static void Continue(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);

	// Creates a new room.
	static void CreateRoom(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);

	// Join room.
	static void JoinRoom(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);

	// Leave room.
	static void LeaveRoom(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);

	// Start game (room -> map).
	static void StartGame(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);

	// Player roll command.
	static void Roll(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player end turn command.
	static void EndTurn(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player buy field command.
	static void BuyField(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player sell field command.
	static void SellField(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player leaves jail with card command.
	static void LeaveJailWCard(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player leaves jail with paying command.
	static void LeaveJailWPaying(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Player sends a bid.
	static void Bid(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all);
	
	// Serves the /createroom calls.
	//static void CreateRoom(struct evhttp_request *req, void *arg);
};
