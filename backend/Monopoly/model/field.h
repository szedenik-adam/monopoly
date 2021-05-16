#ifndef _FIELD_H_
#define _FIELD_H_

#include "stdlib.h"
#include <vector>
#include "field_events.h"
#include "property_info.h"

class Player;

class Field
{
	static std::vector<Field*>* fields; // Stores the fields in order of stepping on them.
	static std::vector<Field*>* BuildFields();
public:
	static Field* GetField(unsigned int ind, unsigned int* ind_loc = 0);

	virtual FEvent::e EnterEvent(Player* player);
	virtual void TravelEvent(Player* player);
	virtual bool CanExit(Player* player);
};

class StartField : public Field
{
public:
	virtual FEvent::e EnterEvent(Player* player);
	virtual void TravelEvent(Player* player);
};

class PropertyField : public Field
{
	unsigned int index;
	static unsigned int count;
protected:
	static PropertyInfo property_values[];
public:
	PropertyField(unsigned int cost, unsigned int map_pos, unsigned int house_limit = 4)
	 :index(count++)
	{ 
		property_values[index] = PropertyInfo(cost, map_pos, house_limit);
	}

	virtual FEvent::e EnterEvent(Player* player);

	unsigned int GetIndex(){return index;}

	static unsigned int GetCount(){return count;}

	static PropertyInfo GetPropertyInfo(unsigned int i)
	{
		return property_values[i];
	}

	static unsigned int MapPosToPPos(unsigned int mapi)
	{
		unsigned int p;
		unsigned int lower = 0, middle, upper = count-1;

		if (  property_values[lower].map_index < mapi  
           && mapi < property_values[upper].map_index )
		{
		  while ( upper > lower + 1 ) {
			 middle = ( upper + lower ) / 2;
			 if (property_values[middle].map_index == mapi) return middle;
			 if (property_values[middle].map_index < mapi) lower = middle;
			 else upper = middle;
		  };
		}
		if (property_values[lower].map_index == mapi) return lower;
		if (property_values[upper].map_index == mapi) return upper;
		return -1;
	}
};

class NoHousePropertyField : public PropertyField
{
public:
	NoHousePropertyField(unsigned int cost, unsigned int map_pos):PropertyField(cost, map_pos, 0){}
};

class DrawCardField : public Field
{
public:
	virtual FEvent::e EnterEvent(Player* player);
};

class JailField : public Field
{
	static int position;
public:
	JailField(size_t pos):Field(){position = pos;}
	static int GetPosition(){return position;}
	static int GetPenalty(Player* player);
	virtual bool CanExit(Player* player);
};

class TaxField : public Field
{
public:
	virtual FEvent::e EnterEvent(Player* player);
};

class GoToJailField : public Field
{
public:
	virtual FEvent::e EnterEvent(Player* player);
};

#endif