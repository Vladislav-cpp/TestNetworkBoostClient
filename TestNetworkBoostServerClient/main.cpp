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
        // ����������� ��������� �����/������
        boost::asio::io_context io_context;

        // ��������� ������
        tcp::socket socket(io_context);

        // ϳ��������� �� ������� (localhost, ���� 12345)
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        boost::asio::connect(socket, endpoints);

        std::cout << "ϳ�������� �� �������!" << std::endl;

        // ��������� ����� � �������
        std::vector<char> buffer(1024);  // ����� ��� �����
        std::ofstream file("TestDataFile.txt", std::ios::binary); // ³������� ����� ��� ������

        if (!file.is_open()) {
            std::cerr << "�������: �� ������� �������� ���� received_response.txt" << std::endl;
            return 1;
        }

        // ������� ����� � ������
        boost::system::error_code error;
        size_t length;
        while((length = socket.read_some(boost::asio::buffer(buffer), error)) > 0) {
            file.write(buffer.data(), length); // ����� ����� � ����
        }

        if(error && error != boost::asio::error::eof) std::cerr << "������� ��� �������� �����: " << error.message() << std::endl;
        else std::cout << "���� ������ �������� �� ���������!" << std::endl;

        // ��������� ���� � �'�������
        file.close();
        socket.close();
        std::cout << "�'������� �������." << std::endl;

    } catch(std::exception& e) {
        std::cerr << "�������: " << e.what() << std::endl;
    }

    return 0;
}
