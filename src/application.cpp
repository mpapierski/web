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

std::vector<std::string> const & application::args() const
{
	return args_;
}

application::view_map_t const & application::routes() const
{
	return views_;
}
	