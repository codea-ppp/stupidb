#include "src/stupidb.h"

#include <mysql.h>

#include <string>

namespace stupid
{
	
stupidb::stupidb(const std::string& host,
				 const std::string& user,
				 const std::string& password,
				 const std::string& db,
				 const std::string& sock,
				 const unsigned int port): 
	_is_open(false),
	_host(host), 
	_user(user), 
	_password(password), 
	_db(db), 
	_sock(sock), 
	_port(port) 
{  
	if (mysql_init(&_handle) == nullptr)
	{
	}

	if (nullptr == mysql_real_connect(&_handle, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, sock.c_str(), 0)) 
	{

	}
}

stupidb::~stupidb()
{
	mysql_close(&_handle);
}

}
