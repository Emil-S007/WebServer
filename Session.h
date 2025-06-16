#ifndef SESSION_H
#define SESSION_H
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket) : socket_(std::move(socket)) {}

	void start()
	{
		readRequest();
	}
private:
    void readRequest() 
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, request_, "\r\n\r\n",
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    handleRequest();
                }
                else {
                    std::cerr << "Ошибка чтения запроса: " << ec.message() << std::endl;
                }
            });
    }

    void handleRequest()
    {
        std::istream request_stream(&request_);
        std::string request_line;
        std::getline(request_stream, request_line);
        std::cout << "Получен запрос: " << request_line << std::endl;
    }

	tcp::socket socket_;
    boost::asio::streambuf request_;
};

#endif