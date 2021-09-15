#ifndef _STUPIDB_SRC_STUPIDBALIAS_H_
#define _STUPIDB_SRC_STUPIDBALIAS_H_

#include <vector>
#include <string>
#include <map>

namespace stupid
{

using column_ret_pt = std::vector<std::map<std::string, std::string>>*;
using column_ret_t = std::vector<std::map<std::string, std::string>>;
using row_ret_pt = std::map<std::string, std::vector<std::string>>*;
using row_ret_t = std::map<std::string, std::vector<std::string>>;

}

#endif 	// _STUPIDB_SRC_STUPIDBALIAS_H_
