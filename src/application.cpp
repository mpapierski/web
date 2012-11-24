#include <web/application.hpp>

using namespace web;

application::application(int argc, char * argv[])
	: args_(argv, argv + argc)
	, server_socket_(-1)
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

std::string application::process(request & req, response & res)
{
	view_function_t view = get_route(req.verb(), req.path());
	view(req, res); // can throw
	std::string const & str = res.stream().str();
	// Construct a valid HTTP response.
	std::stringstream output;
	output << "HTTP/1.1 200 OK\r\n"
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
	server_socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket_ < 0)
	{
		__throw_system_exception();
	}
	struct sockaddr_in serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	int value = 1;
	if (::setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(serv_addr)) < 0)
		__throw_system_exception();
	if (::bind(server_socket_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		__throw_system_exception();
	if (::listen(server_socket_, 1) < 0)
		__throw_system_exception();
	while (true)
	{
		struct sockaddr_in client_addr = {0};
		socklen_t client_len = sizeof(client_addr);
		int client_socket = ::accept(server_socket_, (struct sockaddr *) &client_addr, &client_len);			
		if (client_socket < 0)
			__throw_system_exception();
		// New client connected
		std::vector<char> raw_request(65535); // Need more?
		unsigned int pos = 0;
		while (pos < raw_request.size())
		{
			int n = ::read(client_socket, &raw_request[pos], raw_request.size() - pos);
			if (n < 0)
			{
				__throw_system_exception();
			} else if (n == 0)
			{
				// Client disconnected
				break;
			}
			// Find headers
			pos += n;
			for (unsigned int i = 3; i < pos; i++)
			{
				if ((raw_request[i - 3] == '\r') &&
					(raw_request[i - 2] == '\n') &&
					(raw_request[i - 1] == '\r') &&
					(raw_request[i] == '\n'))
				{
					// Found headers.
					std::string headers(raw_request.begin(), raw_request.end());
					request req(headers); // can throw
					response res(client_socket);
					std::string const & data = process(req, res);
					// Flush raw response (with headers) to client.
					std::vector<char> buf(data.begin(), data.end());
					unsigned int pos = 0;
					while (pos < buf.size())
					{
						int n = ::send(client_socket, &buf[pos], buf.size() - pos, 0);
						if (n < 0)
						{
							std::cerr << "Unable to write data to socket: " << strerror(errno) << std::endl;
							break;
						}
						pos += n;
					}
				}
			}
		}
	}
}