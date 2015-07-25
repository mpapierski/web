#include <web/server.hpp>

namespace web {

http_server_handler::http_server_handler()
	: handler_()
{
	http_server_api::http_server_handler_init(&handler_);
	handler_.on_message_complete = &on_message_complete;
	handler_.on_message_complete_data = this;
}

int http_server_handler::on_message_complete(http_server_api::http_server_client * client, void * data)
{
	return static_cast<http_server_handler *>(data)->message_complete(client);
}

http_server::http_server()
	: server_()
{
	http_server_api::http_server_init(&server_);
}

http_server::~http_server()
{
	http_server_api::http_server_free(&server_);
}

void http_server::start(http_server_handler * handler)
{
	assert(handler);
	handler_.reset(handler);
	int result;
	result = http_server_api::http_server_setopt(&server_, http_server_api::HTTP_SERVER_OPT_HANDLER, handler_->get_raw_pointer());
	std::error_code ec(result, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
	result = http_server_api::http_server_start(&server_);
	ec.assign(result, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}

void http_server::run_forever()
{
	int result = http_server_api::http_server_run(&server_);
	std::error_code ec(result, get_http_error_category());
	if (ec)
	{
		throw std::system_error(ec);
	}
}

}
