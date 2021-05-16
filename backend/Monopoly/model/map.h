#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include <vector>
#include <ctime>
#include "player.h"
#include "property_info.h"

#define MAX_PLAYERS_PER_MAP 8

class Map
{
	unsigned long id;
	std::string name;
	Player* players[MAX_PLAYERS_PER_MAP];

	unsigned int round; // 0 means the game not started yet.
	unsigned char playerCount;
	unsigned char pOrderNext;
	FEvent currentEvents;
	unsigned char lastRoll[2];
	PropertyInfo* properties;

	bool bidding_phase;
	clock_t bidding_end;
	time_t bidding_end_date;
	int best_bidder;
	unsigned int best_bid;
	unsigned int bid_target;

	Map(unsigned long id = 0, const std::string& name = ""):id(id),round(0),name(name),properties(0),bidding_phase(false){for(int i=0;i<MAX_PLAYERS_PER_MAP;i++) players[i]=0;}
	friend class DataStore;
public:
	~Map();

	Player* AddUser(User* user, bool* rejoin = 0);
	static Map* RemoveUser(Map* map, User* user);
	unsigned long GetId(){return id;}
	unsigned long GetRound(){return round;}
	const char* GetName(){return name.c_str();}
	Player* GetOwner(){return players[0];}
	Player* GetPlayer(int i){return players[i];}
	bool IsStarted(){return round!=0;}
	void Start();

	// Queries to check if a command is allowed.
	bool CanRoll(Player* player);
	bool CanEndTurn(Player* player);
	bool CanBuyField(Player* player);
	bool CanSellField(Player* player, unsigned int pfid);

	// Commands to interact with the model.
	FEvent Roll(unsigned int d1, unsigned int d2);
	FEvent EndTurn();
	FEvent BuyField();
	FEvent SellField(unsigned int fid);

	// Bidding related methods.
	bool IsInBidPhase(){return bidding_phase;}
	bool TryBid(Player* p, unsigned int val);
	void EndBidding();
	long GetBidTimeLeft();
	unsigned long GetHighestBidVal(){return best_bid;}
	unsigned long GetHighestBidder(){return best_bidder;}

	unsigned char GetRoll(unsigned int n){ if(n>1)return 0; else return lastRoll[n]; }
	unsigned long GetNextPlayerId();
	FEvent GetEvents(){return currentEvents;}
	PropertyInfo* GetProperties(){return properties;}
	bool Contains(User* user);
};

#endif