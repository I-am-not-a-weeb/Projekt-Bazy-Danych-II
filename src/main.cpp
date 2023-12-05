#include <iostream>
#include <cpprest/details/http_server.h>
#include <cpprest/http_listener.h>
#include "include/usermap.h"



const std::string path_users_file = "users.txt";

int main()
{
	user_map::user_map users;

	users.load_users_from_file(path_users_file);
	

	//web::http::experimental::listener::http_listener listener(L"http://localhost:7475");
}