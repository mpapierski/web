#if !defined(WEB_RESPONSE_H_INCLUDED_)
#define WEB_RESPONSE_H_INCLUDED_

#include <sstream>
#include <web/server.hpp>

namespace web {

class response;

/**
 * HTTP response.
 */
class response
{
public:
	/**
	 * Constructor of HTTP response.
	 */
	response();
	/**
	 * All buffered data is written to the client_socket_ descriptor.
	 */
	~response();
	/**
	 * Begin HTTP response
	 */
	void begin(http_server_api::http_server_client * client);
	/**
	 * Start a response by sending a status code
	 */
	void write_head(int status);
	/**
	 * Send string
	 */
	void write(const std::string & data);
	/**
	 * Send buffer
	 */
	void write(const char * data, std::size_t size);
	/**
	 * End current response
	 */
	void end();
private:
	http_server_api::http_server_response * response_;
};

} /* /namespace web */

#endif /* WEB_RESPONSE_H_INCLUDED_ */