#define BOOST_TEST_MODULE application
#include <boost/test/included/unit_test.hpp>

#include <web/web.hpp>

using namespace boost;

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_application_constructor)
{
	web::application app1(0, 0);
	BOOST_REQUIRE_EQUAL(app1.args().size(), 0);
	const char * args[] = {"./test"};
	web::application app2(1, const_cast<char**>(args));
	BOOST_REQUIRE_EQUAL(app2.args().size(), 1);
	BOOST_REQUIRE_EQUAL(app2.args()[0], "./test");
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_application_router)
{
	const char * args[] = {"./test"};
	web::application app(1, const_cast<char**>(args));
	BOOST_REQUIRE_EQUAL(app.routes().size(), 0);
	app.get("/", [](web::request&, web::response& res) {
		res.stream() << "Hello world!";
	});
	BOOST_REQUIRE_EQUAL(app.routes().size(), 1);
	BOOST_REQUIRE_EQUAL(app.routes().find("/")->second.size(), 1);
	// Mount again, check if data did not changed.
	BOOST_CHECK_THROW(app.get("/", [](web::request&, web::response&){}), std::logic_error);
	BOOST_REQUIRE_EQUAL(app.routes().size(), 1);
	BOOST_REQUIRE_EQUAL(app.routes().find("/")->second.size(), 1);
	// Process route / with GET verb.
	web::application::view_function_t view = app.get_route(web::GET, "/");
	BOOST_CHECK(view);
	// Path not found.
	BOOST_CHECK(!app.get_route(web::GET, "/path2"));
	// Run the view with fake req/res pair.
	web::request req("GET / HTTP/1.1\r\n\r\n");
	web::response res(-1);
	std::string data = app.process(req, res);
	BOOST_REQUIRE_EQUAL(res.stream().str(), "Hello world!");
	// Raw response:
	const char * raw_response = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length: 12\r\n\r\nHello world!";
	BOOST_REQUIRE_EQUAL(data, raw_response);
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_application_router_wildcard)
{
	const char * args[] = {"./test"};
	web::application app(1, const_cast<char**>(args));
	BOOST_REQUIRE_EQUAL(app.routes().size(), 0);
	// Set wildcard view.
	app.all("/", [](web::request& req, web::response& res) {
		res.stream() << "Method is... " << req.method();
	});
	BOOST_REQUIRE_EQUAL(app.routes().find("/")->second.size(), 1);

	// Navigate to "/" using various methods.

	web::request req_get("GET / HTTP/1.1\r\n\r\n");
	web::response res_get(-1);
	std::string data = app.process(req_get, res_get);
	BOOST_REQUIRE_EQUAL(res_get.stream().str(), "Method is... GET");
	const char * raw_response_get = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length: 16\r\n\r\nMethod is... GET";
	BOOST_REQUIRE_EQUAL(data, raw_response_get);

	web::request req_post("POST / HTTP/1.1\r\n\r\n");
	web::response res_post(-1);
	data = app.process(req_post, res_post);
	BOOST_REQUIRE_EQUAL(res_post.stream().str(), "Method is... POST");
	const char * raw_response_post = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length: 17\r\n\r\nMethod is... POST";
	BOOST_REQUIRE_EQUAL(data, raw_response_post);
}
