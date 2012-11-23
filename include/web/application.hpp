#if !defined(WEB_APPLICATION_H_INCLUDED_)
#define WEB_APPLICATION_H_INCLUDED_

#include <string>
#include <vector>

namespace web {

/**
 * Main application logic class.
 * Non-copyable.
 */
class application
{
private:
	std::vector<std::string> args_;
public:
	application(application const &) = delete;
	application & operator=(application const &) = delete;

	/**
	 * Application constructor.
	 *
	 * @param argc Argument count.
	 * @param argv Argument values.
	 */
	application(int argc, char * argv[]);

	/**
	 * Args getter.
	 */
	std::vector<std::string> const & args() const;
};

} /* /namespace web */

#endif /* WEB_APPLICATION_H_INCLUDED_ */

