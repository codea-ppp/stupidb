#ifndef _STUPIDB_SRC_ZLOGHUB_H_
#define _STUPIDB_SRC_ZLOGHUB_H_

#include <string>
#include <map>

#include <zlog.h>

namespace stupid
{

#ifdef ZLOG_ENABLE

// this is depend on zlog config file zlog.conf
// close log on CMakeLists.txt 
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

#define LOG_INIT(config_path) stupid::zloghub::init(config_path)
#define LOG_FINI() stupid::zloghub::fini()

#define LOG_CATEGORY_ONE stupid::zloghub::oneline
#define LOG_CATEGORY_CR  stupid::zloghub::cr

#define LOG_DEBUG(category,  fmt, args...) zlog_debug(category,  fmt, ##args)
#define LOG_INFO(category,   fmt, args...) zlog_info(category,   fmt, ##args)
#define LOG_NOTICE(category, fmt, args...) zlog_notice(category, fmt, ##args)
#define LOG_WARN(category,   fmt, args...) zlog_warn(category,   fmt, ##args)
#define LOG_ERROR(category,  fmt, args...) zlog_error(category,  fmt, ##args)
#define LOG_FATAL(category,  fmt, args...) zlog_fatal(category,  fmt, ##args)

#else

#define LOG_INIT(config_path)
#define LOG_FINI()

#define LOG_CATEGORY_ONE
#define LOG_CATEGORY_CR

#define LOG_DEBUG(category,  fmt, args...)
#define LOG_INFO(category,   fmt, args...)
#define LOG_NOTICE(category, fmt, args...)
#define LOG_WARN(category,   fmt, args...)
#define LOG_ERROR(category,  fmt, args...)
#define LOG_FATAL(category,  fmt, args...)

#endif // ZLOG_ENABLE

}

#endif 	// _STUPIDB_SRC_ZLOGHUB_H_
