#include <mutex>
#include <unistd.h>

#include <thread>
#include <chrono>

#include "src/dbargs.h"
#include "src/stupidbimpl.h"
#include "src/stupidb.h"
#include "src/zloghub.h"
#include "src/stupidbalias.h"

bool static is_run = true;
static std::mutex fuck;

void rolling()
{
	auto impl = stupid::stupidb::get_instance("../dbconfig.json"); 
		
	while (is_run)
	{
		std::scoped_lock<std::mutex> guard(fuck);

		stupid::row_ret_t rret;
		if (impl->query(std::string("select * from tmall_product;"), &rret))
			return;

		usleep(1000);

		zlog_info(stupid::zloghub::oneline, "query success");

		stupid::column_ret_t cret;
		if (impl->query(std::string("select * from tmall_product;"), &cret))
			return;

		zlog_info(stupid::zloghub::oneline, "query success");
	}
}

int main(int argc, char* argv[])
{
	stupid::zloghub::init("../zlog.conf");

	std::thread t1(rolling);
	std::thread t2(rolling);
	std::thread t3(rolling);
	t1.detach();
	t2.detach();
	t3.detach();

	while (true)
		sleep(10);

	stupid::zloghub::fini();
	is_run = false;
	return 0;
}
