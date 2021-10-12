#ifndef _STUPIDB_SRC_STUPIDB_H_
#define _STUPIDB_SRC_STUPIDB_H_

#include <memory>
#include <atomic>
#include <vector>

#include "src/stupidbalias.h"
#include "src/zloghub.h"

namespace stupid
{

class stupidb_impl;
class dbargs;

class stupidb 
{
public:
	static std::shared_ptr<stupidb> get_instance(const char* config_key);
	static std::shared_ptr<stupidb> get_instance(const char* host, 
												 const char* user,
												 const char* password,
												 const char* db,
												 unsigned int port,
												 const char* sock, 
												 const unsigned int max_connection);

	int query(const std::string& statment, column_ret_pt accu) const;
	int query(const std::string& statment, row_ret_pt accu) const;

	~stupidb();

	stupidb(const dbargs args);
	stupidb(const stupidb&) = delete;
	const stupidb& operator=(const stupidb&) = delete;

private:
	stupidb_impl* impls;
	std::atomic_int8_t* busy;
	size_t impl_length;
	mutable std::atomic_uint32_t impl_index;

	inline static std::map<std::string, std::shared_ptr<stupidb>> registe;
};

}

#endif 	// _STUPIDB_SRC_STUPIDB_H_
