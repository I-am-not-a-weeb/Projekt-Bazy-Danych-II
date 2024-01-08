
//path routing method as per https://github.com/microsoft/cpprestsdk/issues/186#issuecomment-230072639


#pragma once
#include <map>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include "usermap.h"

#include <boost/algorithm/string.hpp>
#include <boost/random.hpp>

const std::wstring neo4j_uri{ L"http://localhost:7474/db/neo4j/tx/commit" };
const std::wstring auth_64{ L"bmVvNGo6dGVzdDEyMzQ=" };

bool isNumber(utility::string_t& str);

enum class routes : unsigned short
{
	empty = 0,
	garbage = 999,
	signup = 1,
	login = 2,


	account = 20,
	account_username = 21,
	account_username_preferences = 22,
	account_username_givelike = 23,
	account_username_comments = 24,


	meme = 100,
	meme_id = 101,
	meme_id_comments = 102,
	meme_id_givelike = 103,


	ranking = 200,
	ranking_users = 201,
	ranking_memes = 202,
	ranking_comments = 203,

	comment = 300,
	comment_id = 301,
	comment_id_givelike = 302
};

namespace routing
{
	inline routes path_to_route_parser(std::vector<utility::string_t>&path)
	{
		if (path.empty())														//	/
		{
			return routes::empty;
		}
		else
		{
			if (!path.empty() && path[0] == L"account")							//	/account/?
			{
				if (path.size() == 1)												//	/account
				{
					return routes::account;
				}
				else																//	/account/:username/?
				{
					if (path.size() == 2 && true)									//	/account/:username
					{
						return routes::account_username;
					}
					else															//	/account/:username/?
					{
						if (path.size() == 3 && path[2] == L"comments")				//	/account/:username/comments
						{
							return routes::account_username_comments;
						}
						else if (path.size() == 3 && path[2] == L"preferences")		//	/account/:username/preferences
						{
							return routes::account_username_preferences;
						}
						else if (path.size() == 3 && path[2] == L"givelike")		//	/account/:username/give_like
						{
							return routes::account_username_givelike;
						}
						else														//	/account/:username/[garbage]
						{
							return routes::garbage;
						}
					}
				}
			}
			else if (!path.empty() && path[0] == L"meme")						//	/account/meme/?
			{
				if (path.size() == 1)												//	/account/meme
				{
					return routes::meme;
				}
				else																//	/account/meme/?
				{
					if (path.size() == 2/* && isNumber(path[1])*/)						//	/account/meme/:id
					{
						return routes::meme_id;
					}
					else															//	/account/meme/[garbage]
					{
						if (path.size() == 3 && path[2] == L"comments")				//	/account/meme/:id/comments
						{
							return routes::meme_id_comments;
						}
						else if (path.size() == 3 && path[2] == L"givelike")		//	/account/meme/:id/givelike
						{
							return routes::meme_id_givelike;
						}
						else														//	/account/meme/:id/[garbage]
						{
							return routes::garbage;
						}
					}
				}
			}
			else if (!path.empty() && path[0] == L"comment")							//	/comments/?
			{
				if (path.size() == 1)												//	/comments
				{
					return routes::comment;
				}
				else																//	/comments/?
				{
					if (path.size() == 2 /*&& isNumber(path[1])*/)						//	/comments/:id
					{
						return routes::comment_id;
					}
					else															//	/comments/[garbage]
					{
						if (path.size() == 3 && path[2] == L"givelike")				//	/comments/:id/givelike
						{
							return routes::comment_id_givelike;
						}
						else														//	/comments/:id/[garbage]
						{
							return routes::garbage;
						}
					}
				}
			}
			else if (!path.empty() && path[0] == L"ranking")
			{
				if (path.size() == 1)												//	/ranking
				{
					return routes::ranking;
				}
				else																//	/ranking/?
				{
					if (path.size() == 2 && path[1] == L"users")					//	/ranking/users
					{
						return routes::ranking_users;
					}
					else if (path.size() == 2 && path[1] == L"memes")				//	/ranking/memes
					{
						return routes::ranking_memes;
					}
					else if (path.size() == 2 && path[1] == L"comments")			//	/ranking/comments
					{
						return routes::ranking_comments;
					}
					else															//	/ranking/[garbage]
					{
						return routes::garbage;
					}
				}
			}
			else if (!path.empty() && path[0] == L"login")							//	/login
			{
				return routes::login;
			}
			else if (!path.empty() && path[0] == L"signup")							//	/signup
			{
				return routes::signup;
			}
			else																	//	/[garbage]
			{
				return routes::garbage;
			}

		}
	}

	class RequestProcessor
	{
		web::http::experimental::listener::http_listener& listener;
		web::http::client::http_client& client;
		user_map::user_map& users;
		std::map<unsigned long long, user_map::user&> session_ID_map;

		boost::mt19937_64 rng;

		boost::random::uniform_int_distribution<unsigned long long> dist;
		boost::variate_generator<boost::mt19937_64&, boost::random::uniform_int_distribution<unsigned long long>> gen;

	public:
		RequestProcessor(
			web::http::experimental::listener::http_listener& listener,
			web::http::client::http_client& client,
			user_map::user_map& users) : listener(listener), client(client), users(users),
			rng(std::time(nullptr)),	dist(0, INT64_MAX), gen(rng, dist)
		{
			listener.support(web::http::methods::GET, std::bind( & RequestProcessor::handle_get, this, std::placeholders::_1));
			listener.support(web::http::methods::POST, std::bind(&RequestProcessor::handle_post, this, std::placeholders::_1));
			try
			{
				listener.open().wait();
			}
			catch(std::exception &e)
			{
				std::cout << "error: " << e.what() << std::endl;
			}
			
		}
		void add_session_ID(unsigned long long session_ID, user_map::user& user)
		{
			session_ID_map.insert_or_assign(session_ID,  user);
		}
		auto& get_session_map()
		{
			return session_ID_map;
		}
		void handle_get(web::http::http_request request)
		{
			std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

			unsigned long long session_ID{ 0 };
			bool authorized{ false };

			if (request.headers().find(L"Cookie") != request.headers().end())	// if cookie header exists
			{
				utility::string_t cookie_header{ request.headers().find(L"Cookie")->second };

				std::vector<utility::string_t>cookie_parts;

				boost::split(cookie_parts, cookie_header, boost::is_any_of(L"="));

				if (cookie_parts[0] == L"session_ID" && isNumber(cookie_parts[1]))	// if cookie header is session_ID
				{
					session_ID = std::stoul(cookie_parts[1]);

					if(session_ID_map.contains(session_ID))				// if session_ID is in session_ID_map
					{
						authorized = true;
					}
				}
			}
			

			if (!authorized && session_ID == 0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}


			switch (path_to_route_parser(path))
			{
				/*case routes::empty:
				{

					break;
				}
				case routes::signup:
				{

					break;
				}*/
				case routes::account:
				{


					break;
				}
				case routes::account_username:
				{
					std::wstring account_username_utf16{ path[1] };

					std::wstring cypher_get_user_utf16;
					cypher_get_user_utf16.reserve(256);

					cypher_get_user_utf16.append( L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");

					cypher_get_user_utf16.append(account_username_utf16);
					cypher_get_user_utf16.append(L"\\\"}) return acc }]}");

					web::json::value cypher_get_user_json{ web::json::value::parse(cypher_get_user_utf16) };

					web::http::http_request cypher_get_user_request{ web::http::methods::POST };
					cypher_get_user_request.headers().add(L"Authorization", L"Basic " + auth_64);

					///
					////
					// test
					///

					break;
				}
				case routes::account_username_preferences:
				{

					break;
				}
				case routes::account_username_comments:
				{

					break;
				}
				case routes::meme:
				{
					


					break;
				}
				case routes::meme_id:
				{
					unsigned int meme_id{ static_cast<unsigned>(stoi(path[1])) };

					// MATCH(meme:Meme{id: "
					// "}) return meme
					//
					//
					//
					//
					//

					break;
				}
				case routes::meme_id_comments:
				{
					unsigned int meme_id{ static_cast<unsigned>(stoi(path[1])) };


					std::string cypher_create_user_utf8;
					cypher_create_user_utf8.reserve(256);

					cypher_create_user_utf8.append("{\"statements\": [{\"statement\": \"MATCH(meme:Meme{id: ");
					cypher_create_user_utf8.append(std::to_string(meme_id));
					cypher_create_user_utf8.append( "}) MATCH (acc:Account)-[comment:COMMENTED]->(meme) return comment");
					// 
					//
					// testowac wysylac
					//

					web::json::value cypher_create_user_json{ web::json::value::parse(utility::conversions::to_utf16string(cypher_create_user_utf8)) };

					web::http::http_request cypher_create_user_request{ web::http::methods::POST };

					cypher_create_user_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_create_user_request.set_body(cypher_create_user_json);

					/*
					client.request(cypher_create_user_request).then([](web::http::http_response response)
						{
							std::wcout << response.to_string() << std::endl;
						}).wait();
					//*/

					break;
				}
				case routes::ranking:
				{

					break;
				}
				case routes::ranking_users:
				{

					break;
				}
				case routes::ranking_memes:
				{

					break;
				}
				case routes::ranking_comments:
				{

					break;
				}
				case routes::comment:
				{

					break;
				}
				case routes::comment_id:
				{

					break;
				}
				case routes::comment_id_givelike:
				{

					break;
				}
				case routes::garbage:
				{

					break;
				}
				default:
				{
					//request.reply(web::http::status_codes::MisdirectedRequest);
					return;
				}
			}
			request.reply(web::http::status_codes::MisdirectedRequest);
		}
	

		void handle_post(web::http::http_request request)
		{
			std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

			unsigned long long session_ID{ 0 };
			bool authorized{ false };

			if (request.headers().find(L"Cookie") != request.headers().end())	// if cookie header exists
			{
				utility::string_t cookie_header{ request.headers().find(L"Cookie")->second };

				std::vector<utility::string_t>cookie_parts;

				boost::split(cookie_parts, cookie_header, boost::is_any_of(L"="));

				if (cookie_parts[0] == L"session_ID" && isNumber(cookie_parts[1]))	// if cookie header is session_ID
				{
					session_ID = std::stoull(cookie_parts[1]);

					if (session_ID_map.contains(session_ID))				// if session_ID is in session_ID_map
					{
						authorized = true;
					}
				}
			}

			if (!path.empty() && path[0] == L"login")			// if cookie header does not exist and path is /login
			{
				web::json::value json = request.extract_json().get();

				if (json.has_field(L"username") && json.has_field(L"password"))	// if has username and password fields
				{
					std::string username{ utility::conversions::to_utf8string(json[L"username"].as_string()) };
					std::string password{ utility::conversions::to_utf8string(json[L"password"].as_string()) };

					if (users.get_user_map().contains(username) && users.get_user_map().at(username).password == password)	// if user and password match
					{
						session_ID = gen();

						auto& user = users.get_user(username);

						session_ID_map.insert_or_assign(session_ID, user);

						authorized = true;
					}
					else										// if user does not exist and no session_ID cookie
					{
						request.reply(web::http::status_codes::Unauthorized);
						return;
					}
				}
				else
				{
					request.reply(web::http::status_codes::BadRequest);
					return;
				}

				request.get_response().then([&](web::http::http_response response)
					{
						response.headers().add(L"Set-Cookie", std::format<std::wstring>(L"session_ID={}; Max-Age={}", std::to_wstring(session_ID), 86400));

						//std::wcout << response.to_string() << std::endl;
					}).then([&](){});

				request.reply(web::http::status_codes::Accepted);
				return;
			}
			else if (!path.empty() && path[0] == L"signup")						// if cookie header does not exist and path is /signup
			{
				web::json::value json = request.extract_json().get();

				if (
					!json.has_field(L"username") ||
					!json.has_field(L"nickname") ||
					!json.has_field(L"password") ||
					!json.has_field(L"email")
					)
				{
					request.reply(web::http::status_codes::BadRequest);
					return;
				}

				if (users.get_user_map().contains(utility::conversions::to_utf8string(json.at(L"username").as_string())))
				{
					request.reply(web::http::status_codes::Conflict);
					return;
				}

				std::wstring username{ json.at(L"username").as_string() };
				std::wstring nickname{ json.at(L"nickname").as_string() };
				std::wstring password{ json.at(L"password").as_string() };
				std::wstring email{ json.at(L"email").as_string() };

				std::string username_utf8{ utility::conversions::to_utf8string(username) };
				std::string nickname_utf8{ utility::conversions::to_utf8string(nickname) };
				std::string password_utf8{ utility::conversions::to_utf8string(password) };
				std::string email_utf8{ utility::conversions::to_utf8string(email) };

				users.add_user(
					utility::conversions::to_utf8string(username),
					utility::conversions::to_utf8string(password),
					user_map::generate::user()
				);

				std::string cypher_create_user_utf8{ "{\"statements\": [{\"statement\": \" CREATE(u:Account:User{username: \\\" " };
				
				cypher_create_user_utf8 += username_utf8;
				cypher_create_user_utf8 += "\\\", nickname: \\\" ";
				cypher_create_user_utf8 += nickname_utf8;
				cypher_create_user_utf8 += " \\\", email: \\\" ";
				cypher_create_user_utf8 += email_utf8;

				cypher_create_user_utf8 += " \\\" }) \"}]} ";

				std::wstring cypher_create_user{ utility::conversions::to_utf16string(cypher_create_user_utf8) };

				//std::wcout << cypher_create_user << std::endl;

				web::json::value cypher_create_user_json{ web::json::value::parse(cypher_create_user) };

				web::http::http_request cypher_create_user_request{ web::http::methods::POST };

				cypher_create_user_request.headers().add(L"Authorization",L"Basic " + auth_64);
				cypher_create_user_request.set_body(cypher_create_user_json);

				//std::wcout << cypher_create_user_request.to_string() << std::endl;

				client.request(cypher_create_user_request).then([](web::http::http_response response)
					{
						std::wcout << response.to_string() << std::endl;
					}).wait();

				request.reply(web::http::status_codes::Created);
				return;
			}

			if (!authorized && session_ID==0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}

			auto& user = session_ID_map.at(session_ID);

			web::json::value json;

			try
			{
				json = request.extract_json().get();
			}
			catch (std::exception& e)
			{
				std::cout << "error: " << e.what() << std::endl;
				request.reply(web::http::status_codes::BadRequest,e.what());
				return;
			}

			switch(path_to_route_parser(path))
			{
			case routes::comment:
				{
					if (
						!json.has_field(L"body") ||
						!json.has_field(L"meme_id")
						)
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}

					unsigned int meme_id{ static_cast<unsigned int>(json.at(L"meme_id").as_integer())};
					std::wstring body_utf16{ json.at(L"body").as_string() };

					std::string body_utf8{ utility::conversions::to_utf8string(body_utf16) };

					std::string username_utf8{ user.username };

					std::string neo4j_cypher_utf8;
					neo4j_cypher_utf8.reserve(256);


					neo4j_cypher_utf8.append("{\"statements\": [{\"statement\": \" MATCH (acc:Account{username: \\\"");

					neo4j_cypher_utf8.append(username_utf8);
					neo4j_cypher_utf8.append("\\\"}) MATCH (meme:Meme{id: \\\"");
					neo4j_cypher_utf8.append(std::to_string(meme_id));
					neo4j_cypher_utf8.append("\\\"}) CREATE (acc)-[:COMMENTED{body: \\\"");
					neo4j_cypher_utf8.append(body_utf8);
					neo4j_cypher_utf8.append("\\\"}]->(meme) \" }]}");

					std::wstring neo4j_cypher_utf16 = utility::conversions::to_utf16string(neo4j_cypher_utf8);

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);


					// wyslac i wytestowac

					break;
				}
			case routes::meme:
				{
					if(
						!json.has_field(L"title")	||
						!json.has_field(L"url")		||
						!json.has_field(L"tags")
						)
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}
					
					std::string username_utf8{ user.username };

					std::string neo4j_cypher_utf8;
					neo4j_cypher_utf8.reserve(256);


				}
			case routes::meme_id_givelike:
				{
					std::wstring meme_uuid_utf16{path[1]};

					std::string username_utf8{ user.username };

					std::string neo4j_cypher_utf8;
					neo4j_cypher_utf8.reserve(256);

					neo4j_cypher_utf8.append("MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf8.append(username_utf8);
					neo4j_cypher_utf8.append("\\\"}) MATCH(meme:Meme{uuid: ");
					neo4j_cypher_utf8.append(utility::conversions::to_utf8string(meme_uuid_utf16));
					neo4j_cypher_utf8.append("}) CREATE(acc)-[:LIKED]->(meme)");

					std::wstring neo4j_cypher_utf16 = utility::conversions::to_utf16string(neo4j_cypher_utf8);

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([](web::http::http_response response)
						{
							std::wcout << response.to_string() << std::endl;
						}).wait();		
					//*/

					//testeeeee
					break;
				}

			default:
				{
					request.reply(web::http::status_codes::NotFound);
					break;
				}

			}
		}
	};
}