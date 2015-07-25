#define BOOST_TEST_MODULE request
#include <boost/test/included/unit_test.hpp>

#include <string>

#include <web/web.hpp>

using namespace boost;

web::http_server_api::http_server server;
web::http_server_api::http_server_socket_t sock;
web::http_server_api::http_server_handler handler;
web::http_server_api::http_server_client * client = web::http_server_api::http_server_new_client(&server, sock, &handler);

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_request_constructor)
{
	web::request req(client);
}

//____________________________________________________________________________//
