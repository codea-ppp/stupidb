#ifndef _STUPIDB_SRC_DBARGS_H_
#define _STUPIDB_SRC_DBARGS_H_

namespace stupid
{

class dbargs
{
public:
	dbargs(const char* config_path); // json
	dbargs(const char* host, 
		   const char* user,
		   const char* password,
		   const char* db,
		   unsigned int port,
		   const char* sock, 
		   const unsigned int max_connection);

	~dbargs();

public:
	char* _host;
	char* _user;
	char* _password;
	char* _db;
	unsigned int _port;
	char* _sock;
	unsigned int _max_connection;

	bool _is_right;
};

}

#endif 	// _STUPIDB_SRC_DBARGS_H_
