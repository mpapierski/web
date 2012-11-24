#if !defined(WEB_EXCEPTIONS_H_INCLUDED_)
#define WEB_EXCEPTIONS_H_INCLUDED_

#include <exception>

namespace web {

/**
 * Throw instance of this class to return with specified HTTP response code.
 */
class http_error: public std::exception
{
private:
	// HTTP error code
	unsigned int error_code_;
public:
	/**
	 * Create new instance of exception class.
	 * @param error_code HTTP error code.
	 */
	http_error(unsigned int error_code) throw();
	virtual const char * what() const throw();
	/**
	 * Get http error code.
	 */
	unsigned int error_code() const throw();
};

} /* /namespace web */

#endif /* WEB_EXCEPTIONS_H_INCLUDED_ */
