#ifndef SQL_HELPER_IMPL_H_
#define SQL_HELPER_IMPL_H_

#include <stdint.h>

#include <mysql.h>
#include <vector>

namespace sql_helper
{
	class mariadb_helper_impl
	{
	public:
		static mariadb_helper_impl* get_instance();

		bool connect_to(
			const char* ip,			const uint16_t port, 
			const char* username,	const char* password,
			const char* db_to_use,	const char* db_sock
		);

		bool search_user_competence(const char* token_md5, std::vector<int>& inerface_can_access);
		bool search_user_password(const char* username, unsigned char password[32]);
		bool insert_user_token(const char* username, const char* token);
		bool delete_user_token(const char* token);

	private:
		bool query(const char* sql_statement, MYSQL_BIND* bind, size_t size);

		void show_mysql_error(MYSQL *mysql);
		void show_stmt_error(MYSQL_STMT *stmt);

	private:
		mariadb_helper_impl(const mariadb_helper_impl&)						= delete ;
		mariadb_helper_impl(const mariadb_helper_impl&&)					= delete ;
		const mariadb_helper_impl&	operator=(const mariadb_helper_impl&)	= delete ;
		const mariadb_helper_impl&&	operator=(const mariadb_helper_impl&&)	= delete ;

		mariadb_helper_impl();
		~mariadb_helper_impl();

	private:
		MYSQL*			mysql;
		MYSQL_STMT*		stmt;
	};
}


#endif 
