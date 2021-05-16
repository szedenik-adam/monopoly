#include "datastore.h"
using namespace std;

vector<Map*> DataStore::games;
unsigned long DataStore::mlastId = 1;

vector<User*> DataStore::users;
unsigned long DataStore::ulastId = 1;

Map* DataStore::CreateMap(User* user, const string& name)
{
	Map* map = new Map(DataStore::mlastId++, (name == "")?(std::string(user->GetName())+"'s Game"):(name));
	map->players[0] = new Player(user, map);
	
	games.push_back(map);
	return map;
}

Map* DataStore::GetMap(unsigned long id)
{
	for(int i=0; i < DataStore::games.size(); i++)
		if(DataStore::games[i]->id == id) return DataStore::games[i];
	return 0;
}

bool DataStore::RemoveMap(unsigned long id)
{
	for(int i=0; i < DataStore::games.size(); i++)
		if(DataStore::games[i]->id == id) { delete DataStore::games[i]; DataStore::games.erase(DataStore::games.begin()+i); return true; }
	return false;
}


User* DataStore::GetUser(unsigned long id)
{
	return InvalidUser;
}

User* DataStore::GetUser(const char* name)
{
	for(int i=0; i < DataStore::users.size(); i++)
		if(DataStore::users[i]->name == name) return DataStore::users[i];
	return 0;
}

User* DataStore::GetUserBySession(const char* session)
{
	//printf("GetUser (session: %s)\n",session);

	for(int i=0; i < DataStore::users.size(); i++)
		if(DataStore::users[i]->session == session) return DataStore::users[i];
	return 0;
}

User* DataStore::AddUser(const char* name, const char* email, const char* pass)
{
	// Check parameters.
	if(name == "") return InvalidUser;
	for(int i=0; i < DataStore::users.size(); i++)
		if(DataStore::users[i]->name == name) return InvalidUser;

	User* u = new User();
	u->name = name;
	u->email = email;
	u->pass = pass;

	// Add to storage and get id.
	DataStore::users.push_back(u);
	u->id = DataStore::ulastId++;

	return u;
}