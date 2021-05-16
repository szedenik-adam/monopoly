#ifndef _PROPERTY_INFO_H_
#define _PROPERTY_INFO_H_

class Player;
class Map;

class PropertyInfo
{
	Player* owner;
	int owner_ind; // Owner index in the map's player array.
public:
	unsigned int map_index;
	unsigned int house_count;
	unsigned int house_limit;
	unsigned int value;

	PropertyInfo(){}

	PropertyInfo(unsigned int value, unsigned int map_index, unsigned int house_limit):
		owner(0),owner_ind(-1),map_index(map_index),
		house_count(0),house_limit(house_limit),value(value*100)
	{
	}
	
	Player* GetOwner(){return owner;}

	int GetOwnerInd(){return owner_ind;}

	void SetOwner(Player* p, Map* map);

	void ClearOwner();

	unsigned int GetTrespassingCost()
	{
		return ((value)*(house_count+5))/10;
	}
};

#endif