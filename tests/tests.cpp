#define BOOST_TEST_MODULE UnitTests
#include <boost/test/included/unit_test.hpp>
#include "../src/include/requests.h"

std::ostream& operator<<(std::ostream& os, const routes& obj)
{
	os << static_cast<std::underlying_type<routes>::type>(obj);
	return os;
}

BOOST_AUTO_TEST_SUITE(routing)

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
std::vector<utility::string_t>  meme_1_comments = { L"meme", L"1",L"comments" };
std::vector<utility::string_t>  meme_1_givelike = { L"meme", L"1",L"givelike" };

std::vector<utility::string_t>  ranking = { L"ranking" };
std::vector<utility::string_t>  ranking_users = { L"ranking", L"users" };
std::vector<utility::string_t>  ranking_memes = { L"ranking", L"memes" };
std::vector<utility::string_t>  ranking_comments = { L"ranking", L"comments" };

std::vector<utility::string_t>  comment = { L"comment" };
std::vector<utility::string_t>  comment_1 = { L"comment", L"1" };
std::vector<utility::string_t>  comment_1_givelike = { L"comment", L"1",L"givelike" };

BOOST_AUTO_TEST_CASE(myTestCase)
{
	BOOST_CHECK_EQUAL(path_to_route_parser(path_empty), routes::empty);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_signup), routes::signup);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_login), routes::login);

	BOOST_CHECK_EQUAL(path_to_route_parser(path_account),routes::account);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1), routes::account_id);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_preferences), routes::account_id_preferences);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_comments), routes::account_id_comments);
	BOOST_CHECK_EQUAL(path_to_route_parser(path_account_1_givelike), routes::account_id_givelike);

	BOOST_CHECK_EQUAL(path_to_route_parser(meme), routes::meme);
	BOOST_CHECK_EQUAL(path_to_route_parser(meme_1), routes::meme_id);
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