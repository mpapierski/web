#include <web/application.hpp>

using namespace web;

request_handler::request_handler(application * app)
	: app_(app)
{
}


int request_handler::message_complete(http_server_api::http_server_client * client)
{
	request req(client);
	response res(client);
	std::string data = app_->process(req, res);

	http_server_api::http_server_response * response = http_server_api::http_server_response_new();
	if (!response)
	{
		return 1;
	}
	int r;
	std::error_code ec;
	r = http_server_api::http_server_response_begin(client, response);
	ec.assign(r, get_http_error_category());
	assert(r == http_server_api::HTTP_SERVER_OK);
	r = http_server_api::http_server_response_write_head(response, 200);
	assert(r == http_server_api::HTTP_SERVER_OK);
	r = http_server_api::http_server_response_write(response, const_cast<char *>(data.c_str()), data.size());
	assert(r == http_server_api::HTTP_SERVER_OK);
	r = http_server_api::http_server_response_end(response);
	assert(r == http_server_api::HTTP_SERVER_OK);
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

std::string application::process(request & req, response & res) throw()
{
	unsigned int result_code = 200;
	view_function_t view = get_route(req.verb(), req.path());
	std::string str; // Site response.
	try
	{
		// Check if specified view exists.
		// If not, throw "404" - view does not exists.
		if (!view)
			throw http_error(404);
		// Run view.
		view(req, res);
		// Generated response.
		str = res.stream().str();
	} catch (web::http_error const & e)
	{
		// Change HTTP result.
		result_code = e.error_code();
		// Generated response
		// (before the exception was raised)
		str = res.stream().str();
	} catch (std::exception const & e)
	{
		// We know what does this error (could) mean.
		result_code = 500;
		// Exception description is our response.
		str = e.what();
	} catch (...)
	{
		// We do not have idea what this error means.
		result_code = 500;
	}
	// Construct a valid HTTP response.
	std::stringstream output;
	output << "HTTP/1.1 " << result_code << " OK\r\n"
		"Content-Type:text/html\r\n"
		"Content-Length: " << str.length() <<
		"\r\n\r\n"
		<< str;
	return output.str();
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