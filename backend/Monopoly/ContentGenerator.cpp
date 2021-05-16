#include "ContentGenerator.h"
#include <stdio.h>
#include "model/user.h"
#include "datastore/datastore.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/Helper.h"
using namespace rapidjson;

inline void ContentGenerator::WriteRoomInfo_Detailed(Writer<StringBuffer>& w, Map* map) {ContentGenerator::WriteRoomInfo(w,map,true);}
inline void ContentGenerator::   WriteRoomInfo_Short(Writer<StringBuffer>& w, Map* map) {ContentGenerator::WriteRoomInfo(w,map,false);}

void ContentGenerator::WriteRoomInfo(Writer<StringBuffer>& w, Map* map, bool write_name)
{
	WriteKVPair(w,"id",map->GetId());
	if(write_name) WriteKVPair(w,"name",map->GetName());

	WriteKVPair(w,"started", map->IsStarted());//(map->IsStarted())?("true"):("false"));

	w.String("players");
	w.StartArray();
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++) { 
		Player* p = map->GetPlayer(i);
		if(!p) continue;
		w.StartObject();
		WriteKVPair(w,"name",p->GetUser()->GetName());
		w.EndObject();
	}
	w.EndArray();
}
inline void ContentGenerator::WriteGameInfoDetailed(Writer<StringBuffer>& w, Map* map){WriteGameInfo(w,map,true);}
// Forcing the resending of the players names:
inline void ContentGenerator::   WriteGameInfoShort(Writer<StringBuffer>& w, Map* map){WriteGameInfoDetailed(w,map);}//WriteGameInfo(w,map,false);}

void ContentGenerator::WriteGameInfo(Writer<StringBuffer>& w, Map* map, bool write_name)
{
	WriteKVPair(w,"type","status");

	WriteKVPair(w,"round",map->GetRound());
	
	w.String("players");
	w.StartArray();
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++) { 
		Player* p = map->GetPlayer(i);
		if(!p) break;
		w.StartObject();
		if(write_name) WriteKVPair(w,"name",p->GetUser()->GetName());
		WriteKVPair(w,"money",p->GetMoney());
		WriteKVPair(w,"pos",(unsigned long)p->GetPosition());
		WriteKVPair(w,"jailcard",(unsigned long)p->GetJailEscapeCard());
		WriteKVPair(w,"jailed", p->IsInJail());

		unsigned long card = p->GetCard();
		if(card) WriteKVPair(w,"card", card);

		w.EndObject();
	}
	w.EndArray();
	WriteKVPair(w,"nextplayer",map->GetNextPlayerId());

	w.String("actions");
	w.StartArray();
	FEvent events = map->GetEvents();
	if(events & FEvent::Roll)	w.String("roll");
	if(events & FEvent::End)	w.String("end");

	if(events & FEvent::BuyField)           w.String("buyfield");
	if(events & FEvent::SellField)			w.String("sellfield");
	if(events & FEvent::LeaveJailWithCard)  w.String("leavejailwithcard");
	if(events & FEvent::LeaveJailWithPaying)w.String("leavejailwithpaying");
	w.EndArray();

	w.String("lastroll");
	w.StartArray();
	w.Uint(map->GetRoll(0));
	w.Uint(map->GetRoll(1));
	w.EndArray();

	w.String("properties");
	w.StartArray();
	PropertyInfo* pinfo = map->GetProperties();
	for(int i=0; i < PropertyField::GetCount(); i++)
	{
		w.StartObject();
		WriteKVPair(w, "pos", (unsigned long)pinfo[i].map_index);
		int owner_ind = pinfo[i].GetOwnerInd();
		if(owner_ind != -1) {
			WriteKVPair(w, "owner", (unsigned long)owner_ind);
			if(pinfo[i].house_limit > 0) WriteKVPair(w, "houses", (unsigned long)pinfo[i].house_count);
		}
		WriteKVPair(w, "value", (unsigned long)pinfo[i].value);
		w.EndObject();
	}
	w.EndArray();
}

// Serves the /register webserver calls.
void ContentGenerator::Register(char* input, rapidjson::Writer<StringBuffer>& w, int* reply_code, char** reply_reason)
{
	try {
		Document d; fprintf(stderr, "Received: %s\n",input);
		if (d.ParseInsitu(input).HasParseError()) {
			fprintf(stderr, "Parse error\n");
			throw rapidjson_exception();
		}
		if(!d.HasMember("name") || !d.HasMember("email") || !d.HasMember("pass"))
			throw std::string("bad_members");

		const char* name  = ReadString(d,"name");
		const char* email = ReadString(d,"email");
		const char* pass = ReadString(d,"pass");

		w.StartObject();

		User* u = DataStore::AddUser(name, email, pass); 
		if(u == InvalidUser)
		{	WriteKVPair(w,"error","Registration failed!"); if(reply_code) {*reply_code = 403; *reply_reason = "Forbidden";}
		} else {
			WriteKVPair(w,"id",u->GetId());                if(reply_code) {*reply_code = 200; *reply_reason = "OK";}
		}
		w.EndObject();
		return;
	}
	catch (rapidjson_exception& e) {printf("rapidjson exception\n");}
	catch(...) {printf("other exception\n");}

	if(reply_code) {*reply_code = 403; *reply_reason = "Forbidden";}
}

// Serves the /login calls.
void ContentGenerator::Login(char* input, rapidjson::Writer<StringBuffer>& w, int* reply_code, char** reply_reason)
{
	try {
		Document d; fprintf(stderr, "Received: %s\n",input);
		if (d.ParseInsitu(input).HasParseError()) {
			fprintf(stderr, "Parse error\n");
			throw rapidjson_exception();
		}
		if(!d.HasMember("name") || !d.HasMember("pass"))
			throw std::string("bad_members");

		const char* name  = ReadString(d,"name");
		const char* pass = ReadString(d,"pass");

		w.StartObject();

		User* u = DataStore::GetUser(name); bool error = false;
		if(u == InvalidUser)
		{	WriteKVPair(w,"error","User not exists!"); error = true;
		} else {
			const char* session = u->Login(pass);
			if(session == 0)
			   { WriteKVPair(w,"error","Bad password!"); error = true; }
			else WriteKVPair(w,"session", session);
		}

		w.EndObject();

		if(reply_code)
		if(!error)
		     { *reply_code = 200; *reply_reason = "OK"; }
		else { *reply_code = 401; *reply_reason = "Unauthorized"; }
		return;
	}
	catch (rapidjson_exception& e) {printf("rapidjson exception\n");}
	catch(...) {printf("other exception\n");}

	if(reply_code){ *reply_code = 403; *reply_reason = "Forbidden"; }
}

// Websocket authentication with http-session.
void ContentGenerator::WSLogin(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(!d.HasMember("session")) throw std::string("bad_members");
	const char* http_session = ReadString(d,"session");

	User* u = DataStore::GetUserBySession(http_session);

	if(u)
	{
		session->LinkUser(u); printf("websocket login success\n");

		WriteKVPair(w_self,"type","login");

		// + Add param from email game continuation to load map->player.
		// Without it dont assign player (1 user can have more player objects).
		// Just highlight the rooms where the player mathes with the user.

		// Write rooms here.
		w_self.String("maps");
		w_self.StartArray();
		const std::vector<Map*>& maps = DataStore::GetAllMaps();
		for(int i=0; i < maps.size(); i++)
		{
			if(maps[i]->IsStarted() == false || maps[i]->Contains(u))
			{
				w_self.StartObject();
				WriteRoomInfo_Detailed(w_self, maps[i]);
				w_self.EndObject();
			}
		}
		w_self.EndArray();
	} else {
		printf("websocket login fail\n");

		WriteKVPair(w_self,"type","login");
		WriteKVPair(w_self,"error","Websocket login failed!");
	}
}

// Websocket continue request to rejoin a game from an email url.
void ContentGenerator::Continue(WSAuth* session, rapidjson::Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(!d.HasMember("token")) {
		// Error: Token field is not set.
		WriteKVPair(w_self,"type","continue");
		WriteKVPair(w_self,"error","Required field: token.");
		return;
	}

	std::string ctoken = ReadString(d,"token");

	const std::vector<Map*>& maps = DataStore::GetAllMaps();
	for(int i=0; i < maps.size(); i++)
	{
		for(int j=0; j < MAX_PLAYERS_PER_MAP; j++) { 
			Player* p = maps[i]->GetPlayer(j);
			if(!p) continue;
			const std::string& pct = p->GetContinueToken();

			if(pct.compare(ctoken)==0) {
				unsigned long map_id = p->GetMap()->GetId(); // The map to join.
				session->LinkUser(p->GetUser()); // Authenticate the user.
				
				WriteKVPair(w_self,"type","continue");
				WriteKVPair(w_self,"map", map_id);
				WriteKVPair(w_self,"username", p->GetUser()->GetName());
				return;
			}
		}
	}
	
	WriteKVPair(w_self,"type","continue");
	WriteKVPair(w_self,"error", "Invalid token or something like that idk.");
}

// Creates a new room.
void ContentGenerator::CreateRoom(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->user == 0) {
		// Error: Not authenticated.
		WriteKVPair(w_self,"type","createroom");
		WriteKVPair(w_self,"error","Not authenticated.");
		return;
	}
	if(session->player != 0) {
		// Error: Already in a room.
		WriteKVPair(w_self,"type","createroom");
		WriteKVPair(w_self,"error","Already in a room.");
		return;
	}
	const char* map_name; // Added support for setting name of the room.
	if(d.HasMember("name")) map_name = ReadString(d,"name"); else map_name = "";

	Map* map = DataStore::CreateMap(session->user, map_name);
	session->player = map->GetOwner();
	
	// Getting a room with frame which players list's first username equals with the current user
	// means the room creation was successful.
	WriteKVPair(w_all,"type","room");
	WriteRoomInfo_Detailed(w_all,map);
}

// Join room.
void ContentGenerator::JoinRoom(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->user == 0) {
		// Error: Not authenticated.
		WriteKVPair(w_self,"type","join");
		WriteKVPair(w_self,"error","Not authenticated.");
		return;
	}
	bool rejoin = false;
	// This is no longer an error: player is allowed to rejoin a game.
	if(session->player != 0) {
		bool rejoin = true;
		/*// Error: Already in a room.
		WriteKVPair(w_self,"type","join");
		WriteKVPair(w_self,"error","Already in a room.");
		return;*/
	}
	if(!d.HasMember("id")) throw std::string("bad_members");
	unsigned int room_id = ReadUInt(d,"id");

	Map* map = DataStore::GetMap(room_id);
	if(map == 0)
	{
		// Error: Room doesn't exists.
		WriteKVPair(w_self,"type","join");
		WriteKVPair(w_self,"error","Room doesn't exists.");
		return;
	}
	Player* player = map->AddUser(session->user, &rejoin);
	session->player = player;

	if(rejoin)
	{
		if(map->IsStarted())
		// When rejoining, send the game's current state.
		WriteGameInfoDetailed(w_self, map);
		// And notify the others that the user is online again.
	} else
	{
		// Send info to everybody that the user is now in the selected room.
		WriteKVPair(w_all,"type","room");
		WriteRoomInfo_Short(w_all,map); // No need to send the room's name again.
	}
}

// Leave room.
void ContentGenerator::LeaveRoom(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","leave");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	unsigned long map_id = map->GetId();
	map = Map::RemoveUser(map, session->player->GetUser());
	session->player = 0;
	
	if(map == 0) {
		WriteKVPair(w_all,"type","roomremove");
		WriteKVPair(w_all,"id",map_id);
		return;
	}
	// Send info to everybody that the user has left his room.
	WriteKVPair(w_all,"type","room");
	WriteRoomInfo_Short(w_all,map);
}

// Start game.
void ContentGenerator::StartGame(WSAuth* session, Document& d, Writer<StringBuffer>& w_self, Writer<StringBuffer>& w_group, Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","startgame");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(map->GetOwner() != session->player) {
		// Error: Not the owner of the room.
		WriteKVPair(w_self,"type","startgame");
		WriteKVPair(w_self,"error","Not the owner of the room.");
		return;
	}
	if(map->IsStarted()) {
		// Error: Game already started.
		WriteKVPair(w_self,"type","startgame");
		WriteKVPair(w_self,"error","Game already started.");
		return;
	}
	// Send info to everybody that the game started.
	// In client side: users inside this room: put them to the game field.
	//             users not inside this room: remove this room from their room-list.
	WriteKVPair(w_all,"type","startgame");
	WriteKVPair(w_all,"mapid",map->GetId());
	// Set map to a started game.
	map->Start();
	
	// Send status update to the game's players.
	WriteGameInfoDetailed(w_group, map);
}

void ContentGenerator::Roll(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","roll");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","roll");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(!map->CanRoll(session->player)) {
		// Error: Not the player's turn.
		WriteKVPair(w_self,"type","roll");
		WriteKVPair(w_self,"error","It's not your turn.");
		return;
	}
	unsigned char d1,d2;
	if(d.HasMember("dice1")) d1 = ReadUInt(d,"dice1");
	else d1 = rand()%6+1;
	if(d.HasMember("dice2")) d2 = ReadUInt(d,"dice2");
	else d2 = rand()%6+1;

	FEvent events = map->Roll(d1,d2);
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::EndTurn(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","end");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","end");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(map->IsInBidPhase() && map->GetBidTimeLeft()==0) // A started bidding has ended.
	{
		map->EndBidding();
	}
	if(!map->CanEndTurn(session->player)) {
		// Error: Not the player's turn.
		WriteKVPair(w_self,"type","end");
		WriteKVPair(w_self,"error","It's not your turn.");
		return;
	}

	FEvent events = map->EndTurn();
	
	if(map->IsInBidPhase()) // A bidding has started.
	{
		WriteKVPair(w_group,"type","bid");
		WriteKVPair(w_group,"timeleftms", (unsigned long)map->GetBidTimeLeft());
		WriteKVPair(w_group,"highestbid", map->GetHighestBidVal());
		return;
	}
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::BuyField(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","buyfield");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","buyfield");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(!map->CanBuyField(session->player)) {
		// Error: Not the player's turn.
		WriteKVPair(w_self,"type","buyfield");
		WriteKVPair(w_self,"error","You cannot buy now.");
		return;
	}

	FEvent events = map->BuyField();
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::SellField(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","sellfield");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","sellfield");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	unsigned int pfid;
	if(d.HasMember("field")) pfid = ReadUInt(d,"field");
	else {
		WriteKVPair(w_self,"type","sellfield");
		WriteKVPair(w_self,"error","Missing parameter field (id).");
		return;
	}
	int fid = PropertyField::MapPosToPPos(pfid);
	if(fid==-1) {
		// Error: The field at pfid is not a property field.
		WriteKVPair(w_self,"type","sellfield");
		WriteKVPair(w_self,"error","Not salable field.");
		return;
	}
	if(!map->CanSellField(session->player, fid)) {
		// Error: Not the player's  property.
		WriteKVPair(w_self,"type","sellfield");
		WriteKVPair(w_self,"error","You cannot sell now.");
		return;
	}
	
	FEvent events = map->SellField(fid);
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::LeaveJailWCard(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","leavejailwithcard");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","leavejailwithcard");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(!session->player->TryLeaveJailWCard()) {
		// Error: Not the player's turn or not in jail or don't have card.
		WriteKVPair(w_self,"type","leavejailwithcard");
		WriteKVPair(w_self,"error","You cannot sell now.");
		return;
	}
	
	//FEvent events = map->GetEvents();
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::LeaveJailWPaying(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","leavejailwithpaying");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","leavejailwithpaying");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(!session->player->TryLeaveJailWPaying()) {
		// Error: Not the player's turn or not in jail or don't have card.
		WriteKVPair(w_self,"type","leavejailwithpaying");
		WriteKVPair(w_self,"error","You cannot sell now.");
		return;
	}
	
	//FEvent events = map->GetEvents();
	WriteGameInfoShort(w_group, map);
}

void ContentGenerator::Bid(WSAuth* session, Document& d, rapidjson::Writer<rapidjson::StringBuffer>& w_self, rapidjson::Writer<rapidjson::StringBuffer>& w_group, rapidjson::Writer<rapidjson::StringBuffer>& w_all)
{
	if(session->player == 0) {
		// Error: Not in any room.
		WriteKVPair(w_self,"type","bid");
		WriteKVPair(w_self,"error","Not in a room.");
		return;
	}
	Map* map = session->player->GetMap();
	if(!map->IsStarted()) {
		// Error: Game is not started yet.
		WriteKVPair(w_self,"type","bid");
		WriteKVPair(w_self,"error","Game is not started.");
		return;
	}
	if(!map->IsInBidPhase()) {
		// Error: No bidding phase.
		WriteKVPair(w_self,"type","bid");
		WriteKVPair(w_self,"error","No bidding phase.");
		return;
	}
	
	if(!d.HasMember("value")) {
		// Error: Missing bidding value field.
		WriteKVPair(w_self,"type","bid");
		WriteKVPair(w_self,"error","Missing bidding value field.");
		return;
	}
	unsigned int value = ReadUInt(d,"value");
	unsigned int highest_bid = map->GetHighestBidVal();

	if(value < highest_bid && value!=0) {
		// Error: Too low bid value.
		WriteKVPair(w_self,"type","bid");
		WriteKVPair(w_self,"error","Too low bid value.");
		return;
	}

	bool bid_accepted = map->TryBid(session->player, value);

	if(bid_accepted) {
		WriteKVPair(w_group,"type","bid");
		WriteKVPair(w_group,"timeleftms", (unsigned long)map->GetBidTimeLeft());
		WriteKVPair(w_group,"highestbid", map->GetHighestBidVal());
		WriteKVPair(w_group,"highestbidder", map->GetHighestBidder());
	} else {
		// When unaccepting bids, notify the players about the new board status.
		WriteGameInfoShort(w_group, map);
	}
}