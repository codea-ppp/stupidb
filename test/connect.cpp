#include "src/dbargs.h"
#include "src/stupidb.h"

int main(int argc, char* argv[])
{
	stupid::dbargs args("../dbconfig.json");
	stupid::stupidb db(args);
	return 0;
}
