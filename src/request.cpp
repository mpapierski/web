#include <web/request.hpp>

using namespace web;

request::request(http_server_api::http_server_client * client)
{
	char * url = nullptr;
	int r = http_server_client_getinfo(client, http_server_api::HTTP_SERVER_CLIENTINFO_URL, &url);
    std::error_code ec(r, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
    path_ = url;
    // TODO: No way to retrieve method?
	method_ = "GET";
}

std::string const & request::method() const
{
	return method_;
}

std::string const & request::path() const
{
	return path_;
}

int request::verb() const
{
	if (method_ == "GET")
	{
		return GET;
	}
	else if (method_ == "POST")
	{
		return POST;
	}
	else
	{
		return UNKNOWN;
	}
}