#include <web/application.hpp>

using namespace web;

request_handler::request_handler(application * app)
	: app_(app)
{
}


int request_handler::message_complete(http_server_api::http_server_client * client)
{
	request req(client);
	response res;
	unsigned int result_code = 200;
	auto view = app_->get_route(req.verb(), req.path());
	
	res.begin(client);
	if (!view)
	{
		res.write_head(404);
		res.write("Not found");
		res.end();
		return 0;
	}
	res.write_head(200);
	try
	{
		view(req, res);
	}
	catch (...)
	{
		res.end();
		throw;
	}
	return 0;
}

application::application(int argc, char * argv[])
	: args_(argv, argv + argc)
{
	//
}

void application::mount_route(int verb, std::string const & path, view_function_t view)
{
	view_map_t::iterator mount = views_.find(path);
	if (mount == views_.end())
	{
		// Found no views for specified path.
		verb_map_t verbs;
		verbs.insert(std::make_pair(verb, view));
		views_.insert(std::make_pair(path, verbs));
		return;
	}
	// Add new view.
	std::pair<verb_map_t::iterator, bool> route = mount->second.insert(std::make_pair(verb, view));
	if (!route.second)
	{
		throw std::logic_error("View already mounted at specified path (" + path + ").");
	}
}

void application::get(std::string const & path, view_function_t view)
{
	mount_route(GET, path, view);
}

void application::post(std::string const & path, view_function_t view)
{
	mount_route(POST, path, view);
}

void application::all(std::string const & path, view_function_t view)
{
	mount_route(WILDCARD, path, view);
}

application::view_function_t application::get_route(int http_verb,
	std::string const & path)
{
	view_map_t::iterator mount = views_.find(path);
	if (mount == views_.end())
		return view_function_t(); // Path not found.
	verb_map_t::iterator route = mount->second.find(http_verb);
	if (route == mount->second.end())
	{
		route = mount->second.find(WILDCARD);
		if (route == mount->second.end())
			return view_function_t(); // Method not supported?
	}
	return route->second;
}

std::vector<std::string> const & application::args() const
{
	return args_;
}

application::view_map_t const & application::routes() const
{
	return views_;
}

void application::listen(unsigned short port, const char * address)
{
	http_server server;
	server.start(new request_handler(this));
	server.run_forever();
}