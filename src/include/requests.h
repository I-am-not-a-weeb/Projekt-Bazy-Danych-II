#pragma once

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include "usermap.h"

bool isNumber(utility::string_t& str);


enum class routes : unsigned short
{
	empty = 0,
	garbage = 999,
	signup = 1,
	login = 2,


	account = 20,
	account_id = 21,
	account_id_preferences = 22,
	account_id_givelike = 23,
	account_id_comments = 24,


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

inline routes path_to_route_parser(std::vector<utility::string_t>path)
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
					return routes::account_id;
				}
				else															//	/account/:username/?
				{
					if (path.size() == 3 && path[2] == L"comments")				//	/account/:username/comments
					{
						return routes::account_id_comments;
					}
					else if(path.size() == 3 && path[2] == L"preferences")		//	/account/:username/preferences
					{
						return routes::account_id_preferences;
					}
					else if (path.size() == 3 && path[2] == L"givelike")		//	/account/:username/give_like
					{
						return routes::account_id_givelike;
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
				if(path.size() ==2 && isNumber(path[1]))						//	/account/meme/:id
				{
					return routes::meme_id;
				}
				else															//	/account/meme/[garbage]
				{
					if(path.size() == 3 && path[2] == L"comments")				//	/account/meme/:id/comments
					{
						return routes::meme_id_comments;
					}
					else if(path.size() == 3 && path[2] == L"givelike")		//	/account/meme/:id/givelike
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
		else if(!path.empty() && path[0] == L"comment")							//	/comments/?
		{
			if (path.size() == 1)												//	/comments
			{
				return routes::comment;
			}
			else																//	/comments/?
			{
				if (path.size() == 2 && isNumber(path[1]))						//	/comments/:id
				{
					return routes::comment_id;
				}
				else															//	/comments/[garbage]
				{
					if(path.size() == 3 && path[2] == L"givelike")				//	/comments/:id/givelike
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
	std::map<unsigned long, user_map::user&> session_ID_map;
protected:
	RequestProcessor(
		web::http::experimental::listener::http_listener& listener,
		web::http::client::http_client& client) : listener(listener), client(client) {}

	void handle_get(web::http::http_request request)
	{
		//path routing method as per https://github.com/microsoft/cpprestsdk/issues/186#issuecomment-230072639
		std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

		switch (path_to_route_parser(path))
		{
			case routes::empty:

			case routes::signup:

			case routes::login:

			break;
		}

	}
};