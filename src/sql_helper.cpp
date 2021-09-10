#include "sql_helper.h"
#include "sql_helper_impl.h"

using namespace sql_helper;

bool mariadb_helper::connect_to(
		const char* ip,			const uint16_t port, 
		const char* username,	const char* password, 
		const char* db_to_use,	const char* db_sock) 
{
	return mariadb_helper_impl::get_instance()->connect_to(ip, port, username, password, db_to_use, db_sock);
}

bool mariadb_helper::search_user_competence(const char* token_md5, std::vector<int>& inerface_can_access)
{
	return mariadb_helper_impl::get_instance()->search_user_competence(token_md5, inerface_can_access);
}

bool mariadb_helper::search_user_password(const char* username, unsigned char password[33])
{
	return mariadb_helper_impl::get_instance()->search_user_password(username, password);
}

bool mariadb_helper::insert_user_token(const char* username, const char* token)
{
	return mariadb_helper_impl::get_instance()->insert_user_token(username, token);
}

bool mariadb_helper::delete_user_token(const char* token)
{
	return mariadb_helper_impl::get_instance()->delete_user_token(token);
}

mariadb_helper::mariadb_helper()
{
}

mariadb_helper::~mariadb_helper()
{
}
