#if !defined(WEB_RESPONSE_H_INCLUDED_)
#define WEB_RESPONSE_H_INCLUDED_

#include <sstream>
#include <web/server.hpp>

namespace web {

/**
 * HTTP response.
 */
class response
{
private:
	/**
	 * Buffered data written from view.
	 */
	std::stringstream stream_;
	http_server_api::http_server_client * client_;
public:
	/**
	 * Constructor of HTTP response.
	 */
	response(http_server_api::http_server_client * client);
	/**
	 * All buffered data is written to the client_socket_ descriptor.
	 */
	~response();
	/**
	 * A reference to view's stream.
	 */
	std::stringstream & stream();
};

} /* /namespace web */

#endif /* WEB_RESPONSE_H_INCLUDED_ */