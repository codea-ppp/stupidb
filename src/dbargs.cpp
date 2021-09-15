#include "src/dbargs.h"

#include <unistd.h>

#include <string>

#include <mysql/mysql.h>
#include <json/json.h>

namespace stupid
{

static inline void allocate_string(char** notalloc, const char* src)
{
	size_t len = strlen(src);
	*notalloc = (char*)malloc(len * sizeof(char) + 1);
	memcpy(*notalloc, src, len);
	(*notalloc)[len] = '\0';
}

static inline void free_string(char* allocated)
{
	if (allocated) free(allocated);
}

static bool loading_config_buffer(Json::Value* root, const char* config_path)
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
	if (0 == len)
	{
		perror("fread");
		fclose(fp);
		return false;
	}

	std::string err;
	if (!reader->parse(buffer, buffer + len, root, &err))
	{
		printf("json config parse failed: %s\n", err.c_str());

		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

static bool loading_config(Json::Value& root, dbargs* that)
{
	bool is_unixsock = true;
	bool is_network = true;

	if (!root.isMember("dbm_db") || !root["dbm_db"].isString())
		return false;
	if (!root.isMember("dbm_user") || !root["dbm_user"].isString())
		return false;
	if (!root.isMember("dbm_password") || !root["dbm_password"].isString())
		return false;
	if (!root.isMember("dbm_max_connection") || !root["dbm_max_connection"].isInt())
		return false;

	if (!root.isMember("dbm_host") || !root["dbm_host"].isString())
		is_network = false;
	if (!root.isMember("dbm_port") || !root["dbm_port"].isUInt())
		is_network = false;
	if (!root.isMember("dbm_sock") || !root["dbm_sock"].isString())
		is_unixsock = false;

	if (!is_network || !is_unixsock)
		return false;

	if (is_network)
	{
		allocate_string(&that->_host, root["dbm_host"].asString().c_str());
		that->_port = root["dbm_port"].asInt();
	}

	if (is_unixsock)
	{
		allocate_string(&that->_sock, root["dbm_sock"].asString().c_str());
	}

	allocate_string(&that->_password, root["dbm_password"].asString().c_str());
	allocate_string(&that->_user, root["dbm_user"].asString().c_str());
	allocate_string(&that->_db, root["dbm_db"].asString().c_str());
	that->_max_connection = root["dbm_max_connection"].asUInt();

	return true;
}

dbargs::dbargs(const char* config_path) : 
	_host(NULL),
	_user(NULL),
	_password(NULL),
	_db(NULL),
	_port(0),
	_sock(NULL)
{
	Json::Value root;
	if (!loading_config_buffer(&root, config_path))
		return;

	if (!loading_config(root, this))
		return;
}

dbargs::dbargs(const char* host, 
		   	   const char* user, 
			   const char* password, 
			   const char* db,
			   unsigned int port, 
			   const char* sock, 
			   const int max_connection)
{
	host ? allocate_string(&_host, host) : [this](){ _host = NULL; }();
	user ? allocate_string(&_user, user) : [this](){ _user = NULL; }();
	password ? allocate_string(&_password, password) : [this](){ _password = NULL; }();
	db ? allocate_string(&_db, db) : [this](){ _db = NULL; }();
	_port = port;
	sock ? allocate_string(&_sock, sock) : [this](){ _sock = NULL; }();
	_max_connection = max_connection;
}

dbargs::~dbargs()
{
	free_string(_host);
	free_string(_user);
	free_string(_password);
	free_string(_db);
	free_string(_sock);
}

}
