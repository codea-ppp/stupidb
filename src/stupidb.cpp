#include "src/stupidb.h"

#include <exception>
#include <stdexcept>
#include <memory>
#include <atomic>
#include <mutex>
#include <new>
#include <string.h>

#include "src/stupidbimpl.h"
#include "src/dbargs.h"
#include "src/zloghub.h"

namespace stupid
{

std::shared_ptr<stupidb> stupidb::get_instance(const char* config_path)
{
	static std::mutex lk;
	std::scoped_lock<std::mutex> guard(lk);

	dbargs args(config_path);
	if (!args._is_right)
		throw std::runtime_error("bad config file");

	std::string key(args._user);
	key.append(args._password);
	key.append(args._db);

	if (registe.count(key))
		return registe[key];

	registe[key] = std::shared_ptr<stupidb>(new stupidb(args));
	return registe[key];
}

std::shared_ptr<stupidb> stupidb::get_instance(const char* host, 
											   const char* user,
											   const char* password,
											   const char* db,
											   unsigned int port,
											   const char* sock, 
											   const unsigned int max_connection)
{
	static std::mutex lk;
	std::scoped_lock<std::mutex> guard(lk);

	dbargs args(host, user, password, db, port, sock, max_connection);
	if (!args._is_right)
		throw std::runtime_error("bad config file");

	std::string key(args._user);
	key.append(args._password);
	key.append(args._db);

	if (registe.count(key))
		return registe[key];

	registe[key] = std::shared_ptr<stupidb>(new stupidb(args));
	return registe[key];
}

static thread_local int8_t except = 0; // always 0 in actual

int stupidb::query(const std::string& statment, column_ret_pt accu) const
{
	unsigned int index = impl_index.fetch_add(1, std::memory_order_release) % impl_length;
	while (!busy[index].compare_exchange_weak(except, 1, std::memory_order_release, std::memory_order_relaxed));

	LOG_INFO(LOG_CATEGORY_ONE, "%d in use", index);

	int ret = impls[index].query(statment, accu);
	busy[index].store(0, std::memory_order_relaxed);

	LOG_INFO(LOG_CATEGORY_ONE, "%d out of use", index);

	return ret;
}

int stupidb::query(const std::string& statment, row_ret_pt accu) const
{
	unsigned int index = impl_index.fetch_add(1, std::memory_order_release) % impl_length;
	while (!busy[index].compare_exchange_weak(except, 1, std::memory_order_release, std::memory_order_relaxed));

	LOG_INFO(LOG_CATEGORY_ONE, "%d in use", index);

	int ret = impls[index].query(statment, accu);
	busy[index].store(0, std::memory_order_relaxed);

	LOG_INFO(LOG_CATEGORY_ONE, "%d out of use", index);

	return ret;
}

stupidb::stupidb(const dbargs args)
{
	impls = (stupidb_impl*)malloc(sizeof(stupidb_impl) * args._max_connection);
	for (size_t i = 0; i < args._max_connection; ++i)
	{
		new(impls + i) stupidb_impl(args);
	}

	busy = new std::atomic_int8_t[args._max_connection];
	for (size_t i = 0; i < args._max_connection; ++i)
	{
		busy[i].store(0);
	}

	impl_length = args._max_connection;
	impl_index = 0;
}

stupidb::~stupidb()
{
	for (size_t i = 0; i < impl_length; ++i)
	{
		impls[i].~stupidb_impl();
	}

	free(impls);
}

}
