#ifndef SQL_HEAPER_H_
#define SQL_HEAPER_H_

#include <vector>
#include <memory>

namespace sql_helper 
{
	class mariadb_helper_impl;

	class mariadb_helper
	{
	public:
		static bool connect_to(
			const char* ip,			const uint16_t port, 
			const char* username,	const char* password,
			const char* db_to_use,	const char* db_sock
		);

		static bool search_user_competence(const char* token_md5, std::vector<int>& inerface_can_access);
		static bool search_user_password(const char* username, unsigned char password[33]);
		static bool insert_user_token(const char* username, const char* token);
		static bool delete_user_token(const char* token);

	private:
		mariadb_helper(const mariadb_helper&)						= delete ;
		mariadb_helper(const mariadb_helper&&)						= delete ;
		const mariadb_helper&	operator=(const mariadb_helper&)	= delete ;
		const mariadb_helper&&	operator=(const mariadb_helper&&)	= delete ;

		mariadb_helper();
		~mariadb_helper();
	};
}

#endif 
