#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using boost::asio::ip::tcp;

int main() {
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);  

    try {
        // Ініціалізація контексту вводу/виводу
        boost::asio::io_context io_context;

        // Створення сокета
        tcp::socket socket(io_context);

        // Підключення до сервера (localhost, порт 12345)
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        boost::asio::connect(socket, endpoints);

        std::cout << "Підключено до сервера!" << std::endl;

        // Отримання даних з сервера
        std::vector<char> buffer(1024);  // Буфер для даних
        std::ofstream file("TestDataFile.txt", std::ios::binary); // Відкриття файлу для запису

        if (!file.is_open()) {
            std::cerr << "Помилка: не вдається створити файл received_response.txt" << std::endl;
            return 1;
        }

        // Читання даних з сокета
        boost::system::error_code error;
        size_t length;
        while((length = socket.read_some(boost::asio::buffer(buffer), error)) > 0) {
            file.write(buffer.data(), length); // Запис даних у файл
        }

        if(error && error != boost::asio::error::eof) std::cerr << "Помилка при отриманні даних: " << error.message() << std::endl;
        else std::cout << "Файл успішно отримано та збережено!" << std::endl;

        // Закриваємо файл і з'єднання
        file.close();
        socket.close();
        std::cout << "З'єднання закрито." << std::endl;

    } catch(std::exception& e) {
        std::cerr << "Виняток: " << e.what() << std::endl;
    }

    return 0;
}
