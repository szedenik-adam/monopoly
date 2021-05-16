#include <vector>
#include "../model/map.h"
#include "../model/user.h"

class DataStore
{
	static std::vector<Map*> games;
	static unsigned long mlastId;

	static std::vector<User*> users;
	static unsigned long ulastId;

public:
	static Map* CreateMap(User* user, const std::string& name = "");
	static Map* GetMap(unsigned long id);
	static bool RemoveMap(unsigned long id);
	static const std::vector<Map*>& GetAllMaps(){return games;}

	static User* GetUser(unsigned long id);
	static User* GetUser(const char* name);
	static User* GetUserBySession(const char* session);
	static User* AddUser(const char* name, const char* email, const char* pass);
};