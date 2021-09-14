#ifndef _STUPIDB_SRC_STUPIDB_H_
#define _STUPIDB_SRC_STUPIDB_H_

#include <mysql.h>

#include <string>

#include "src/dbdef.h"

namespace stupid
{

class stupidb 
{
public:
	stupidb(const dbargs& args);

	~stupidb();

private:
	bool _is_open;

	dbargs _args;
 
	MYSQL _handle;
};

}

#endif 	// _STUPIDB_SRC_STUPIDB_H_
