#include "src/dbargs.h"

#include <unistd.h>

#include <string>
#include <fstream>

#include <json/json.h>

namespace stupid
{

bool dbargs::loading_config_buffer(Json::Value* root, const char* config_path)
{
	Json::CharReader* reader = Json::CharReaderBuilder().newCharReader();
	FILE* fp = fopen(config_path, "r");
	if (nullptr == fp)
	{
		perror("fopen");
		return false;
	}

	char buffer[1024] = { 0 };
	size_t len = fread(buffer, 1, sizeof(buffer), fp);
	if (-1 == len)
	{
		perror("fread");
		fclose(fp);
		return false;
	}

	std::string err;
	if (!reader->parse(buffer, buffer + len - 1, root, &err))
	{
		printf("json config parse failed: %s\n", err.c_str());

		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

bool dbargs::loading_config(Json::Value& root)
{
	if (!root.isMember("dbm_db") || !root["dbm_db"].isString())
		return false;
	if (!root.isMember("dbm_host") || !root["dbm_host"].isString())
		return false;
	if (!root.isMember("dbm_port") || !root["dbm_port"].isInt())
		return false;
	if (!root.isMember("dbm_user") || !root["dbm_user"].isString())
		return false;
	if (!root.isMember("dbm_password") || !root["dbm_password"].isString())
		return false;
	if (!root.isMember("dbm_sock") || !root["dbm_sock"].isString())
		return false;

	_host = root["dbm_host"].asString();
	_user = root["dbm_user"].asString();
	_password = root["dbm_password"].asString();
	_db = root["dbm_db"].asString();
	_port = root["dbm_port"].asInt();
	_sock = root["dbm_sock"].asString();

	return true;
}

dbargs::dbargs(const std::string& config_path) : _is_load(false)
{
	Json::Value root;
	if (!loading_config_buffer(&root, config_path.c_str()))
		return;

	if (!loading_config(root))
		return;

	_is_load = true;
}

MYSQL* dbargs::open() const
{
	if (!_is_load)
		return nullptr;

	MYSQL* temp = mysql_init(nullptr);
	if (nullptr == temp)
		return nullptr;

	mysql_real_connect(temp, 
					   _host.c_str(), 
					   _user.c_str(), 
					   _password.c_str(), 
					   _db.c_str(), 
					   _port, 
					   _sock.c_str(), 
					   0);
	if (nullptr == temp)
		return nullptr;

	return temp;
}

}
