#include "src/stupidbimpl.h"

#include <utility>
#include <vector>
#include <map>

#include <mysql/mysql.h>

#include "src/zloghub.h"

namespace stupid
{

stupidb_impl::stupidb_impl(const dbargs& args) : 
	 _is_open(false), _args(args), _db(open())
{
	if (_db != nullptr)
	{
		_is_open = true;
		zlog_info(zloghub::oneline, "database %s opened", _args._db);
	}
}

stupidb_impl::~stupidb_impl()
{
	if (_is_open)
	{
		mysql_close(_db);
		_is_open = false;

		zlog_info(zloghub::oneline, "database %s closed", _args._db);
	}
}

int stupidb_impl::query(const std::string& statment, column_ret_pt accu) const
{
	MYSQL_RES* ret = query(statment);
	if (!ret)
		return -1;

	fill(ret, accu);

	zlog_info(zloghub::oneline, "sql query success: %s", statment.c_str());

	mysql_free_result(ret);
	return 0;
}

int stupidb_impl::query(const std::string& statment, row_ret_pt accu) const
{
	MYSQL_RES* ret = query(statment);
	if (!ret)
		return -1;

	fill(ret, accu);

	zlog_info(zloghub::oneline, "sql query success: %s", statment.c_str());

	mysql_free_result(ret);
	return 0;
}

MYSQL_RES* stupidb_impl::query(const std::string& statment) const 
{
	if (!is_open())
		return nullptr;

	if (mysql_real_query(_db, statment.c_str(), statment.length()))
	{
		zlog_error(zloghub::oneline, "sql query failed: %s", mysql_error(_db));
		return nullptr;
	}

	MYSQL_RES* ret = mysql_store_result(_db);
	if (!ret)
	{
		zlog_error(zloghub::oneline, "sql query failed: %s", mysql_error(_db));
		return nullptr;
	}

	return ret;
}

MYSQL* stupidb_impl::open() const
{
	MYSQL* temp = mysql_init(nullptr);
	if (nullptr == temp)
	{
		zlog_error(zloghub::oneline, "failed to init mysql handle");
		return nullptr;
	}

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
		zlog_error(zloghub::oneline, "failed to connect to database: %s", mysql_error(temp));
		return nullptr;
	}

	return temp;
}

void stupidb_impl::fill(MYSQL_RES* result, column_ret_pt accu) const 
{
	unsigned int num_fields = mysql_num_fields(result);

	std::map<std::string, std::string> temp;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		temp.clear();
		for(unsigned int i = 0; i < num_fields; i++)
		{
			temp[mysql_fetch_field_direct(result, i)->name] = row[i];
		}

		accu->push_back(temp);
	}
}

void stupidb_impl::fill(MYSQL_RES* result, row_ret_pt accu) const 
{
	unsigned int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		for(unsigned int i = 0; i < num_fields; i++)
		{
			(*accu)[mysql_fetch_field_direct(result, i)->name].push_back(row[i]);
		}
	}
}

}
