#pragma once
#include <map>
#include <string>
#include <fstream>
#include <cpprest/filestream.h>

namespace user_map
{
	struct permission_struct
	{
		bool banned : 1;

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
		std::string username,password;
		permission_union permissions_union;
	};

	class user_map
	{
		std::map<std::string, user> user_map_;		//username, user
	public:
		user_map()
		{
			
		}
		user_map(user_map& other)
		{
			user_map_ = other.user_map_;
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
		pplx::task<size_t> async_load_users_from_file(std::string path)
		{
			//Concurrency::streams::file_stream<std::string> user_file;
			//user_file.
			return pplx::create_task([this, path]()
			{
				return load_users_from_file(path);
			});
		}
		size_t save_users_to_file(std::string path)
		{
			std::ofstream user_file(path);
			size_t size{ 0 };
			for (auto& user : user_map_)
			{
				user_file << user.first << " " << user.second.password << " " << user.second.permissions_union.permissions_short << std::endl;
				size++;
			}
			return size;
		}
		pplx::task<size_t> async_save_users_to_file(std::string path)
		{
			return pplx::create_task([this, path]()
			{
				return save_users_to_file(path);
			});
		}
		void add_user(std::string& username, std::string& password, permission_union& permissions)
		{
			user_map_.insert({ username,{username,password,permissions} });
		}
		void add_user(std::string username, std::string password, permission_union permissions)
		{
			user_map_.insert({ username,{username,password,permissions} });
		}
		void add_user(std::string username,user& user)
		{
			user_map_.insert({ username,{username,user.password,user.permissions_union} });
		}
		user& get_user(std::string username)
		{
			return user_map_.at(username);
		}
		std::map<std::string, user>& get_user_map()
		{
			return user_map_;
		}

		void operator=(const user_map& other)
		{
			user_map_ = other.user_map_;
		}
	};
	namespace generate
	{
		inline permission_union admin()
		{
			return permission_union{
				{
					false,
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
				false,
				false
				}
			};
		}
	}
}




