#ifndef _STUPIDB_SRC_ZLOGHUB_H_
#define _STUPIDB_SRC_ZLOGHUB_H_

#include <string>
#include <map>

#include <zlog.h>

namespace stupid
{

class zloghub
{
public:
	static bool init(const char* config_path);
	static void fini();

	inline static zlog_category_t* oneline;
	inline static zlog_category_t* cr;

private:
	const zloghub& operator=(const zloghub&) = delete;
	zloghub(const zloghub&) = delete;
	zloghub() = delete;
};


}

#endif 	// _STUPIDB_SRC_ZLOGHUB_H_
