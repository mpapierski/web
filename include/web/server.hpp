#if !defined(WEB_SERVER_H_INCLUDED_)
#define WEB_SERVER_H_INCLUDED_

#include <cassert>
#include <string>
#include <system_error>

namespace web {

namespace http_server_api {

extern "C" {
#include "http-server/http-server.h"
}

}

namespace detail {

struct http_error_category
    : std::error_category
{
    const char * name() const noexcept
    {
        return "http_server";
    }
    std::string message(int rc) const
    {
    	auto err = static_cast<http_server_api::http_server_errno>(rc);
        char * str = http_server_api::http_server_errstr(err);
        assert(str);
        return str;
    }
};

}

inline std::error_category & get_http_error_category()
{
    static detail::http_error_category category;
    return category;
}


class http_server_handler
{
public:
    http_server_handler();
    virtual int message_complete(http_server_api::http_server_client * client) = 0;
    inline http_server_api::http_server_handler * get_raw_pointer()
    {
        return &handler_;
    }
private:
	http_server_api::http_server_handler handler_;
    static int on_message_complete(http_server_api::http_server_client * client, void * data);
};

class http_server
{
public:
    http_server();
    ~http_server();
    void start(http_server_handler * handler);
    void run_forever();
private:
    http_server_api::http_server server_;
    std::unique_ptr<http_server_handler> handler_;
};

} /* WEB_SERVER_H_INCLUDED_ */

#endif
