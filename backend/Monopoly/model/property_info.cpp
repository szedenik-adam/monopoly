#include "property_info.h"
#include "map.h"
#include "player.h"

void PropertyInfo::SetOwner(Player* p, Map* map)
{
	int j=0;
	while(j < MAX_PLAYERS_PER_MAP && map->GetPlayer(j)!=p) j++;
	if(j == MAX_PLAYERS_PER_MAP) return;
	this->owner = p;
	if(this->house_limit >= 1) this->house_count = 1;
	this->owner_ind = j;
}

void PropertyInfo::ClearOwner()
{
	this->owner = 0;
	this->house_count = 0;
	this->owner_ind = -1;
}