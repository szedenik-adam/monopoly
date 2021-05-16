#include "player.h"
#include "user.h"

bool Player::operator==(const User& u)
{
	return u.GetId() == user->GetId();
}

void Player::Start()
{
	this->pos = 0;
	this->money = 150000;
	this->double_rolls = 0;
	this->jail_escape_cards = 0;
	this->jail_time = 0;
	this->jailed = false;
	this->card = 0;
}

FEvent Player::Roll(unsigned char d1, unsigned char d2, bool& rolls_again)
{	
	FEvent events = FEvent::Nothing;
	
	this->d[0]=d1;
	this->d[1]=d2;

	if(d1 == d2) {
		if(++double_rolls == 3) { // Go to jail.
			this->Jail(true);
		} else {
			this->steps = d1+d2;
			rolls_again = true; // Signal the map that the player can roll again.
		}
	} else {
		double_rolls = 0;
		this->steps = d1+d2;
	}
	Field* currf = Field::GetField(this->pos, &this->pos);

	if(currf->CanExit(this))
	{
		while(this->steps)
		{
			this->pos++;
			Field* f = Field::GetField(this->pos, &this->pos); // This modifies the position.
			this->steps--;
			if(steps == 0) events = f->EnterEvent(this);
			else f->TravelEvent(this);
		}
	}
	if(this->IsInJail()) this->jail_time++;
	if(this->IsInJail() && this->jail_escape_cards ) events |= FEvent::LeaveJailWithCard;
	if(this->IsInJail() && this->money > JailField::GetPenalty(this) ) events |= FEvent::LeaveJailWithPaying;

	return events;
}

bool Player::TryLeaveJailWCard()
{
	if(this->jail_escape_cards == 0) return false;
	this->jail_escape_cards--;
	this->Jail(false);
	return true;
}
bool Player::TryLeaveJailWPaying()
{
	int penalty = JailField::GetPenalty(this);
	if(this->money < penalty) return false;
	this->RemoveMoney(penalty);
	this->Jail(false);
	return true;
}

void Player::Jail(bool in)
{
	jailed = in;
	if(in){
		this->pos = JailField::GetPosition();
		this->jail_time = 0;
	}
}

bool Player::IsDoubleRoll()
{
	return this->d[0] == this->d[1];
}
