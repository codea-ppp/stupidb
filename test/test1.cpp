#include <unistd.h>

#include <stdexcept>
#include <thread>
#include <chrono>
#include <mutex>

#include "src/stupidb.h"

bool static is_run = true;

void rolling()
{
	try
	{
		auto impl = stupid::stupidb::get_instance("../dbconfig.json"); 

		while (is_run)
		{
			stupid::row_ret_t rret;
			if (impl->query(std::string("select * from tmall_product;"), &rret))
				return;

			usleep(100);

			stupid::column_ret_t cret;
			if (impl->query(std::string("select * from tmall_product;"), &cret))
				return;
		}
	}
	catch (std::runtime_error err)
	{
		LOG_FATAL(LOG_CATEGORY_ONE, "%s", err.what());
		return;
	}
}

int main(int argc, char* argv[])
{
	LOG_INIT("../zlog.conf");

	std::thread t1(rolling);
	std::thread t2(rolling);
	std::thread t3(rolling);
	std::thread t4(rolling);
	std::thread t5(rolling);
	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();
	t5.detach();

	while (true)
		sleep(20);

	is_run = false;

	LOG_FINI();

	return 0;
}
