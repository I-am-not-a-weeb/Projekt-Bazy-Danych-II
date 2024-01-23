#include <iostream>
#include <cpprest/details/http_server.h>
#include <cpprest/http_listener.h>
#include "include/usermap.h"
#include "include/RESTapi.h"


const std::wstring path_users_file = L"users.txt";
const std::wstring uri_string = L"http://localhost:7475";
web::http::client::http_client client(L"http://localhost:7474/db/neo4j/tx/commit");

int main()
{
	RESTServer rest_server(uri_string,path_users_file,client);
	rest_server.start();
	while (true)
	{}
}