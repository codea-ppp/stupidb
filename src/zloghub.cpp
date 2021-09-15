#include "src/zloghub.h"

namespace stupid
{

bool zloghub::init(const char* config_path)
{ 
	if (zlog_init(config_path))
		return false;

	oneline = zlog_get_category("stupidb_oneline");
	cr = zlog_get_category("stupidb_cr");

	return true;
}

void zloghub::fini()
{ 
	zlog_fini(); 
}

}
