#include <web/request.hpp>

using namespace web;

request::request(std::string const & headers)
{
	std::stringstream ss(headers);
	std::string line;
	while (std::getline(ss, line))
	{
		if ((line.find("GET") == 0) ||
			(line.find("POST") == 0))
		{
			std::stringstream ss(line);
			ss >> method_ >> path_;
		}
	}

	if (method_.empty())
	{
		throw std::runtime_error("Found no HTTP verb in the request!");
	}
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