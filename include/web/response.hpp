#if !defined(WEB_RESPONSE_H_INCLUDED_)
#define WEB_RESPONSE_H_INCLUDED_

namespace web {

/**
 * HTTP response.
 */
class response
{
private:
	/**
	 * Socket descriptor.
	 */
	int client_socket_;
public:
	/**
	 * Constructor of HTTP response.
	 * Needs raw socket descriptor to send a valid http response.
	 * @param client_socket Client socket descriptor.
	 */
	response(int client_socket);
	/**
	 * All buffered data is written to the client_socket_ descriptor.
	 */
	~response();
};

} /* /namespace web */

#endif /* WEB_RESPONSE_H_INCLUDED_ */