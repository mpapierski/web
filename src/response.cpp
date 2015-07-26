#include <web/response.hpp>

using namespace web;

response::response()
	: response_(http_server_api::http_server_response_new())
{
	if (!response_)
	{
		throw std::bad_alloc();
	}
}

response::~response()
{
	//
}

void response::begin(http_server_api::http_server_client * client)
{
	int r = http_server_api::http_server_response_begin(client, response_);
	std::error_code ec(r, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}

void response::write_head(int status)
{
	int r = http_server_api::http_server_response_write_head(response_, status);
	std::error_code ec(r, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}
void response::write(const std::string & data)
{
	write(data.c_str(), data.size());
}

void response::write(const char * data, std::size_t size)
{
	int r = http_server_api::http_server_response_write(response_, const_cast<char *>(data), size);
	std::error_code ec(r, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}

/**
 * End current response
 */
void response::end()
{
	int r = http_server_api::http_server_response_end(response_);
	std::error_code ec(r, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}
