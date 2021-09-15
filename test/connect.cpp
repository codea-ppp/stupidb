#include "src/dbargs.h"
#include "src/stupidbimpl.h"
#include "src/stupidb.h"

int main(int argc, char* argv[])
{
	stupid::stupidb_impl impl("../dbconfig.json");
	impl.query("select * from tmall_product;");

	return 0;
}
