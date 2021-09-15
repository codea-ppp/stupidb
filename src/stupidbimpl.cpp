#include "src/stupidbimpl.h"

#include <vector>
#include <map>

#include <mysql/mysql.h>

namespace stupid
{

stupidb_impl::stupidb_impl(const char* config_path) : 
	_is_open(false), _args(config_path), _db(open())
{
	if (_db != nullptr) _is_open = true;
}

stupidb_impl::~stupidb_impl()
{
	if (_is_open)
	{
		mysql_close(_db);
		_is_open = false;
	}
}

std::map<std::string, std::vector<std::string>> stupidb_impl::query(const std::string& statment)
{
	std::map<std::string, std::vector<std::string>> accu;
	if (!is_open())
		return accu;

	if (mysql_real_query(_db, statment.c_str(), statment.length()))
	{
		printf("%s\n", mysql_error(_db));
		return accu;
	}

	MYSQL_RES* ret = mysql_store_result(_db);
	if (!ret)
	{
		printf("%s\n", mysql_error(_db));
		return accu;
	}

	unsigned int num_fields = mysql_num_fields(ret);
	unsigned int i;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(ret)))
	{
		unsigned long* lengths = mysql_fetch_lengths(ret);

		for(i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
		}

		printf("\n");
	}

	mysql_free_result(ret);
	return accu;
}

MYSQL* stupidb_impl::open() const
{
	MYSQL* temp = mysql_init(nullptr);
	if (nullptr == temp)
		return nullptr;

	if (nullptr == mysql_real_connect(
			temp, 
			_args._host, 
			_args._user, 
			_args._password, 
			_args._db, 
			_args._port, 
			_args._sock, 
			0)
	) {
		return nullptr;
	}

	return temp;
}

}
