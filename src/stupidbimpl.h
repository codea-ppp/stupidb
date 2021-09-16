#ifndef _STUPIDB_SRC_STUPIDBIMPL_H_
#define _STUPIDB_SRC_STUPIDBIMPL_H_

#include <vector>
#include <string>
#include <memory>
#include <map>

#include <mysql/mysql.h>

#include "src/stupidbalias.h"
#include "src/dbargs.h"

namespace stupid
{

class stupidb_impl
{
public:
	stupidb_impl(const dbargs& args);
	~stupidb_impl();

	bool is_open() const { return _is_open; }

	int query(const std::string& statment, column_ret_pt accu) const;
	int query(const std::string& statment, row_ret_pt accu) const;
 
private:
	MYSQL* open() const; // must be freed by mysql_close
	MYSQL_RES* query(const std::string& statment) const;

	void fill(MYSQL_RES* result, column_ret_pt accu) const;
	void fill(MYSQL_RES* result, row_ret_pt accu) const;

private:
	bool _is_open;
	const dbargs _args;
	MYSQL* _db;
};

}

#endif 	// _STUPIDB_SRC_STUPIDBIMPL_H_
