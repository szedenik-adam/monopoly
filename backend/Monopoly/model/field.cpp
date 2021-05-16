#include "field.h"
#include "player.h"
#include "map.h"

std::vector<Field*>* Field::fields = Field::BuildFields();
std::vector<Field*>* Field::BuildFields()
{
	std::vector<Field*>* fields = new std::vector<Field*>();
	fields->push_back(new StartField());
	fields->push_back(new PropertyField(60, fields->size()));
	fields->push_back(new DrawCardField());
	fields->push_back(new PropertyField(60, fields->size()));
	fields->push_back(new TaxField());
	fields->push_back(new NoHousePropertyField(200, fields->size()));
	fields->push_back(new PropertyField(100, fields->size()));
	fields->push_back(new Field()); // WTF is chance field?
	fields->push_back(new PropertyField(100, fields->size()));
	fields->push_back(new PropertyField(100, fields->size()));

	fields->push_back(new JailField(fields->size()));

	fields->push_back(new PropertyField(140, fields->size()));
	fields->push_back(new PropertyField(140, fields->size()));
	fields->push_back(new NoHousePropertyField(150, fields->size()));
	fields->push_back(new PropertyField(140, fields->size()));
	fields->push_back(new NoHousePropertyField(200, fields->size()));
	fields->push_back(new PropertyField(180, fields->size()));
	fields->push_back(new DrawCardField());
	fields->push_back(new PropertyField(180, fields->size()));
	fields->push_back(new PropertyField(180, fields->size()));

	fields->push_back(new Field()); // Free Parking.

	fields->push_back(new PropertyField(220, fields->size()));
	fields->push_back(new Field()); // chance field?
	fields->push_back(new PropertyField(220, fields->size()));
	fields->push_back(new PropertyField(240, fields->size()));
	fields->push_back(new NoHousePropertyField(200, fields->size()));
	fields->push_back(new PropertyField(260, fields->size()));
	fields->push_back(new PropertyField(260, fields->size()));
	fields->push_back(new NoHousePropertyField(150, fields->size()));
	fields->push_back(new PropertyField(280, fields->size()));

	fields->push_back(new GoToJailField());

	fields->push_back(new PropertyField(300, fields->size()));
	fields->push_back(new PropertyField(300, fields->size()));
	fields->push_back(new DrawCardField());
	fields->push_back(new PropertyField(320, fields->size()));
	fields->push_back(new NoHousePropertyField(200, fields->size()));
	fields->push_back(new Field()); // chance field?
	fields->push_back(new PropertyField(350, fields->size()));
	fields->push_back(new PropertyField(400, fields->size()));

	return fields;
}

// Field events.

Field* Field::GetField(unsigned int ind, unsigned int* ind_loc)
{
	if(ind >= Field::fields->size()) {
		ind %= Field::fields->size();
		if(ind_loc) *ind_loc=ind;
	}
	return (*Field::fields)[ind];
}

FEvent::e Field::EnterEvent(Player* player)
{
	return FEvent::Nothing;
}
void Field::TravelEvent(Player* player)
{
}
bool Field::CanExit(Player* player)
{
	return true;
}

// Start field events.

FEvent::e StartField::EnterEvent(Player* player)
{
	player->AddMoney(20000);

	return FEvent::Nothing;
}
void StartField::TravelEvent(Player* player)
{
	unsigned int field_index = player->StepsRemaining();
	Field* dstfield = Field::GetField(field_index + player->GetPosition());

	//void** vtable = *(void***)this;
	void** dstType = *(void***)dstfield; DrawCardField dcf;
	void** drawCardType = *(void***)(&dcf);

	if(dstType == drawCardType)
		  player->AddMoney(40000);
	else  player->AddMoney(20000);
}

// Property field events.

unsigned int PropertyField::count = 0;
PropertyInfo PropertyField::property_values[40];

FEvent::e PropertyField::EnterEvent(Player* player)
{
	FEvent events;

	Map* map = player->GetMap();
	PropertyInfo* prop = map->GetProperties() + this->index;
	Player* owner = prop->GetOwner();

	if(owner == 0) {
		if(player->GetMoney() > prop->value) events |= FEvent::BuyField;
	}
	else if(owner == player) {
		events |= FEvent::SellField;
		if(prop->house_count < prop->house_limit && player->GetMoney() > prop->value){
			events |= FEvent::BuyField;
		}
	}
	else {
		unsigned int penalty = prop->GetTrespassingCost();
		player->RemoveMoney(penalty);
		prop->GetOwner()->AddMoney(penalty);
	}

	return events.ee;
}

// Draw card field events.

FEvent::e DrawCardField::EnterEvent(Player* player)
{
	int cardtype = rand()%100;
	if(cardtype < 10) { // 10%
		player->Jail(true);
		player->SetCard(1);
	} else if(cardtype < 35) { // 25%
		player->AddJailEscapeCard();
		player->SetCard(2);
	} else if(cardtype < 75) { // 40%
		unsigned int money = ((cardtype-10)/15)*3000;
		player->AddMoney( money );
		player->SetCard(1<<30 | money);
	} else { // 25%
		unsigned int money = ((cardtype-50)/15)*3000;
		player->RemoveMoney( money );
		player->SetCard(1<<31 | money);
	}

	return FEvent::Nothing;
}

// Jail field events.

int JailField::position = 0;

int JailField::GetPenalty(Player* player)
{
	if(player->GetJailTime() < 3) return 5000;
	return 1500;
}

bool JailField::CanExit(Player *player)
{
	if(!player->IsInJail()) return true;
	if(player->IsDoubleRoll()) {
		player->Jail(false);
		return true;
	}
	return false;
}

// Tax field event.

FEvent::e TaxField::EnterEvent(Player* player)
{
	long percent_pay = player->GetMoney()/10;
	int normal_pay = 200;
	if(percent_pay < normal_pay && percent_pay >= 0) player->RemoveMoney(percent_pay);
	else  player->RemoveMoney(normal_pay);

	return FEvent::Nothing;
}

// GoToJail field event.

FEvent::e GoToJailField::EnterEvent(Player* player)
{
	//int jail_pos = JailField::GetPosition();
	player->Jail(true);

	return FEvent::Nothing;
}