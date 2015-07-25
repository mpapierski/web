#include <web/response.hpp>

using namespace web;

response::response(http_server_api::http_server_client * client)
	: client_(client)
{
	//
}

response::~response()
{
	//
}

std::stringstream & response::stream()
{
	return stream_;
}