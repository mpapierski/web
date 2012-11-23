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
	BOOST_CHECK_THROW(app.get("/", [](web::request&, web::response&){}), std::runtime_error);
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
	view(req, res);
	BOOST_REQUIRE_EQUAL(res.stream().str(), "Hello world!");
}

//____________________________________________________________________________//
