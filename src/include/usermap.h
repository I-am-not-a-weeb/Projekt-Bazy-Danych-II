#pragma once
#include <map>
#include <string>
#include <fstream>

namespace user_map
{
	struct permission_struct
	{
		bool can_modify_admins : 1;
		bool can_modify_users : 1;
		bool can_modify_memes : 1;
		bool can_modify_comments : 1;

		bool can_add_memes : 1;
		bool can_add_comments : 1;

		bool can_modify_self : 1;
		bool can_modify_own_memes : 1;
		bool can_modify_own_comments : 1;

		bool can_report : 1;
	};
	union permission_union
	{
		permission_struct permissions;
		unsigned short permissions_short;

		permission_union(const unsigned short& permissions_short) : permissions_short(permissions_short) {}
		permission_union(permission_struct permissions_struct) : permissions(permissions_struct) {}
	};

	struct user
	{
		std::string username, password;
		permission_union permissions_union;
	};

	class user_map
	{
		std::map<std::string, user> user_map_;
	public:
		user_map()
		{
			
		}
		size_t load_users_from_file(std::string path)
		{
			std::ifstream user_file(path);
			std::string username, password;
			unsigned short permissions;
			while (user_file >> username >> password >> permissions)
				user_map_.insert({ username,{username,password,permission_union{permissions}} });

			return user_map_.size();
		}

		size_t save_users_to_file(std::string path)
		{
			std::ofstream user_file(path);
			size_t size{ 0 };
			for (auto& user : user_map_)
			{
				user_file << user.second.username << " " << user.second.password << " " << user.second.permissions_union.permissions_short << std::endl;
				size++;
			}
			return size;
		}

		void add_user(std::string username, std::string password, permission_union permissions)
		{
			user_map_.insert({ username,{username,password,permissions} });
		}
		user& get_user(std::string username)
		{
			return user_map_.at(username);
		}
	};
	namespace generate
	{
		inline permission_union admin()
		{
			return permission_union{
				{
					true,
					true,
					true,
					true,
					true,
					true,
					true,
					true,
					true,
					true
				}
			};
		}

		inline permission_union user()
		{
			return permission_union
			{
				{
					false,
					false,
					false,
					false,
					true,
					true,
					true,
					true,
					true,
					true
				}
			};
		}

		inline permission_union quest()
		{
			return permission_union
			{
				{
				false,
				false,
				false,
				false,
				false,
				false,
				false,
				false,
				false,
				false
				}
			};
		}
	}
}




