#if !defined(WEB_REQUEST_H_INCLUDED_)
#define WEB_REQUEST_H_INCLUDED_

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace web {

/**
 * HTTP request class.
 */
class request
{
private:
	/**
	 * Line separated raw header.
	 */
	std::vector<std::string> request_lines_;
public:
	/**
	 * HTTP verb (GET, POST, PATCH, PUT, etc.)
	 */
	std::string method;
	/**
	 * Path for this HTTP request. Like:
	 * VERB /path HTTP/6.9
	 */
	std::string path;
	/**
	 * Construct the request object.
	 *
	 * @param headers Raw HTTP request headers.
	 */
	request(std::string const & headers);
};

} /* /namespace web */

#endif /* WEB_REQUEST_H_INCLUDED_ */
