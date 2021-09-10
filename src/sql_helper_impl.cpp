#include <stdio.h>
#include <string.h>

#include <base64.h>
#include <butil/logging.h>
#include <string>

#include "sql_helper_impl.h"

using namespace sql_helper;

mariadb_helper_impl* mariadb_helper_impl::get_instance()
{
	static mariadb_helper_impl instance;
	return &instance;
}

bool mariadb_helper_impl::connect_to(
	const char* ip,			const uint16_t port, 
	const char* username,	const char* password,
	const char* db_to_use,	const char* db_sock
) { 
	if (!mysql_real_connect(mysql, ip, username, password, db_to_use, port, db_sock, 0))
	{
		show_mysql_error(mysql);
		return false;
	}

	return true;
}

bool mariadb_helper_impl::search_user_competence(const char* token_md5, std::vector<int>& inerface_can_access)
{
	std::string query_statement("select competence.inerface_id from competence where competence.user_id = (select user.user_id from user where user.current_token = '");
	query_statement.append(token_md5).append("')");

	LOG(INFO) << query_statement;

	if (mysql_query(mysql, query_statement.c_str()))
		show_mysql_error(mysql);

	MYSQL_RES* result = mysql_store_result(mysql);
	if (!result)
	{
		show_mysql_error(mysql);
		return false;
	}

	unsigned long filed_number = mysql_num_fields(result);
	if (1 != filed_number)
	{
		mysql_free_result(result);

		show_mysql_error(mysql);
		return false;
	}

	MYSQL_ROW row;

	try 
	{
		while ((row = mysql_fetch_row(result)))
			inerface_can_access.push_back(std::stoi(row[0]));
	}
	catch (std::invalid_argument)
	{
		LOG(ERROR) << "Bad inerface number " << row[0] << "database type error !";
		mysql_free_result(result);
		return false;
	}
	catch (std::out_of_range)
	{
		LOG(ERROR) << "Bad inerface number " << row[0] << "over int range ?";
		mysql_free_result(result);
		return false;
	}

	mysql_free_result(result);
	return true;
}

bool mariadb_helper_impl::search_user_password(const char* username, unsigned char password[32])
{
	size_t data_length	= strlen(username);
	char* base64_string = new char(Base64encode_len(data_length));

	Base64encode(base64_string, username, data_length);

	std::string query_statement("select password from user where user_name = '");
	query_statement.append(base64_string).append("'");

	LOG(INFO) << query_statement;

	if (mysql_query(mysql, query_statement.c_str()))
		show_mysql_error(mysql);

	MYSQL_RES* result = mysql_store_result(mysql);
	if (!result)
	{
		show_mysql_error(mysql);
		delete base64_string;
		return false;
	}

	if (1 != mysql_num_rows(result))
	{
		mysql_free_result(result);
		delete base64_string;

		return false;
	}

	MYSQL_ROW row = mysql_fetch_row(result);
	unsigned long* length = mysql_fetch_lengths(result); 

	if (32 != length[0])
	{
		LOG(ERROR) << "Invailed password " << row[0];

		mysql_free_result(result);
		delete base64_string;

		return false;
	}

	memcpy(password, row[0], 32);
	return true;
}

bool mariadb_helper_impl::insert_user_token(const char* username, const char* token)
{
	size_t data_length	= strlen(username);
	char* base64_string = new char(Base64encode_len(data_length));

	Base64encode(base64_string, username, data_length);

	std::string query_statement("update user set current_token = '");
	query_statement.append(token).append("', is_online = 1 where user_name = '") .append(base64_string).append("'");

	LOG(INFO) << query_statement;

	if (mysql_query(mysql, query_statement.c_str()))
	{
		show_mysql_error(mysql);

		delete base64_string;
		return false;
	}

	delete base64_string;
	return true;
}

bool mariadb_helper_impl::delete_user_token(const char* token)
{
	std::string query_statement("select user_id from user where current_token = '");
	query_statement.append(token).append("' limit 1");


	query_statement.clear();
	query_statement = "update user set current_token = '', is_online = 0 where current_token = '";
	query_statement.append(token).append("'");

	LOG(INFO) << query_statement;

	if (mysql_query(mysql, query_statement.c_str()))
	{
		show_mysql_error(mysql);
		return false;
	}

	return true;
}

mariadb_helper_impl::mariadb_helper_impl()
{
	mysql	= mysql_init(nullptr);
	stmt	= mysql_stmt_init(mysql);
}

mariadb_helper_impl::~mariadb_helper_impl()
{
	mysql_stmt_close(stmt);
	mysql_close(mysql);
}

bool mariadb_helper_impl::query(const char* sql_statement, MYSQL_BIND* bind, size_t size)
{
	if (mysql_stmt_prepare(stmt, sql_statement, -1))
	{
		show_stmt_error(stmt);
		return false;
	}

	mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &size);
	mysql_stmt_bind_param(stmt, bind);

	if (mysql_stmt_execute(stmt))
	{
		show_stmt_error(stmt);
		return false;
	}

	return true;
}

void mariadb_helper_impl::show_mysql_error(MYSQL *mysql) 
{ 
	LOG(FATAL) << "Error " << mysql_errno(mysql) << '[' << mysql_sqlstate(mysql) << ']' << '"' << mysql_error(mysql) << '"';
}

void mariadb_helper_impl::show_stmt_error(MYSQL_STMT *stmt)
{ 
	LOG(FATAL) << "Error " << mysql_stmt_errno(stmt) << '[' << mysql_stmt_sqlstate(stmt) << ']' << '"' << mysql_stmt_error(stmt) << '"';
}
