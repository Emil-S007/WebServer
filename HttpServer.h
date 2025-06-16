#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "Session.h"

using boost::asio::ip::tcp;

class HttpServer
{
public:
    HttpServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        startAccept();
    }

private:
    void startAccept() {
        std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& error)
            {
                if (!error)
                {
                    std::cout << "Соединение прошло успешно!" << std::endl;
                    handleRequest(socket);
                }
                else
                {
                    std::cerr << "Ошибка при принятии соединения: " << error.message() << std::endl;
                }
                startAccept();
            });
    }

    void handleRequest(std::shared_ptr<tcp::socket> socket)
    {
        try
        {
            char buffer[1024];
            socket->read_some(boost::asio::buffer(buffer));

            std::string response = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Hello, World!";

            boost::asio::write(*socket, boost::asio::buffer(response));
        }
        catch (std::exception& ex)
        {
            std::cerr << "Ошибка при обработке запроса: " << ex.what() << std::endl;
        };
    }

    tcp::acceptor acceptor_;
};

#endif