#ifndef _STUPIDB_SRC_DBARGS_H_
#define _STUPIDB_SRC_DBARGS_H_

#include <string>

#include <mysql.h>

namespace stupid
{

class dbargs
{
public:
	dbargs(const std::string& config_path); // yaml

	MYSQL* open() const; // must be freed by mysql_close
 
private:
	bool _is_load;

	std::string _host;
	std::string _user;
	std::string _password;
	std::string _db;
	unsigned int _port;
	std::string _sock;
	unsigned long _flags;
};

}

#endif 	// _STUPIDB_SRC_DBARGS_H_
