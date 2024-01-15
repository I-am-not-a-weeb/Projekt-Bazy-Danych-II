#define BOOST_TEST_MODULE UnitTests
#include <boost/test/included/unit_test.hpp>
#include "../src/include/requests.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


std::ostream& operator<<(std::ostream& os, const routes& obj)
{
	os << static_cast<std::underlying_type<routes>::type>(obj);
	return os;
}
#include <crtdbg.h>

#include "../src/include/requests.h"

BOOST_AUTO_TEST_SUITE(routing)
	web::http::experimental::listener::http_listener tmp_listener(L"http://localhost:7475");
	web::http::client::http_client tmp_client(L"http://localhost:7475");
	user_map::user_map tmp_user_map;

BOOST_AUTO_TEST_CASE(routesTests)
{
	std::vector<utility::string_t> path_empty = {};
	std::vector<utility::string_t> path_signup = { L"signup" };
	std::vector<utility::string_t> path_login = { L"login" };

	std::vector<utility::string_t>  path_account = { L"account" };
	std::vector<utility::string_t>  path_account_1 = { L"account", L"1" };
	std::vector<utility::string_t>  path_account_1_preferences = { L"account", L"1",L"preferences" };
	std::vector<utility::string_t>  path_account_1_comments = { L"account", L"1",L"comments" };
	std::vector<utility::string_t>  path_account_1_givelike = { L"account", L"1",L"givelike" };

	std::vector<utility::string_t>  meme = { L"meme" };
	std::vector<utility::string_t>  meme_1 = { L"meme", L"1" };
	std::vector<utility::string_t>  meme_1_comment{ L"meme",L"1",L"comment" };
	std::vector<utility::string_t>  meme_1_comments = { L"meme", L"1",L"comments" };
	std::vector<utility::string_t>  meme_1_givelike = { L"meme", L"1",L"givelike" };

	std::vector<utility::string_t>  ranking = { L"ranking" };
	std::vector<utility::string_t>  ranking_users = { L"ranking", L"users" };
	std::vector<utility::string_t>  ranking_memes = { L"ranking", L"memes" };
	std::vector<utility::string_t>  ranking_comments = { L"ranking", L"comments" };

	std::vector<utility::string_t>  comment = { L"comment" };
	std::vector<utility::string_t>  comment_1 = { L"comment", L"1" };
	std::vector<utility::string_t>  comment_1_givelike = { L"comment", L"1",L"givelike" };

	BOOST_CHECK_EQUAL(path_to_route_parser(path_empty), routes::empty);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_signup), routes::signup);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_login), routes::login);

	BOOST_CHECK_EQUAL(path_to_route_parser(path_account),routes::account);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1), routes::account_username);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_preferences), routes::account_username_preferences);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_comments), routes::account_username_comments);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_givelike), routes::account_username_givelike);

	BOOST_CHECK_EQUAL(path_to_route_parser(meme), routes::meme);
	BOOST_CHECK_EQUAL(path_to_route_parser(meme_1), routes::meme_id);
	BOOST_CHECK_EQUAL(path_to_route_parser(meme_1_comment), routes::meme_id_comment);
	BOOST_CHECK_EQUAL(path_to_route_parser(meme_1_comments), routes::meme_id_comments);
	BOOST_CHECK_EQUAL(path_to_route_parser(meme_1_givelike), routes::meme_id_givelike);

	BOOST_CHECK_EQUAL(path_to_route_parser(ranking), routes::ranking);
	BOOST_CHECK_EQUAL(path_to_route_parser(ranking_users), routes::ranking_users);
	BOOST_CHECK_EQUAL(path_to_route_parser(ranking_memes), routes::ranking_memes);
	BOOST_CHECK_EQUAL(path_to_route_parser(ranking_comments), routes::ranking_comments);

	BOOST_CHECK_EQUAL(path_to_route_parser(comment), routes::comment);
	BOOST_CHECK_EQUAL(path_to_route_parser(comment_1), routes::comment_id);
	BOOST_CHECK_EQUAL(path_to_route_parser(comment_1_givelike), routes::comment_id_givelike);

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(handlers)

web::http::experimental::listener::http_listener tmp_listener(L"http://localhost:7475");
web::http::client::http_client tmp_client(L"http://localhost:7474/db/neo4j/tx/commit");
user_map::user_map tmp_user_map;

routing::RequestProcessor tmp_processor(tmp_listener, tmp_client, tmp_user_map);
user_map::user userred{ "user","user",user_map::generate::user() };

/**
BOOST_AUTO_TEST_CASE(no_path)
{


	tmp_user_map.add_user("user", userred);

	web::http::http_request request_good = web::http::http_request();


	request_good.headers().add(L"Cookie", L"session_ID=123456789");
	request_good.set_method(web::http::methods::POST);
	request_good.set_body(L"{\"username\":\"user\",\"password\":\"user\"}");

	tmp_processor.add_session_ID(123456789, userred);

	request_good.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::MisdirectedRequest);
		});

	tmp_processor.handle_get(request_good);
}
//*/
BOOST_AUTO_TEST_CASE(post_signup)
{
	web::http::http_request request_bad1 = web::http::http_request();
	request_bad1.set_request_uri(L"/signup");
	request_bad1.set_method(web::http::methods::POST);
	request_bad1.set_body(L"{\"username\":\"weekerr\",\"password\":\"test\"}", L"application/json");

	web::http::http_request request_bad2 = web::http::http_request();
	request_bad2.set_request_uri(L"/signup");
	request_bad2.set_method(web::http::methods::POST);
	request_bad2.set_body(L"{\"username\":\"weekerr\",\"nickname\":\"weekerABS\",\"password\":\"test\"}", L"application/json");

	web::http::http_request request_bad3 = web::http::http_request();
	request_bad3.set_request_uri(L"/signup");
	request_bad3.set_method(web::http::methods::POST);
	request_bad3.set_body(L"{\"nickname\":\"weekerABS\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_good1 = web::http::http_request();
	request_good1.set_request_uri(L"/signup");
	request_good1.set_method(web::http::methods::POST);
	request_good1.set_body(L"{\"username\":\"weekerr\",\"nickname\":\"weekerABS\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_bad_repeat = web::http::http_request();
	request_bad_repeat.set_request_uri(L"/signup");
	request_bad_repeat.set_method(web::http::methods::POST);
	request_bad_repeat.set_body(L"{\"username\":\"weekerr\",\"nickname\":\"weekerABS\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_good2 = web::http::http_request();
	request_good2.set_request_uri(L"/signup");
	request_good2.set_method(web::http::methods::POST);
	request_good2.set_body(L"{\"username\":\"weekerr1\",\"nickname\":\"weekerABS1\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_good3 = web::http::http_request();
	request_good3.set_request_uri(L"/signup");
	request_good3.set_method(web::http::methods::POST);
	request_good3.set_body(L"{\"username\":\"weekerr2\",\"nickname\":\"weekerABS2\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_good4 = web::http::http_request();
	request_good4.set_request_uri(L"/signup");
	request_good4.set_method(web::http::methods::POST);
	request_good4.set_body(L"{\"username\":\"weekerr3\",\"nickname\":\"weekerABS3\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	web::http::http_request request_good5 = web::http::http_request();
	request_good5.set_request_uri(L"/signup");
	request_good5.set_method(web::http::methods::POST);
	request_good5.set_body(L"{\"username\":\"weekerr4\",\"nickname\":\"weekerABS4\",\"password\":\"test\",\"email\":\"email@gmail.com\"}", L"application/json");

	request_bad1.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::BadRequest);
		});

	request_bad2.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::BadRequest);
		});

	request_bad3.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::BadRequest);
		});

	request_good1.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Created);
		});

	request_bad_repeat.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Conflict);
		});

	tmp_processor.handle_post(request_bad1);
	tmp_processor.handle_post(request_bad2);
	tmp_processor.handle_post(request_bad3);
	tmp_processor.handle_post(request_good1);
	tmp_processor.handle_post(request_good2);
	tmp_processor.handle_post(request_good3);
	tmp_processor.handle_post(request_good4);
	tmp_processor.handle_post(request_good5);
	tmp_processor.handle_post(request_bad_repeat);
}

BOOST_AUTO_TEST_CASE(post_login)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	web::http::http_request request_bad = web::http::http_request();
	request_bad.set_request_uri(L"/login");
	request_bad.set_method(web::http::methods::POST);
	request_bad.set_body(L"{\"username\":\"user\",\"password\":\"test\"}", L"application/json");

	web::http::http_request request_good = web::http::http_request();
	request_good.set_request_uri(L"/login");
	request_good.set_method(web::http::methods::POST);
	request_good.set_body(L"{\"username\":\"weekerr\",\"password\":\"test\"}", L"application/json");

	request_bad.get_response().then([](web::http::http_response response) {
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Unauthorized);
		});

	request_good.get_response().then([](web::http::http_response response) {
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Accepted);
		});

	tmp_processor.handle_post(request_bad);
	tmp_processor.handle_post(request_good);
}

BOOST_AUTO_TEST_CASE(get_account)
{
	std::string username_utf8 = tmp_user_map.get_user_map().begin()->second.username;
	std::wstring username = utility::conversions::to_utf16string(username_utf8);

	unsigned long long session_ID = tmp_processor.get_session_map().begin()->first;

	web::http::http_request request = web::http::http_request();
	request.set_request_uri(L"/account/"+username);
	request.set_method(web::http::methods::GET);
	request.headers().add(L"Cookie", L"session_ID="+std::to_wstring(session_ID));

	request.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);

		});

	tmp_processor.handle_get(request);
}

BOOST_AUTO_TEST_CASE(upload_meme)
{
	std::string username_utf8 = tmp_user_map.get_user_map().begin()->second.username;
	std::wstring username = utility::conversions::to_utf16string(username_utf8);

	unsigned long long session_ID = tmp_processor.get_session_map().begin()->first;

	std::fstream meme_file("a.png", std::ios::in | std::ios::binary);

	std::stringstream meme_payload_utf8;
	meme_payload_utf8 << meme_file.rdbuf();

	std::string payload_base64{ requests_base64::encode64(meme_payload_utf8.str()) };

	std::wstring body;

	body.append(L"{\"title\":\"ale smieszne xd\",\"tags\":[\"humor\",\"witam\"],\"payload_type\":\"png\",");
	body.append(L"\"payload\":\"");
	body.append(utility::conversions::to_utf16string(payload_base64));
	body.append(L"\"}");

	web::json::value request_body_json = web::json::value::parse(body);

	web::http::http_request request = web::http::http_request();
	request.set_method(web::http::methods::POST);
	request.set_request_uri(L"/meme");
	request.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	request.set_body(request_body_json);

	/**/
	auto waiting = request.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Created);
		});
	//*/
	tmp_processor.handle_post(request);

	waiting.wait();

	std::fstream meme_file2("b.png", std::ios::in | std::ios::binary);

	std::stringstream meme_payload2_utf8;
	meme_payload2_utf8 << meme_file2.rdbuf();

	std::string payload_base64_2{ requests_base64::encode64(meme_payload2_utf8.str()) };

	std::wstring body2;

	body2.append(L"{\"title\":\"dobre pomaranczowe xd\",\"tags\":[\"witam\",\"gigot\"],\"payload_type\":\"png\",");
	body2.append(L"\"payload\":\"");
	body2.append(utility::conversions::to_utf16string(payload_base64_2));
	body2.append(L"\"}");

	web::json::value request_body_json2 = web::json::value::parse(body2);

	web::http::http_request request2 = web::http::http_request();
	request2.set_method(web::http::methods::POST);
	request2.set_request_uri(L"/meme");
	request2.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	request2.set_body(request_body_json2);

	/**/
	waiting = request2.get_response().then([](web::http::http_response response) {
		BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Created);
		});
	//*/
	tmp_processor.handle_post(request2);

	waiting.wait();
}
/**/

std::wstring* added_meme_id1 = new std::wstring;

BOOST_AUTO_TEST_CASE(get_memes_of_account_then_meme)
{

	std::wstring username_utf16{ utility::conversions::to_utf16string(tmp_processor.get_session_map().begin()->second.username) };
	unsigned long long session_ID{ tmp_processor.get_session_map().begin()->first };


	web::http::http_request request = web::http::http_request();
	std::wstring path{ L"/account/" + username_utf16 +  L"/memes" };
	request.set_request_uri(path);
	request.set_method(web::http::methods::GET);
	request.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));



	auto waiting = request.get_response().then([&](web::http::http_response response) {
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);
			try
			{
				web::json::value response_body_json = response.extract_json().get();

				std::wcout << response_body_json.serialize() << std::endl;

				*added_meme_id1 = response_body_json.at(0).as_object().at(L"id").as_string();
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		});
	tmp_processor.handle_get(request);

	waiting.wait();

	//delete added_meme_id1;
}

BOOST_AUTO_TEST_CASE(write_comment)
{
	std::wstring username_utf16{ utility::conversions::to_utf16string((tmp_processor.get_session_map().begin())->second.username) };
	unsigned long long session_ID{ tmp_processor.get_session_map().begin()->first };

	web::http::http_request request2 = web::http::http_request();
	std::wstring path2{ L"/meme/" + *added_meme_id1 };
	request2.set_request_uri(path2);
	request2.set_method(web::http::methods::GET);
	request2.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	// ReSharper disable once CppLocalVariableMayBeConst concurrency dependent
	auto waiting = request2.get_response().then([&](web::http::http_response response)
		{
			try
			{
				BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);

				web::json::value response_body_json = response.extract_json().get();

				std::wcout << response_body_json.serialize() << std::endl;

				std::wstring id = response_body_json.at(L"id").as_string();

				BOOST_CHECK_EQUAL(id == *added_meme_id1, true);
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		});
	tmp_processor.handle_get(request2);


	std::wstring body{L"bardzo fajne, podoba mi sie!"};

	web::http::http_request request = web::http::http_request();
	request.set_method(web::http::methods::POST);
	request.set_request_uri(L"/meme/"+*added_meme_id1+L"/comment");
	request.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	web::json::value request_body_json = web::json::value::parse(L"{\"body\":\""+body+L"\"}");
	request.set_body(request_body_json);

	waiting.wait();
	request.get_response().then([](web::http::http_response response)
		{
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::Created);
		});

	tmp_processor.handle_post(request);
}

BOOST_AUTO_TEST_CASE(read_comment_and_give_like)
{
	std::wstring username_utf16{ utility::conversions::to_utf16string((tmp_processor.get_session_map().begin()++)->second.username) };
	unsigned long long session_ID{ tmp_processor.get_session_map().begin()++->first };

	web::http::http_request request1 = web::http::http_request();
	std::wstring path1{ L"/meme/" + *added_meme_id1 +L"/comments"};
	request1.set_request_uri(path1);
	request1.set_method(web::http::methods::GET);
	request1.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	auto waiting = request1.get_response().then([](web::http::http_response response)
		{
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);

			web::json::value response_body_json = response.extract_json().get();

		});
	tmp_processor.handle_get(request1);

	web::http::http_request request2 = web::http::http_request();
	std::wstring path2{ L"/meme/" + *added_meme_id1 + L"/givelike" };
	request2.set_request_uri(path2);
	request2.set_method(web::http::methods::POST);
	request2.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	waiting.wait();

	waiting = request2.get_response().then([](web::http::http_response response)
		{
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);
		});

	tmp_processor.handle_post(request2);
	waiting.wait();
}

BOOST_AUTO_TEST_CASE(ranking_memes)
{
	std::wstring username_utf16{ utility::conversions::to_utf16string((tmp_processor.get_session_map().begin()++)->second.username) };
	unsigned long long session_ID{ tmp_processor.get_session_map().begin()++->first };

	web::http::http_request request2 = web::http::http_request();
	std::wstring path2{ L"/ranking/comments"};
	request2.set_request_uri(path2);
	request2.set_method(web::http::methods::GET);
	request2.headers().add(L"Cookie", L"session_ID=" + std::to_wstring(session_ID));

	auto waiting = request2.get_response().then([](web::http::http_response response)
		{
			BOOST_CHECK_EQUAL(response.status_code(), web::http::status_codes::OK);

			web::json::value response_body_json = response.extract_json().get();


		});

	//
	//
	//
	//
	//
	//
	//
	//



	tmp_processor.handle_get(request2);

}

/**/
/**
BOOST_AUTO_TEST_CASE(rest_on)
{
	web::http::experimental::listener::http_listener tmp_listener(L"http://localhost:7475");
	web::http::client::http_client tmp_client(neo4j_uri);
	user_map::user_map tmp_user_map;

	RequestProcessor tmp_processor(tmp_listener, tmp_client, tmp_user_map);


	while (true);
}
//*/

BOOST_AUTO_TEST_SUITE_END()

//BOOST_AUTO_TEST_SUITE(handler_post)



//BOOST_AUTO_TEST_SUITE_END()