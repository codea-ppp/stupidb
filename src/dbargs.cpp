#include "src/dbargs.h"

#include <string>

#include <yaml.h>

namespace stupid
{

dbargs::dbargs(const std::string& config_path) : _is_load(false)
{
}

MYSQL* dbargs::open() const
{
	if (!_is_load)
		return nullptr;

	MYSQL* temp = mysql_init(nullptr);
	if (nullptr == temp)
		return nullptr;

	mysql_real_connect(temp, 
					   _host.c_str(), 
					   _user.c_str(), 
					   _password.c_str(), 
					   _db.c_str(), 
					   _port, 
					   _sock.c_str(), 
					   _flags);

	if (nullptr == temp)
		return nullptr;

	return temp;
}

}
