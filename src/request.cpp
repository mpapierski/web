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
			ss >> method >> path;
		}
	}

	if (method.empty())
	{
		throw std::runtime_error("Found no HTTP verb in the request!");
	}
}

