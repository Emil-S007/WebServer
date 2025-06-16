#include <iostream>
#include "HttpServer.h"

int main()
{
	std::setlocale(LC_ALL, "rus");
	try
	{
		std::cout << "Веб-Сервер запущен!" << std::endl;
		boost::asio::io_context io_context;
		HttpServer server(io_context, 8080);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Ошибка: " << e.what() << std::endl;
	}
	return 0;
}