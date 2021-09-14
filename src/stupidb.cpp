#include "src/stupidb.h"

#include <mysql/mysql.h>

#include <string>

namespace stupid
{

stupidb::stupidb(const dbargs& args) : _is_open(false), _args(args), _db(nullptr)
{
	_db = args.open();
	if (_db != nullptr)
		_is_open = true;

	mysql_query(_db, "show databases;");
	MYSQL_RES* ret = mysql_store_result(_db);
	if (ret)
	{
		MYSQL_ROW row;
		unsigned int num_fields;
		unsigned int i;

		num_fields = mysql_num_fields(ret);
		while ((row = mysql_fetch_row(ret)))
		{
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(ret);
			for(i = 0; i < num_fields; i++)
			{
				printf("[%.*s] ", (int) lengths[i],
				row[i] ? row[i] : "NULL");
			}

			printf("\n");
		}
	}

	mysql_free_result(ret);
}	

stupidb::~stupidb()
{
	if (_is_open)
	{
		mysql_close(_db);
	}
}

}
