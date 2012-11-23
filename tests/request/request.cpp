#define BOOST_TEST_MODULE request
#include <boost/test/included/unit_test.hpp>

#include <string>

#include <web/web.hpp>

using namespace boost;

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_request_simple_header_get)
{
	std::string header = "GET / HTTP/1.1\r\n\r\n";
	web::request req(header);
	BOOST_CHECK_EQUAL(req.method, "GET");
	BOOST_CHECK_EQUAL(req.path, "/");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_request_simple_header_post)
{
	std::string header = "POST /address HTTP/1.1\r\n\r\n";
	web::request req(header);
	BOOST_CHECK_EQUAL(req.method, "POST");
	BOOST_CHECK_EQUAL(req.path, "/address");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_request_invalid_header_no_verb)
{
	std::string header;
	BOOST_CHECK_THROW((web::request(header)), std::runtime_error);
}

//____________________________________________________________________________//
