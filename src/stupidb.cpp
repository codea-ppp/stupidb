#include "src/stupidb.h"

#include <mysql.h>

#include <string>

namespace stupid
{

stupidb::stupidb(const dbargs& args) : _is_open(false), _args(args), _db(nullptr)
{
	_db = args.open();
	if (_db != nullptr)
		_is_open = true;
}	

stupidb::~stupidb()
{
	if (_is_open)
	{
		mysql_close(_db);
	}
}

}
