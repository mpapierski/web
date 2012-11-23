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
	app.get("/", [](web::request&, web::response&) {

	});
	BOOST_REQUIRE_EQUAL(app.routes().size(), 1);
}
