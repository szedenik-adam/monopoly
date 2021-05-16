#include "ws_user.h"
#include "../../rapidjson/stringbuffer.h"
#include "../../rapidjson/writer.h"
#include "../../rapidjson/document.h"

class WSParser
{
	class Command
	{
	public:
		char* cmd;
		void (*executor)(WSAuth* session, rapidjson::Document& d,
			rapidjson::Writer<rapidjson::StringBuffer>& w_self,
			rapidjson::Writer<rapidjson::StringBuffer>& w_group,
			rapidjson::Writer<rapidjson::StringBuffer>& w_all );
	};
	static Command ws_cmds[];
public:
	static void ParseFrame(WSAuth* session, char* input, size_t n);
};