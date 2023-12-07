#include <iostream>
#include <cpprest/details/http_server.h>
#include <cpprest/http_listener.h>
#include "include/usermap.h"
#include "include/RESTapi.h"


const std::wstring path_users_file = L"users.txt";
const std::wstring uri_string = L"http://localhost:7475";

int main()
{
	RESTServer rest_server(uri_string,path_users_file);
}