
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

inline void WriteKVPair(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* key, const char* val) {w.String(key); w.String(val);}
inline void WriteKVPair(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* key, unsigned long val) {w.String(key); w.Uint64(val);}
inline void WriteKVPair(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* key, long val) {w.String(key); w.Int64(val);}
inline void WriteKVPair(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* key, bool val) {w.String(key); w.Bool(val);}
inline const char* ReadString(rapidjson::Document& d, const char* key) {rapidjson::Value& val = d[key]; return val.GetString();}
inline unsigned int ReadUInt(rapidjson::Document& d, const char* key) {rapidjson::Value& val = d[key]; return val.GetUint();}