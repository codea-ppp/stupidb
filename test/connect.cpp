#include "src/dbargs.h"
#include "src/stupidbimpl.h"
#include "src/stupidb.h"
#include "src/zloghub.h"
#include "src/stupidbalias.h"

int main(int argc, char* argv[])
{
	stupid::zloghub::init("../zlog.conf");

	stupid::stupidb_impl impl("../dbconfig.json");
	
	stupid::row_ret_t rret;
	if (impl.query("select * from tmall_product;", &rret))
		return -1;

	for (auto i : rret)
	{
		printf("%s: ", i.first.c_str());

		for (auto j : i.second)
		{
			printf("%s ", j.c_str());
		}

		printf("\n");
	}

	stupid::column_ret_t cret;
	if (impl.query("select * from tmall_product;", &cret))
		return -1;

	for (auto i : cret)
	{
		for (auto j : i)
		{
			printf("%s ", j.second.c_str());
		}

		printf("\n");
	}

	stupid::zloghub::fini();
	return 0;
}
