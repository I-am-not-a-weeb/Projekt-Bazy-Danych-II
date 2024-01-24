#ifdef _DEBUG

#define _DEBUG_WPRINT(x) std::wcout << x << std::endl;
#define _DEBUG_PRINT(x)  std::cout  << x << std::endl;

#else

#define _DEBUG_WPRINT(x) 
#define _DEBUG_PRINT(x)

#endif

//path routing method as per https://github.com/microsoft/cpprestsdk/issues/186#issuecomment-230072639

#pragma once
#include <map>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include "usermap.h"

#include <boost/algorithm/string.hpp>
#include <boost/random.hpp>


#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

//#include "RESTapi.h"

const std::wstring neo4j_uri{ L"http://localhost:7474/db/neo4j/tx/commit" };
const std::wstring auth_64{ L"bmVvNGo6dGVzdDEyMzQ=" };

bool isNumber(utility::string_t& str);

/**/


namespace requests_base64
{
	inline std::string decode64(const std::string& val) {
		using namespace boost::archive::iterators;
		using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
		return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {return c == '\0'; });
	}

	inline std::string encode64(const std::string& val) {
		using namespace boost::archive::iterators;
		using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
		auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
		return tmp.append((3 - val.size() % 3) % 3, '=');
	}
}
//*/

enum class routes : unsigned short
{
	empty = 0,
	garbage = 999,
	signup = 1,						// POST
	login = 2,						// POST		

	admin_dump,						// GET

	feed_page = 10,					// GET
	preferences,					// GET, PATCH

	search,							// GET

	account = 20,					// POST
	account_username,				// GET, PATCH
	account_username_preferences,	// GET, PATCH
	account_username_givelike,		// POST
	account_username_comments,		// GET
	account_username_memes,			// GET
	account_username_report,		// POST

	account_username_ban,			// POST
	account_username_unban,			// POST
	account_username_perms, 		// GET, PATCH

	meme = 100,						// GET, POST
	meme_id,						// GET, PATCH
	meme_id_comment,				// POST
	meme_id_comments,				// GET
	meme_id_givelike,				// POST

	meme_id_delete,					// DELETE
	meme_id_report,					// POST


	ranking = 200,					// GET
	ranking_users,					// GET
	ranking_memes,					// GET
	ranking_comments,				// GET

	comment = 300,					// POST
	comment_id,						// GET, PATCH
	comment_id_comment,				// POST	!!
	comment_id_comments,			// GET	!!
	comment_id_givelike,			// POST

	comment_id_delete,				// DELETE ??
	comment_id_edit,				// PATCH ??	
	comment_id_report,				// POST ??

	exportdata = 400,				// GET

	saveusers,						// POST
	loadusers,						// POST
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
						else if (path.size() == 3 && path[2] == L"ban")				//	/account/:username/ban
						{
							return routes::account_username_ban;
						}
						else if(path.size() == 3 && path[2] == L"memes")
						{
							return routes::account_username_memes;
						}
						else if (path.size() == 3 && path[2] == L"unban")			//	/account/:username/unban
						{
							return routes::account_username_unban;
						}
						else if (path.size() == 3 && path[2] == L"perms")			//	/account/:username/perms
						{
							return routes::account_username_perms;
						}
						else if(path.size()==3 && path[2] == L"report")
						{
							return routes::account_username_report;
						}
						else														//	/account/:username/[garbage]
						{
							return routes::garbage;
						}
					}
				}
			}
			else if (!path.empty() && path[0] == L"meme")							//	/account/meme/?
			{
				if (path.size() == 1)												//	/account/meme
				{
					return routes::meme;
				}
				else																//	/account/meme/?
				{
					if (path.size() == 2)											//	/account/meme/:id
					{
						return routes::meme_id;
					}
					else															//	/account/meme/[garbage]
					{	if(path.size() == 3 && path[2] == L"comment")
						{
							return routes::meme_id_comment;
						}
						else if (path.size() == 3 && path[2] == L"comments")				//	/account/meme/:id/comments
						{
							return routes::meme_id_comments;
						}
						else if (path.size() == 3 && path[2] == L"givelike")		//	/account/meme/:id/givelike
						{
							return routes::meme_id_givelike;
						}
						else if (path.size() == 3 && path[2] == L"delete")			//	/account/meme/:id/delete
						{
							return routes::meme_id_delete;
						}
						else if (path.size() == 3 && path[2] == L"report")			//	/account/meme/:id/report
						{
							return routes::meme_id_report;
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
					if (path.size() == 2)					//	/comments/:id
					{
						return routes::comment_id;
					}
					else															//	/comments/[garbage]
					{
						if (path.size() == 3 && path[2] == L"givelike")				//	/comments/:id/givelike
						{
							return routes::comment_id_givelike;
						}
						else if (path.size() == 3 && path[2] == L"comment")			//	/comments/:id/comment
						{
							return routes::comment_id_comment;
						}
						else if (path.size() == 3 && path[2] == L"report")			//	/comments/:id/report
						{
							return routes::comment_id_report;
						}
						else if (path.size() == 3 && path[2] == L"edit")			//	/comments/:id/edit
						{
							return routes::comment_id_edit;
						}
						else if (path.size() == 3 && path[2] == L"delete")			//	/comments/:id/delete
						{
							return routes::comment_id_delete;
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
			else if (path.size() == 2 && path[0] == L"feed" && isNumber(path[1]))							//	/feed
			{
				return routes::feed_page;
			}
			else if (!path.empty() && path[0] == L"preferences")					//	/preferences
			{
				return routes::preferences;
			}
			else if (!path.empty() && path[0] == L"login")							//	/login
			{
				return routes::login;
			}
			else if (!path.empty() && path[0] == L"signup")							//	/signup
			{
				return routes::signup;
			}
			else if(!path.empty() && path[0] == L"search")							//	/search
			{
				return routes::search;
			}
			else if (path.size() == 2 && path[0] == L"admin" && path[1] == L"dump")	//	/admin_dump
			{
				return routes::admin_dump;
			}
			else if(!path.empty() && path[0] == L"exportdata")						//	/exportdata
			{
				return routes::exportdata;
			}
			else if(!path.empty() && path[0] == L"saveusers")						//	/saveusers
			{
				return routes::saveusers;
			}
			else if(!path.empty() && path[0] == L"loadusers")						//	/loadusers
			{
				return routes::loadusers;
			}
			else																	//	/[garbage]
			{
				return routes::garbage;
			}
			return routes::garbage;
		}
	}

	class request_processor
	{
		web::http::experimental::listener::http_listener& listener;
		web::http::client::http_client& client;

		user_map::user_map& users;
		std::string path_to_file{ "users.txt" };

		std::map<unsigned long long, user_map::user&> session_ID_map;

		boost::mt19937_64 rng;

		boost::random::uniform_int_distribution<unsigned long long> dist;
		boost::variate_generator<boost::mt19937_64&, boost::random::uniform_int_distribution<unsigned long long>> gen;

	public:
		request_processor(
			web::http::experimental::listener::http_listener& listener,
			web::http::client::http_client& client,
			user_map::user_map& users) : listener(listener), client(client), users(users),
			rng(std::time(nullptr)),	dist(0, INT64_MAX), gen(rng, dist)
		{
			listener.support(web::http::methods::GET, std::bind( & request_processor::handle_get, this, std::placeholders::_1));
			listener.support(web::http::methods::POST, std::bind(&request_processor::handle_post, this, std::placeholders::_1));
			listener.support(web::http::methods::OPTIONS, std::bind(&request_processor::handle_options, this, std::placeholders::_1));
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
		void handle_options(web::http::http_request request)
		{
			request.get_response().then([](web::http::http_response response)
				{
					response.headers().add(L"Access-Control-Allow-Origin", L"http://localhost:3000");
					response.headers().add(L"Access-Control-Allow-Methods", L"GET, POST, OPTIONS");
					response.headers().add(L"Access-Control-Allow-Headers", L"Content-Type, Authorization");
					response.headers().add(L"Access-Control-Allow-Credentials", L"true");
					response.headers().add(L"Access-Control-Max-Age", L"86400");
				});
			request.reply(web::http::status_codes::OK);
		}
		void handle_get(web::http::http_request request)
		{
			std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

			unsigned long long session_ID{ 0 };
			bool authorized{ false };

			request.get_response().then([](web::http::http_response response)
				{
					response.headers().add(L"Access-Control-Allow-Origin", L"http://localhost:3000");
					response.headers().add(L"Access-Control-Allow-Methods", L"GET, POST, OPTIONS, PATCH,DELETE");
					response.headers().add(L"Access-Control-Allow-Credentials", L"true");

				});

			if (request.headers().find(L"Cookie") != request.headers().end())	// if cookie header exists
			{
				utility::string_t cookie_header{ request.headers().find(L"Cookie")->second };

				std::vector<utility::string_t>cookie_parts;

				boost::split(cookie_parts, cookie_header, boost::is_any_of(L"="));

				if (cookie_parts[0] == L"session_ID" && isNumber(cookie_parts[1]))	// if cookie header is session_ID
				{
					session_ID = std::stoull(cookie_parts[1]);

					if(session_ID_map.contains(session_ID))				// if session_ID is in session_ID_map
					{
						authorized = true;
					}
				}
			}

			if (!authorized || session_ID == 0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}

			auto& user = session_ID_map.at(session_ID);

			if (user.permissions_union.permissions.banned)
			{
				request.reply(web::http::status_codes::Forbidden);
				return;
			}

			switch (path_to_route_parser(path))
			{
			case routes::search:
				{
					auto query = web::http::uri::split_query(web::http::uri::decode(request.relative_uri().query()));

					utility::string_t searchbar = query.contains(L"searchbar") ? query.at(L"searchbar") : L"";

					utility::string_t tag = query.contains(L"tag") ? query.at(L"tag") : L"";

					std::wstring cypher_search_utf16;
					cypher_search_utf16.reserve(256);

					cypher_search_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(meme:Meme) where TRUE ");
					if (!searchbar.empty())
					{
						cypher_search_utf16.append(L" AND  meme.title =~ \\\".*" + searchbar + L".*\\\" ");
					}
					if(!tag.empty())
					{
						cypher_search_utf16.append(L" AND \\\"" + tag + L"\\\" in meme.tags ");
					}
					cypher_search_utf16.append(L" return meme ORDER BY meme.created_at DESC\"}]}");

					web::json::value cypher_search_json{ web::json::value::parse(cypher_search_utf16) };
					web::http::http_request cypher_search_request{ web::http::methods::POST };
					cypher_search_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_search_request.set_body(cypher_search_json);

					client.request(cypher_search_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value before_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data")
							};

							web::json::value after_response_json = web::json::value::array();

							for(int i = 0; i < before_response_json.size(); i++)
							{
								after_response_json[i] = before_response_json[i].as_object().at(L"row").as_array().at(0);
							}
							request.reply(web::http::status_codes::OK, after_response_json).wait();

						}).wait();

					break;
				}
			case routes::feed_page:
				{
					std::wstring cypher_get_feed_utf16;
					cypher_get_feed_utf16.reserve(256);

					cypher_get_feed_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(meme:Meme) RETURN meme ORDER BY meme.created_at DESC LIMIT ");
					cypher_get_feed_utf16.append(std::to_wstring(stoi(path[1])*10+10));
					cypher_get_feed_utf16.append(L" SKIP ");
					cypher_get_feed_utf16.append(std::to_wstring(stoi(path[1]) * 10));
					cypher_get_feed_utf16.append(L"\\\"\"}]}");

					web::json::value cypher_get_feed_json{ web::json::value::parse(cypher_get_feed_utf16) };

					web::http::http_request cypher_get_feed_request{ web::http::methods::GET };

					cypher_get_feed_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_feed_request.set_body(cypher_get_feed_json);

					client.request(cypher_get_feed_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row")
							};
							//_DEBUG_WPRINT(response.to_string())
							request.reply(web::http::status_codes::OK, send_response_json);
							
						}).wait();
					break;
				}
				case routes::preferences:
				{
					utility::string_t username_utf16{ utility::conversions::to_string_t(user.username) };

					std::wstring cypher_get_preferences_utf16;
					cypher_get_preferences_utf16.reserve(256);

					cypher_get_preferences_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account)-[:PREFERENCES]->(prefs:Preferences) where acc.username=\\\"");
					cypher_get_preferences_utf16.append(username_utf16);
					cypher_get_preferences_utf16.append(L"\\\" return prefs \"}]}");

					web::json::value cypher_get_user_json{ web::json::value::parse(cypher_get_preferences_utf16) };

					web::http::http_request cypher_get_user_request{ web::http::methods::POST };
					cypher_get_user_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_user_request.set_body(cypher_get_user_json);

					client.request(cypher_get_user_request).then([&](web::http::http_response response)
					{
						web::json::value response_json{ cypher_get_user_request.extract_json().get() };

						web::json::value send_response_json
						{
							response_json.at(L"results").as_array().at(0).as_object()
							.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
						};

						request.reply(web::http::status_codes::OK, send_response_json);
					}).wait();

					break;
				}
			    case routes::account:
				{
					std::wstring account_username_utf16{ path[1] };

					std::wstring cypher_get_user_utf16;
					cypher_get_user_utf16.reserve(256);

					cypher_get_user_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account}) return acc \"}]}");

					web::json::value cypher_get_user_json{ web::json::value::parse(cypher_get_user_utf16) };

					web::http::http_request cypher_get_users_request{ web::http::methods::POST };
					cypher_get_users_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_users_request.set_body(cypher_get_user_json);

					client.request(cypher_get_users_request).then([&](web::http::http_response response)
						{
						web::json::value response_get_users_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_get_users_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};

							request.reply(web::http::status_codes::OK, send_response_json);
							//_DEBUG_WPRINT(response.to_string())

						}).wait();

					break;
				}

				case routes::account_username:
				{
					std::wstring account_username_utf16{ path[1] };

					std::wstring cypher_get_user_utf16;
					cypher_get_user_utf16.reserve(256);

					cypher_get_user_utf16.append( L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");

					cypher_get_user_utf16.append(account_username_utf16);
					cypher_get_user_utf16.append(L"\\\"}) return acc \"}]}");

					web::json::value cypher_get_user_json{ web::json::value::parse(cypher_get_user_utf16) };

					web::http::http_request cypher_get_user_request{ web::http::methods::POST };
					cypher_get_user_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_user_request.set_body(cypher_get_user_json);

					/**/
					client.request(cypher_get_user_request).then([&](web::http::http_response response)
						{
							web::json::value cypher_response_json;
							cypher_response_json = response.extract_json().get();

							web::json::value send_response_json
							{
								cypher_response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};

							request.reply(web::http::status_codes::OK, send_response_json);
							//_DEBUG_WPRINT(response.to_string())
							return;

						}).wait();
					//*/
					
					break;
				}
				case routes::account_username_preferences:
				{

					break;
				}
				case routes::account_username_comments:
				{
					std::wstring account_username_utf16{ path[1] };

					std::wstring cypher_get_account_comments_utf16;
					cypher_get_account_comments_utf16.reserve(256);

					cypher_get_account_comments_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					cypher_get_account_comments_utf16.append(account_username_utf16);
					cypher_get_account_comments_utf16.append(L"\\\"}) MATCH (acc)-[comment:COMMENTED]->(:MEME) return comment\"}]}");

					web::json::value cypher_get_account_comments_json{ web::json::value::parse(cypher_get_account_comments_utf16) };

					web::http::http_request cypher_get_account_comments_request{ web::http::methods::POST };
					cypher_get_account_comments_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_account_comments_request.set_body(cypher_get_account_comments_json);

					/**/
					client.request(cypher_get_account_comments_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row")
							};

							request.reply(web::http::status_codes::OK, send_response_json);

							//_DEBUG_WPRINT(response.to_string())
							//std::wcout << response.to_string() << std::endl;
						}).wait();
					//*/

					break;
				}
				case routes::account_username_memes:
				{
					std::wstring account_username_utf16{ path[1] };

					std::wstring cypher_get_account_memes_utf16;
					cypher_get_account_memes_utf16.reserve(256);

					cypher_get_account_memes_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					cypher_get_account_memes_utf16.append(account_username_utf16);
					cypher_get_account_memes_utf16.append(L"\\\"}) MATCH (acc)-[:AUTHORED]->(memes:Meme) return memes ORDER BY memes.created_at DESC\"}]}");

					web::json::value cypher_get_account_memes_json{ web::json::value::parse(cypher_get_account_memes_utf16) };

					web::http::http_request cypher_get_account_memes_request{ web::http::methods::POST };
					cypher_get_account_memes_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_account_memes_request.set_body(cypher_get_account_memes_json);

					/**/
					client.request(cypher_get_account_memes_request).then([&](web::http::http_response response)
						{
							try
							{
								web::json::value cypher_response_json{ response.extract_json().get() };

								//std::wcout << cypher_response_json.serialize() << std::endl;

								web::json::value before_response_json
								{
									cypher_response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data")
								};

								web::json::value after_response_json = web::json::value::array();

								for(int i = 0; i < before_response_json.size(); i++)
								{
									after_response_json[i] = before_response_json[i].as_object().at(L"row").as_array().at(0);
								}

								request.reply(web::http::status_codes::OK, after_response_json).wait();
								//_DEBUG_WPRINT(response.to_string())
								return;
							}
							catch (std::exception& e)
							{
								std::cout << "error: " << e.what() << std::endl;
							}
						}).wait();
					//*/

					break;
				}
				case routes::meme:
				{
					auto kek = web::http::uri::split_query(web::http::uri::decode(request.relative_uri().query()));

					std::wstring filter = kek.contains(L"filter") ? kek.at(L"filter") : L"";

					std::wstring cypher_get_meme_utf16;
					cypher_get_meme_utf16.reserve(256);

					cypher_get_meme_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(meme:Meme) ");
					if(!filter.empty())
					{
						cypher_get_meme_utf16.append(L" where \\\"");
						cypher_get_meme_utf16.append(filter);
						cypher_get_meme_utf16.append(L"\\\" in meme.tags ");
					}
					cypher_get_meme_utf16.append(L"return meme\"}]}");

					web::json::value cypher_get_meme_json{ web::json::value::parse(cypher_get_meme_utf16) };
					web::http::http_request cypher_get_meme_request{ web::http::methods::POST };

					cypher_get_meme_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_meme_request.set_body(cypher_get_meme_json);

					client.request(cypher_get_meme_request).then([&](web::http::http_response response)
					{
						web::json::value response_json{ response.extract_json().get() };

						web::json::value send_response_json
						{
							response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row")
						};
						//_DEBUG_WPRINT(response_json)
						request.reply(web::http::status_codes::OK, send_response_json);
					}).wait();

				}
				case routes::meme_id:
				{
					std::wstring meme_uuid_utf16{ path[1] };


					std::wstring cypher_get_meme_utf16;
					cypher_get_meme_utf16.reserve(256);

					cypher_get_meme_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(meme:Meme{id: \\\"");
					cypher_get_meme_utf16.append(meme_uuid_utf16);
					cypher_get_meme_utf16.append(L"\\\"}) return meme \"}]}");

					web::json::value cypher_get_meme_json{ web::json::value::parse(cypher_get_meme_utf16) };

					web::http::http_request cypher_get_meme_request{ web::http::methods::POST };
					cypher_get_meme_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_get_meme_request.set_body(cypher_get_meme_json);


					client.request(cypher_get_meme_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
									response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};
							

							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();

					break;
				}
				case routes::meme_id_comments:
				{
					std::wstring meme_id{ path[1] };

					std::wstring cypher_create_user_utf16;
					cypher_create_user_utf16.reserve(256);

					cypher_create_user_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(meme:Meme{id: \\\"");
					cypher_create_user_utf16.append(meme_id);
					cypher_create_user_utf16.append(L"\\\"}) MATCH(acc:Account) -[:COMMENTED]->(comment:Comment)-[:COMMENTED_ON]->(meme:Meme) return comment ORDER BY comment.created_at DESC \"}]}");

					web::json::value cypher_create_user_json{ web::json::value::parse(utility::conversions::to_utf16string(cypher_create_user_utf16)) };

					web::http::http_request cypher_create_user_request{ web::http::methods::POST };

					cypher_create_user_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_create_user_request.set_body(cypher_create_user_json);

					client.request(cypher_create_user_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value before_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data")
							};

							web::json::value after_response_json = web::json::value::array();

							for(int i = 0; i < before_response_json.size(); i++)
							{
								after_response_json[i] = before_response_json[i].as_object().at(L"row").as_array().at(0);
							}
							
							request.reply(web::http::status_codes::OK, after_response_json);
						}).wait();

					break;
				}
				case routes::ranking:
				{



					break;
				}
				case routes::ranking_users:
				{
					//match(acc:Account)-[:LIKED]->(meme:Meme) with meme, count(meme) as like_num, collect(meme) as memes return like_num, memes

					std::wstring cypher_ranking_users_utf16;
					cypher_ranking_users_utf16.reserve(256);

					cypher_ranking_users_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account)-[:LIKED]->(meme:Meme) with meme, count(meme) as like_num, collect(meme) as memes order by like_num return like_num, memes\"}]}");

					web::http::http_request cypher_ranking_users_request{ web::http::methods::POST };
					cypher_ranking_users_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_ranking_users_request.set_body(web::json::value::parse(cypher_ranking_users_utf16));

					client.request(cypher_ranking_users_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row")
							};
							
							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();
					
					break;
				}
				case routes::ranking_memes:
				{
					std::wstring cypher_ranking_users_utf16;
					cypher_ranking_users_utf16.reserve(256);

					cypher_ranking_users_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account)-[:LIKED]->(acc2:Account) with acc2, count(acc2) as like_num, collect(acc2) as acc2 order by like_num return like_num, acc2\"}]}");

					web::http::http_request cypher_ranking_users_request{ web::http::methods::POST };
					cypher_ranking_users_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_ranking_users_request.set_body(web::json::value::parse(cypher_ranking_users_utf16));

					client.request(cypher_ranking_users_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row")
							};
							
							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();

					break;
				}
				case routes::ranking_comments:
				{
					std::wstring cypher_ranking_users_utf16;
					cypher_ranking_users_utf16.reserve(256);

					cypher_ranking_users_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account)-[:LIKED]->(com:Comment) with com, count(com) as like_num, collect(com) as coms order by like_num return like_num, coms\"}]}");

					web::http::http_request cypher_ranking_users_request{ web::http::methods::POST };
					cypher_ranking_users_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_ranking_users_request.set_body(web::json::value::parse(cypher_ranking_users_utf16));

					client.request(cypher_ranking_users_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row")
							};
							
							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();


					break;
				}
				case routes::comment:
				{

					break;
				}
				case routes::comment_id:
				{
					std::wstring comment_id_utf16{ path[1] };

					std::wstring cypher_comment_utf16;
					
					cypher_comment_utf16.reserve(256);
					cypher_comment_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(comment:Comment{id: \\\"");
					cypher_comment_utf16.append(comment_id_utf16);
					cypher_comment_utf16.append(L"\\\"}) return comment\"}]}");

					web::json::value cypher_comment_json{ web::json::value::parse(cypher_comment_utf16) };
					web::http::http_request cypher_comment_request{ web::http::methods::POST };
					cypher_comment_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_comment_request.set_body(cypher_comment_json);

					client.request(cypher_comment_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};
							
							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();

					break;
				}
				case routes::comment_id_comments:
				{
					std::wstring comment_id_utf16{ path[1] };

					std::wstring cypher_comment_utf16;
					cypher_comment_utf16.reserve(256);

					cypher_comment_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(comment:Comment{id: \\\"");
					cypher_comment_utf16.append(comment_id_utf16);
					cypher_comment_utf16.append(L"\\\"}) MATCH(acc:Account)-[:COMMENTED]->(com:Comment)-[:COMMENTED_ON]->(comment:Comment) return com ORDER BY com.created_at DESC \"}]}");

					web::json::value cypher_comment_json{ web::json::value::parse(cypher_comment_utf16) };
					web::http::http_request cypher_comment_request{ web::http::methods::POST };

					cypher_comment_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_comment_request.set_body(cypher_comment_json);

					client.request(cypher_comment_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
									.at(L"data")
							};

							web::json::value after_response_json = web::json::value::array();

							for(int i = 0; i < send_response_json.size(); i++)
							{
								after_response_json[i] = send_response_json[i].as_object().at(L"row").as_array().at(0);
							}

							request.reply(web::http::status_codes::OK, after_response_json);
						}).wait();

					break;
				}
				case routes::admin_dump:
				{
					std::wstring cypher_dump_utf16;
					cypher_dump_utf16.reserve(256);

					cypher_dump_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(all) return all\"}]}");

					web::json::value cypher_dump_json{ web::json::value::parse(cypher_dump_utf16) };
					web::http::http_request cypher_dump_request{ web::http::methods::POST };

					cypher_dump_request.headers().add(L"Authorization", L"Basic " + auth_64);
					cypher_dump_request.set_body(cypher_dump_json);

					client.request(cypher_dump_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{ response.extract_json().get() };

							request.reply(web::http::status_codes::OK, response_json);
						}).wait();

					break;
				}
				case routes::garbage:
				{
					request.reply(web::http::status_codes::MisdirectedRequest);
					break;
				}
				default:
				{
					return;
				}
			}
			// request.reply(web::http::status_codes::MisdirectedRequest);
		}

		void handle_post(web::http::http_request request)
		{
			//web::http::http_request* request_ptr = new web::http::http_request;

			//*request_ptr = request;

			std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

			unsigned long long session_ID{ 0 };
			bool authorized{ false };

			request.get_response().then([&](web::http::http_response response)
				{
					response.headers().add(L"Access-Control-Allow-Origin", L"http://localhost:3000");
					response.headers().add(L"Access-Control-Allow-Methods", L"GET, POST, OPTIONS, PATCH,DELETE");
					response.headers().add(L"Access-Control-Allow-Credentials", L"true");
					std::cout << "GET added headers" << std::endl;
				});

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
						try {
							response.headers().add(L"Set-Cookie", std::format<std::wstring>(L"session_ID={}; Max-Age={}", std::to_wstring(session_ID), 86400));
							std::wcout << response.headers().find(L"Set-Cookie")->second << std::endl;
							std::cout << "POST added headers" << std::endl;
						}catch (std::exception& e)
						{
							std::cout << "error: " << e.what() << std::endl;
						}
					});

				web::json::value response_json= web::json::value::parse(std::format<std::wstring>(L"{{\"new_session_ID\":{}}}", std::to_wstring(session_ID)));

				request.reply(web::http::status_codes::Accepted,response_json).wait();
				return;
			}
			else if (!path.empty() && path[0] == L"signup")						// if cookie header does not exist and path is /signup
			{
				web::json::value json = request.extract_json().get();

				if (
					!json.has_field(L"avatar") ||
					!json.has_field(L"image_type") ||
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
				std::wstring avatar{ json.at(L"avatar").as_string() };
				std::wstring image_type{ json.at(L"image_type").as_string() };
				std::wstring username{ json.at(L"username").as_string() };
				std::wstring nickname{ json.at(L"nickname").as_string() };
				std::wstring password{ json.at(L"password").as_string() };
				std::wstring email{ json.at(L"email").as_string() };

				std::string avatar_utf8{ utility::conversions::to_utf8string(avatar) };
				std::string image_type_utf8{ utility::conversions::to_utf8string(image_type) };
				std::string username_utf8{ utility::conversions::to_utf8string(username) };
				std::string nickname_utf8{ utility::conversions::to_utf8string(nickname) };
				std::string password_utf8{ utility::conversions::to_utf8string(password) };
				std::string email_utf8{ utility::conversions::to_utf8string(email) };

				users.add_user(
					utility::conversions::to_utf8string(username),
					utility::conversions::to_utf8string(password),
					user_map::generate::user()
				);

				std::string cypher_create_user_utf8{ "{\"statements\": [{\"statement\":\"CREATE(u:Account:User{avatar:\\\"" };
				cypher_create_user_utf8.append (avatar_utf8);
				cypher_create_user_utf8.append("\\\",image_type:\\\"");
				cypher_create_user_utf8 += image_type_utf8;
				cypher_create_user_utf8 += "\\\",username:\\\"";
				cypher_create_user_utf8 += username_utf8;
				cypher_create_user_utf8 += "\\\",nickname:\\\"";
				cypher_create_user_utf8 += nickname_utf8;
				cypher_create_user_utf8 += "\\\",email:\\\"";
				cypher_create_user_utf8 += email_utf8;
				cypher_create_user_utf8 += "\\\", created_at: timestamp()})-[:PREFERENCES]->(prefs:Preferences{}) with u, apoc.create.uuid() as avatar_id SET u.avatar_id = avatar_id return u.avatar_id as avatar_id\"}]}";	// TODO preferences

				std::wstring cypher_create_user{ utility::conversions::to_utf16string(cypher_create_user_utf8) };

				web::json::value cypher_create_user_json{ web::json::value::parse(cypher_create_user) };
				web::http::http_request cypher_create_user_request(web::http::methods::POST);

				cypher_create_user_request.headers().add(L"authorization", L"Basic " + auth_64);
				cypher_create_user_request.headers().add(L"Content-Type", L"application/json");
				cypher_create_user_request.set_body(cypher_create_user_json);

				_DEBUG_WPRINT(cypher_create_user_request.to_string())

				//std::string* uuid_path = new std::string;

				auto kek = client.request(cypher_create_user_request).then([&](web::http::http_response response)
					{
						try 
						{
							

							web::json::value send_response_json
							{
								response.extract_json().get().at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};

							//*uuid_path = utility::conversions::to_utf8string(send_response_json.as_string());

							request.reply(web::http::status_codes::Created);
						}
						catch (std::exception& e)
						{
							std::cout << "error: " << e.what() << std::endl;
						}
					}).wait();

				//std::fstream avatar_file{ *uuid_path + '.' + image_type_utf8, std::ios::out | std::ios::binary };

				//avatar_file << avatar_utf8;
				//avatar_file.close();

				//delete uuid_path;

				return;
			}

			if (!authorized || session_ID==0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}

			auto& user = session_ID_map.at(session_ID);

			if(user.permissions_union.permissions.banned)
			{
				request.reply(web::http::status_codes::Forbidden);
				return;
			}

			web::json::value json;

			try
			{
				if(request.headers().content_type() == L"application/json")	json = request.extract_json().get();
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

					if(!user.permissions_union.permissions.can_add_comments)
					{
						request.reply(web::http::status_codes::Forbidden);
						return;
					}

					std::wstring meme_uuid_utf16{json.at(L"meme_id").as_string()};
					std::wstring body_utf16{ json.at(L"body").as_string() };

					std::string username_utf8{ user.username };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);


					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \" MATCH (acc:Account{username: \\\"");

					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(username_utf8));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH (meme:Meme{id: \\\"");
					neo4j_cypher_utf16.append(meme_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) CREATE (acc)-[:COMMENTED]->(com:COMMENT{body: \\\"");
					neo4j_cypher_utf16.append(body_utf16);
					neo4j_cypher_utf16.append(L"\\\"})-[:COMMENTED_ON]->(meme) WITH com, apoc.create.uuid() AS comId SET com.id = comId SET com.dt = datetime() RETURN com.id AS comId");
					neo4j_cypher_utf16.append(L"\"}]}");


					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							web::json::value response_json{	response.extract_json().get() };

							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};
							
							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();

					break;
				}
				case routes::comment_id:
				{
					if (
						!json.has_field(L"body") ||
						!json.has_field(L"meme_id")
						)
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}
					if(!user.permissions_union.permissions.can_modify_comments)
					{
						request.reply(web::http::status_codes::Forbidden);
						return;
					}

					std::wstring com_uuid_utf16{ path[1] };

					std::wstring meme_uuid_utf16{ json.at(L"meme_id").as_string() };
					std::wstring body_utf16{ json.at(L"body").as_string() };

					std::string username_utf8{ user.username };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \" MATCH (acc:Account{username: \\\"");

					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(username_utf8));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH (meme:Meme{id: \\\"");
					neo4j_cypher_utf16.append(meme_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MATCH (com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(com_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) CREATE (acc)-[:COMMENTED]->(com:Comment{body: \\\"");
					neo4j_cypher_utf16.append(body_utf16);
					neo4j_cypher_utf16.append(L"\\\"})-[:COMMENTED_ON]->(com) \" }]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							

							web::json::value response_json{ response.extract_json().get() };
							
							web::json::value send_response_json
							{
								response_json.at(L"results").as_array().at(0).as_object()
								.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};

							request.reply(web::http::status_codes::OK, send_response_json);
						}).wait();

					break;
				}
				case routes::comment_id_comment:
				{
					if(!json.has_field(L"body"))
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}

					std::wstring com_uuid_utf16{ path[1] };

					std::string username_utf8{ user.username };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(username_utf8));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(com_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) CREATE(acc)-[:COMMENTED]->(com:Comment{body: \\\"");
					neo4j_cypher_utf16.append(json.at(L"body").as_string());
					neo4j_cypher_utf16.append(L"\\\"})-[:COMMENTED_ON]->(com) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };
					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::Created);
						}).wait();
				}
				case routes::comment_id_report:
				{
					std::wstring com_uuid_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(user.username));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(com_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:REPORTED]->(com) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };
					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK).wait();
						}).wait();

					break;
				}
				case routes::comment_id_givelike:
				{
					std::wstring com_uuid_utf16{ path[1] };

					std::string username_utf8{ user.username };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(username_utf8));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(com_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:LIKED]->(com) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK);
						}).wait();
					break;
				}
				case routes::meme_id_givelike:
				{
					std::wstring meme_uuid_utf16{ path[1] };

					std::string username_utf8{ user.username };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(username_utf8));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(meme:Meme{id: \\\"");
					neo4j_cypher_utf16.append(meme_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:LIKED]->(meme) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK);
						}).wait();
					break;
				}
				case routes::meme:
				{
					if(
						!json.has_field(L"title")	||
						!json.has_field(L"payload")		||
						!json.has_field(L"payload_type") ||
						!json.has_field(L"tags")
						)
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}

					if(!user.permissions_union.permissions.can_add_memes)
					{
						request.reply(web::http::status_codes::Forbidden);
						return;
					}

					std::string username_utf8{ user.username };

					std::wstring meme_title{json.at(L"title").as_string() };
					std::wstring payload_type{ json.at(L"payload_type").as_string() };
					//std::wstring payload_base64{ json.at(L"payload").as_string() };

					//std::string payload{ requests_base64::decode64(utility::conversions::to_utf8string(payload_base64)) };

					std::wstring neo4j_cypher_add_meme_utf16;
					neo4j_cypher_add_meme_utf16.reserve(256);

					neo4j_cypher_add_meme_utf16.append(L"{\"statements\": [{\"statement\":\"");
					neo4j_cypher_add_meme_utf16.append(L"MATCH (acc:Account{username: \\\"");
					neo4j_cypher_add_meme_utf16.append(utility::conversions::to_string_t(username_utf8));
					neo4j_cypher_add_meme_utf16.append(L"\\\"}) CREATE (acc)-[:AUTHORED]->(meme:Meme{title:\\\"");
					neo4j_cypher_add_meme_utf16.append(meme_title);
					neo4j_cypher_add_meme_utf16.append(L"\\\",payload:\\\"");
					neo4j_cypher_add_meme_utf16.append(json.at(L"payload").as_string());
					neo4j_cypher_add_meme_utf16.append(L"\\\",payload_type:\\\"");
					neo4j_cypher_add_meme_utf16.append(payload_type);
					neo4j_cypher_add_meme_utf16.append(L"\\\",tags: [");

					for (const auto& el : json.at(L"tags").as_array())
					{
						neo4j_cypher_add_meme_utf16.append(L"\\\"");
						neo4j_cypher_add_meme_utf16.append(el.as_string());
						neo4j_cypher_add_meme_utf16.append(L"\\\",");
					}

					neo4j_cypher_add_meme_utf16.pop_back();
					neo4j_cypher_add_meme_utf16.append(L"]}) WITH meme, apoc.create.uuid() AS memeId SET meme.id = memeId SET meme.created_at = timestamp() RETURN meme.id AS memeId\"}]}");

					web::json::value neo4j_cypher_add_meme_json{ web::json::value::parse(neo4j_cypher_add_meme_utf16) };

					web::http::http_request neo4j_cypher_add_meme_request{ web::http::methods::POST };
					neo4j_cypher_add_meme_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_add_meme_request.set_body(neo4j_cypher_add_meme_json);

					//std::string* uuid_path = new std::string;

					/**/
					client.request(neo4j_cypher_add_meme_request).then([&](web::http::http_response response)
						{

							web::json::value tmp = response.extract_json().get();

							web::json::value send_response_json
							{
								tmp.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row")
							};

							//*uuid_path = utility::conversions::to_utf8string(send_response_json.as_string());
							request.reply(web::http::status_codes::Created).wait();
							
						}).wait();

					//std::fstream meme_file{ *uuid_path+".png", std::ios::out | std::ios::binary };

					//meme_file << payload;

					//meme_file.close();

					//delete uuid_path;

					return;
				}
				case routes::meme_id_report:
				{
					std::wstring meme_uuid_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(user.username));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(meme:Meme{id: \\\"");
					neo4j_cypher_utf16.append(meme_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:REPORTED]->(meme) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };
					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK).wait();
						}).wait();
				}
				case routes::meme_id_comment:
				{
					if(!user.permissions_union.permissions.can_add_comments)
					{
						request.reply(web::http::status_codes::Forbidden);
						return;
					}

					if(!json.has_field(L"body"))
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}
					std::wstring comment_body_utf16{ json.at(L"body").as_string() };
					std::wstring meme_uuid_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH (acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(user.username));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH (meme:Meme{id: \\\"");
					neo4j_cypher_utf16.append(meme_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) CREATE (acc)-[:COMMENTED]->(com:Comment{body: \\\"");
					neo4j_cypher_utf16.append(comment_body_utf16);
					neo4j_cypher_utf16.append(L"\\\"})-[:COMMENTED_ON]->(meme) WITH com, apoc.create.uuid() AS comId SET com.id = comId SET com.created_at = timestamp() RETURN com.id AS comId \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };
					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							

							web::json::value tmp = response.extract_json().get();

							web::json::value send_response_json
							{
								tmp.at(L"results").as_array().at(0).as_object()
									.at(L"data").as_array().at(0).as_object().at(L"row").as_array().at(0)
							};
							
							request.reply(web::http::status_codes::Created);
						}).wait();

					break;
				}
				case routes::account_username_givelike:
				{
					std::wstring username_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(user.username));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(acc2:Account{username: \\\"");
					neo4j_cypher_utf16.append(username_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:LIKED]->(acc2) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };
					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK);
						}).wait();

					break;
				}
				case routes::account_username_report:
				{
					std::wstring username_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH(acc:Account{username: \\\"");
					neo4j_cypher_utf16.append(utility::conversions::to_utf16string(user.username));
					neo4j_cypher_utf16.append(L"\\\"}) MATCH(acc2:Account{username: \\\"");
					neo4j_cypher_utf16.append(username_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) MERGE(acc)-[r:REPORTED]->(acc2) \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };
					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					client.request(neo4j_cypher_request).then([&](web::http::http_response response)
						{
							request.reply(web::http::status_codes::OK).wait();
						}).wait();

					break;
				}
				case routes::account_username_ban:
				{
					std::wstring username_utf16{ path[1] };

					if(users.get_user_map().contains(utility::conversions::to_utf8string(username_utf16)))
					{
						users.get_user_map().at(utility::conversions::to_utf8string(username_utf16)).permissions_union = 0;
						users.get_user_map().at(utility::conversions::to_utf8string(username_utf16)).permissions_union.permissions.banned = true;
						request.reply(web::http::status_codes::OK);
					}
					else
					{
						request.reply(web::http::status_codes::NotFound);
					}
					return;
				}
				case routes::account_username_unban:
				{
					if(users.get_user_map().contains(utility::conversions::to_utf8string(path[1])))
					{
						users.get_user_map().at(utility::conversions::to_utf8string(path[1])).permissions_union.permissions.banned = false;
						users.get_user_map().at(utility::conversions::to_utf8string(path[1])).permissions_union = user_map::generate::user();
						request.reply(web::http::status_codes::OK);
					}
					else
					{
						request.reply(web::http::status_codes::NotFound);
					}
				}
				case routes::saveusers:
				{
					users.save_users_to_file(path_to_file);
					request.reply(web::http::status_codes::OK);
					return;
				}
				case routes::loadusers:
				{
					users.load_users_from_file(path_to_file);
					request.reply(web::http::status_codes::OK);
					return;
				}
			default:
				{
					request.reply(web::http::status_codes::NotFound);
					return;
				}

			}
		}
		void handle_patch(web::http::http_request request)
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

			if (!authorized || session_ID == 0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}

			web::json::value json;

			try
			{
				if (request.headers().content_type() == L"application/json")	json = request.extract_json().get();
			}
			catch (std::exception& e)
			{
				std::cout << "error: " << e.what() << std::endl;
				request.reply(web::http::status_codes::BadRequest, e.what());
				return;
			}

			auto& user = session_ID_map.at(session_ID);

			switch(path_to_route_parser(path))
			{
			case routes::preferences:
				{
					if(!json.has_field(L"preferences"))
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}

					request.reply(web::http::status_codes::OK);
					return;
				}
			case routes::comment_id_edit:
				{
					if(!json.has_field(L"body"))
					{
						request.reply(web::http::status_codes::BadRequest);
						return;
					}

					std::wstring comment_uuid_utf16{ path[1] };
					std::wstring comment_body_utf16{ json.at(L"body").as_string() };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH (com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(comment_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) SET com.body = \\\"");
					neo4j_cypher_utf16.append(comment_body_utf16);
					neo4j_cypher_utf16.append(L"\\\"\"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };
				}

			default:
				{
					request.reply(web::http::status_codes::BadRequest);
					return;
				}
			}
		}
		void handle_delete(web::http::http_request request)
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

			if(!authorized || session_ID == 0)
			{
				request.reply(web::http::status_codes::Unauthorized);
				return;
			}

			switch(path_to_route_parser(path))
			{
			case routes::comment_id_delete:
				{
					std::wstring comment_uuid_utf16{ path[1] };

					std::wstring neo4j_cypher_utf16;
					neo4j_cypher_utf16.reserve(256);

					neo4j_cypher_utf16.append(L"{\"statements\": [{\"statement\": \"MATCH (com:Comment{id: \\\"");
					neo4j_cypher_utf16.append(comment_uuid_utf16);
					neo4j_cypher_utf16.append(L"\\\"}) DETACH DELETE com \"}]}");

					web::json::value neo4j_cypher_json{ web::json::value::parse(neo4j_cypher_utf16) };

					web::http::http_request neo4j_cypher_request{ web::http::methods::POST };

					neo4j_cypher_request.headers().add(L"Authorization", L"Basic " + auth_64);
					neo4j_cypher_request.set_body(neo4j_cypher_json);

					/**/
					client.request(neo4j_cypher_request).then([](web::http::http_response response)
						{
							//_DEBUG_WPRINT(response.to_string())
						}).wait();
					//*/
					return;
				}
				default:
				{
					request.reply(web::http::status_codes::BadRequest);
					return;
				}
			}
		}
	};
}