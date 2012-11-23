#include <web/application.hpp>

using namespace web;

application::application(int argc, char * argv[])
	: args_(argv, argv + argc)
{
	//

}

void application::get(std::string const & path, view_function_t view)
{
	view_map_t::iterator mount = views_.find(path);
	if (mount == views_.end())
	{
		// Found no views for specified path.
		verb_map_t verbs;
		verbs.emplace(GET, view);
		views_.emplace(path, verbs);
		return;
	}
	// Add new view.
	std::pair<verb_map_t::iterator, bool> route = mount->second.insert(std::make_pair(GET, view));
	if (!route.second)
	{
		throw std::runtime_error("GET view already mounted at specified path.");
	}
}

application::view_function_t application::get_route(int http_verb,
	std::string const & path)
{
	view_map_t::iterator mount = views_.find(path);
	if (mount == views_.end())
		return view_function_t(); // Path not found.
	verb_map_t::iterator route = mount->second.find(http_verb);
	if (route == mount->second.end())
		return view_function_t(); // Method not supported?
	return route->second;
}

void application::process(request & req, response & res)
{
	view_function_t view = get_route(req.verb(), req.path());
	view(req, res);
}

std::vector<std::string> const & application::args() const
{
	return args_;
}

application::view_map_t const & application::routes() const
{
	return views_;
}
	