#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "field.h"
#include "../util/random_string.h"
class User;
class Map;

class Player
{
	unsigned long id;
	User* user;
	Map* map;

	std::string ctoken;

	long money;
	unsigned char steps; // Remaining steps to perform.
	unsigned int pos; // Points to a static Field descriptor.
	unsigned char d[2], double_rolls;
	unsigned char jail_escape_cards, jail_time;
	bool jailed;
	unsigned int card;

public:
	Player(User* user, Map* map):user(user),map(map),  ctoken(""),  money(0),steps(0),pos(0),jail_escape_cards(0),jail_time(0),jailed(false){}

	bool operator==(const User& u);

	User* GetUser(){return user;}
	Map* GetMap(){return map;}

	//const std::string& CreateContinueToken(){RandomString(this->ctoken,10); return this->ctoken;}
	const std::string& GetContinueToken(){if(this->ctoken.compare(std::string(""))==0) RandomString(this->ctoken,10); return this->ctoken;}

	void Start();
	FEvent Roll(unsigned char d1, unsigned char d2, bool& rolls_again);

	long GetMoney(){return money;}
	unsigned int& Position(){return pos;}
	unsigned int GetPosition(){return pos;}
	unsigned char GetJailTime(){return jail_time;}
	unsigned char GetJailEscapeCard(){return jail_escape_cards;}
	void AddMoney(unsigned int val) {money += val;}
	void RemoveMoney(unsigned int val) {money -= val;}
	void SetCard(unsigned int val){card = val;}
	unsigned int GetCard(){unsigned int cc = card; card = 0; return cc;} // Self destructing variable to only send the card once (don't worry it's a last week hack, normally I wouldn't write such a thing).
	unsigned char StepsRemaining(){return steps;}
	void AddJailEscapeCard(){jail_escape_cards++;}
	bool TryLeaveJailWCard();
	bool TryLeaveJailWPaying();
	void Jail(bool in);
	bool IsInJail(){return jailed;}
	bool IsDoubleRoll();

	friend class Field; // For easier access from Field class.
};

#endif