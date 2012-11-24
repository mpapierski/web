#if !defined(WEB_AUX_UTILS_H_INCLUDED_)
#define WEB_AUX_UTILS_H_INCLUDED_

/**
 * Throws a runtime error exception with a text representation of the error.
 * 
 * Remember to include <cerrno>.
 */
#define __throw_system_exception()                                           \
	do                                                                       \
	{                                                                        \
		int e = errno;                                                       \
		const char * s = std::strerror(e);                                        \
		throw std::runtime_error(s);                                         \
	} while(0)

#endif /* WEB_AUX_UTILS_H_INCLUDED_ */
