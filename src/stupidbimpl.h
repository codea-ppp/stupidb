#ifndef _STUPIDB_SRC_STUPIDBIMPL_H_
#define _STUPIDB_SRC_STUPIDBIMPL_H_

#include <vector>
#include <string>
#include <map>

#include <mysql/mysql.h>

#include "src/dbargs.h"

namespace stupid
{
class stupidb_impl
{
public:
	stupidb_impl(const char* config_path);
	~stupidb_impl();

	bool is_open() const { return _is_open; }

	std::map<std::string, std::vector<std::string>> query(const std::string& statment);
 
private:
	MYSQL* open() const; // must be freed by mysql_close

private:
	bool _is_open;

	const dbargs _args;
 
	MYSQL* _db;
};

}

#endif 	// _STUPIDB_SRC_STUPIDBIMPL_H_
