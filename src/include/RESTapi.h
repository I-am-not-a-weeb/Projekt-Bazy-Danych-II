#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_headers.h>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/random.hpp>

#include "usermap.h"
#include <fstream>
#include <ctime>

namespace restsdk = web::http;
namespace json = web::json;

std::fstream log_file("log.txt", std::ios::out | std::ios::app);

std::string getTimeStr() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string s(30, '\0');
	std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return s;
}

std::string decode64(const std::string& val) {
	using namespace boost::archive::iterators;
	using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
	return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {return c == '\0';});
}

std::string encode64(const std::string& val) {
	using namespace boost::archive::iterators;
	using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
	auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
	return tmp.append((3 - val.size() % 3) % 3, '=');
}

class RESTServer
{
	web::http::experimental::listener::http_listener listener_;
	const std::wstring uri_,path_userfile;
	user_map::user_map users_{};

	std::map<std::string,user_map::user&> sessions_;	// session_id  -  user
public:
	RESTServer(const std::wstring uri,const std::wstring path_userfile)
		:uri_(uri),
		listener_(restsdk::experimental::listener::http_listener(uri)),
		path_userfile(path_userfile)
	{
		users_.load_users_from_file(utility::conversions::to_utf8string(path_userfile));
	}
	~RESTServer()
	{
		listener_.close();
	}
	void start()
	{
		listener_.open();
	}
};

class rest_logger
{
	std::ofstream log_file_;
	rest_logger(std::string path)
	{
		log_file_.open(path);
	}

	~rest_logger()
	{
		log_file_.close();
	}

	void log_event(std::string event,std::string session_id,user_map::user& user)
	{
		
	}
};

namespace handlers
{
	void request_handler(web::http::http_request& request, std::function<void(web::json::value const&, web::json::value&)> const& action)
	{
		auto answer = web::json::value::object();
		request
			.extract_json()
			.then([&](pplx::task<web::json::value>& task)
				{
					try
					{
						const web::json::value& jvalue = task.get();

						std::string session_id = "TRY_LOG_IN";
						if (request.headers().has(L"Cookie"))
						{
							 session_id = utility::conversions::to_utf8string(request.headers().find(L"Cookie")->second);
						}
						
						log_file << getTimeStr << " R: " << session_id << " " << request.method().c_str() << std::endl;

						//printJSON(jvalue, L"R: ");
						if (!jvalue.is_null())
						{
							action(jvalue, answer);
						}
					}
					catch (web::http::http_exception const& e)
					{
						std::wcout << e.what() << std::endl;
						request.reply(web::http::status_codes::BadRequest, e.what());
					}
				});
			//.wait();
		//printJSON(answer, L"S: ");
		//request.reply(web::http::status_codes::OK, answer);
	}
	void handle_get(web::http::http_request& request)
	{
		std::vector<utility::string_t>path = web::http::uri::split_path(web::http::uri::decode(request.relative_uri().path()));

		if(path.empty())			/// /
		{
			
		}
		else
		{

		}
	}

	void handle_post(web::http::http_request& request)
	{
		
	}

	void handle_put(web::http::http_request& request)
	{
		
	}
}