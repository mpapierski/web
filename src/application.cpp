#include <web/application.hpp>

using namespace web;

application::application(int argc, char * argv[])
	: args_(argv, argv + argc)
{
	//
}

std::vector<std::string> const & application::args() const
{
	return args_;
}