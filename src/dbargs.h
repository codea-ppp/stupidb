#ifndef _STUPIDB_SRC_DBARGS_H_
#define _STUPIDB_SRC_DBARGS_H_

#include <string>

#include <mysql/mysql.h>
#include <json/json.h>

namespace stupid
{

class dbargs
{
public:
	dbargs(const std::string& config_path); // yaml

	MYSQL* open() const; // must be freed by mysql_close

	bool is_load() { return _is_load; }

private:
	bool loading_config_buffer(Json::Value* root, const char* config_path);
	bool loading_config(Json::Value& root);

private:
	bool _is_load;

	std::string _host;
	std::string _user;
	std::string _password;
	std::string _db;
	unsigned int _port;
	std::string _sock;
};

}

#endif 	// _STUPIDB_SRC_DBARGS_H_
