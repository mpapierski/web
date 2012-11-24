#include <web/response.hpp>

using namespace web;

response::response()
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