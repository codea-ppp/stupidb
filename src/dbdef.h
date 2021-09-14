#ifndef _STUPIDB_SRC_DBDEF_H_
#define _STUPIDB_SRC_DBDEF_H_

#include <string>

namespace stupid
{

class dbargs
{
public:
	dbargs(const std::string& config_path); // yaml
 
private:
	std::string user;
	std::string password;
	std::string db;
	std::string sock;
	std::string host;
	unsigned int port;
	unsigned long flags;
};

}

#endif 	// _STUPIDB_SRC_DBDEF_H_
