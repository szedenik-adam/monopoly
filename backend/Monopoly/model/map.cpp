#include "map.h"
#include "user.h"
#include "../datastore/datastore.h"
#include <cstring>
#include <stdio.h>
using namespace std;

Map::~Map()
{
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
		if(this->players[i] != 0) { delete this->players[i];}
	if(properties) delete[] properties;
}


Player* Map::AddUser(User* user, bool* rejoin)
{
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
		if(this->players[i] != 0 && strcmp(this->players[i]->GetUser()->GetName(),user->GetName())==0) {
			printf("Rejoin (Map::AddUser) successful.\n");
			if(rejoin) *rejoin = true; // Signal the rejoin to the caller.
			return this->players[i];}

	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
		if(this->players[i] == 0) {this->players[i]= new Player(user, this); return this->players[i];}
	return 0;
}

Map* Map::RemoveUser(Map* map, User* user)
{
	bool isEmpty = true;
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
	{
		if(map->players[i] != 0)
		{
			if(*(map->players[i]) == *user)
			{
				delete map->players[i];
				map->players[i] = 0;
			}
			else {isEmpty = false;}
		}
	}
	if(isEmpty) {DataStore::RemoveMap(map->id); return 0;} return map;
}

void Map::Start()
{
	if(round == 0)
	{
		round = 1;
		playerCount = 0;
		for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
		{
			if(players[i] != 0)
			{
				players[i]->Start();
				int j = playerCount; // Compresses the players array.
				if(j != i) {players[j]=players[i]; players[i]=0;}
				playerCount = j+1;
			}
		}
		pOrderNext = 0;

		bidding_phase = false;
		bid_target = 999;

		properties = new PropertyInfo[PropertyField::GetCount()];
		for(int i=0; i < PropertyField::GetCount(); i++) properties[i] = PropertyInfo(PropertyField::GetPropertyInfo(i));

		currentEvents = FEvent::Roll;
	}
}

bool Map::CanRoll(Player* player)
{
	if( this->IsStarted() && this->players[this->pOrderNext] == player &&
		 (currentEvents & FEvent::Roll) )
		return true;
	    return false;
}

bool Map::CanEndTurn(Player* player)
{
	if( this->IsStarted() && this->players[this->pOrderNext] == player &&
		(currentEvents & FEvent::End) )
		return true;
		return false;
}
bool Map::CanBuyField(Player* player)
{
	if( this->IsStarted() && this->players[this->pOrderNext] == player &&
		(currentEvents & FEvent::BuyField) )
		return true;
		return false;
}
bool Map::CanSellField(Player* player, unsigned int fid)
{
	if( this->IsStarted() /*&& this->players[this->pOrderNext] == player*/ &&
		//(currentEvents & FEvent::SellField) &&
		fid < PropertyField::GetCount() && this->properties[fid].GetOwner() == player )
		return true;
	    return false;
}

FEvent Map::Roll(unsigned int d1, unsigned int d2)
{
	Player* p = players[pOrderNext];
	bool rolls_again = false;
	lastRoll[0] = d1;
	lastRoll[1] = d2;
	FEvent events = p->Roll(d1, d2, rolls_again);

	if(events == FEvent::Nothing) {
		if(!rolls_again) return this->EndTurn();
		events = FEvent::Roll;
		events &= ~FEvent::End;
	} else {
		events |= FEvent::End;
	}

	if(rolls_again) events |= FEvent::Roll;
	currentEvents = events;
	return events;
}

FEvent Map::EndTurn()
{
	FEvent events = FEvent::Roll;

	if( (currentEvents & FEvent::BuyField) && !this->IsInBidPhase() )
	{
		unsigned int pos = players[pOrderNext]->Position();
		unsigned int pfi = PropertyField::MapPosToPPos(pos);
		if( this->bid_target != pfi ) // This cancels the repetition of the bid.
		{
			if(pfi != -1 && this->properties[pfi].GetOwner() == 0) {
				// Start the bidding for the not bought property.
				bidding_phase = true;
				bidding_end = clock() + CLOCKS_PER_SEC*10; // 10 sec
				bidding_end_date = time(0) + 10; // 10 sec
				best_bidder = -1;
				best_bid = 0; //this->properties[pfi].value;
				bid_target = pfi;
			}
			else { goto NEXTPLAYER; }
			return currentEvents;
		}
	}
	if(this->IsInBidPhase()) return currentEvents;

NEXTPLAYER:
	pOrderNext = (pOrderNext+1)%playerCount;
	if(pOrderNext == 0) {
		this->round++;
		if(this->round == 100) {
			events = FEvent::Nothing;
		}
	}

	currentEvents = events;
	return events;
}

FEvent Map::BuyField()
{
	Player* p = players[pOrderNext];
	PropertyInfo* pinfo = this->properties + PropertyField::MapPosToPPos(p->GetPosition());
	if(pinfo->GetOwner() != p) {
		pinfo->SetOwner(p,this);
		p->RemoveMoney(pinfo->value);
	}
	else if(pinfo->house_count < pinfo->house_limit) {
		pinfo->house_count++;
		p->RemoveMoney(pinfo->value);
	}

	// Remove BuyField interaction.
	if(pinfo->house_count >= pinfo->house_limit) currentEvents &= ~FEvent::BuyField;
	// If the only option is to end the turn, automatically call it and step to the next player.
	if(currentEvents == FEvent::End) {currentEvents = this->EndTurn();}

	return currentEvents;
}

FEvent Map::SellField(unsigned int fid)
{
	if(fid >= PropertyField::GetCount()) return currentEvents;
	Player* p = players[pOrderNext];
	PropertyInfo* pinfo = this->properties + fid;
	p->AddMoney(pinfo->value * max<int>(1,pinfo->house_count));
	pinfo->ClearOwner();
	return currentEvents;
}

// Return value shows that are we in a bidding state or not.
bool Map::TryBid(Player* p, unsigned int val)
{
	if(this->bidding_phase == false)return false;
	int pid = 0;
	while(pid < this->playerCount && this->players[pid]!=p) pid++;

	if(pid == this->playerCount)    return false; // If this error is caught the server is about to die.
	if(this->best_bid >= val && val)return true;  // True: bidding phase is not ended (just the bid is not accepted).
	if(p->GetMoney() < val)         return true;

	long bidtime = this->GetBidTimeLeft();
	if(bidtime <= 0) { // Warning: a client needs to send a bid with 0 value after the bid ended to invoke this. (todo: replace this with a cancelable timeout callback)
		EndBidding();
		return false; // Hint: false tells the caller that the bidding is over.
	}
	/*if(bidtime < 3000)  { // Feature disabled until the UI refreshes the countdown properly.
		this->bidding_end = clock()+3*CLOCKS_PER_SEC; // Extend bidding time to 3 sec after a successful bid.
		this->bidding_end_date = time(0)+3;
	}*/
	this->best_bid = val;
	this->best_bidder = pid;
	return true;
}
void Map::EndBidding()
{
	bidding_phase = false; // End bidding phase.
	if(this->best_bidder != -1) {
		Player* winner = this->players[this->best_bidder]; 
		winner->RemoveMoney(this->best_bid); // Commit bid.
		this->properties[this->bid_target].SetOwner(winner,this);
	}
}

long Map::GetBidTimeLeft()
{
	time_t now_date = time(0);
	if( this->bidding_end_date + 1 < now_date ) { return 0; }

	clock_t now = clock();
	long ms_left = ((this->bidding_end - now)*1000)/CLOCKS_PER_SEC;
	printf("\nBidTimeLeft(ms): %ld \n");
	if(ms_left < 0) ms_left = 0;
	if(ms_left > 10000*CLOCKS_PER_SEC) ms_left = 0; // limit too big value
	return ms_left;
}


unsigned long Map::GetNextPlayerId()
{
	if(round == 0) return 0;
	return pOrderNext;
}

bool Map::Contains(User* user)
{
	for(int i=0; i < MAX_PLAYERS_PER_MAP; i++)
	{
		if(this->players[i] != 0)
		{
			if(*(this->players[i]) == *user)
			{
				return true;
			}
		}
	}
	return false;
}