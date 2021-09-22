#include "src/stupidb.h"

#include <exception>
#include <stdexcept>
#include <memory>
#include <mutex>
#include <new>

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

int stupidb::query(const std::string& statment, column_ret_pt accu) const
{
	return impls[++impl_index % impl_length].query(statment, accu);
}

int stupidb::query(const std::string& statment, row_ret_pt accu) const
{
	return impls[++impl_index % impl_length].query(statment, accu);
}

stupidb::stupidb(const dbargs args)
{
	impls = (stupidb_impl*)malloc(sizeof(stupidb_impl) * args._max_connection);
	for (size_t i = 0; i < args._max_connection; ++i)
	{
		new(impls + i) stupidb_impl(args);
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
