// main.cpp
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "Server.h"

namespace po = boost::program_options;

int main(int argc, char** argv) {
    try {
        int port = 43184;
        std::string dbPath = "auth_base.txt";

        po::options_description desc("Доступные параметры");
        desc.add_options()
            ("help,h", "Показать справку")
            ("port,p", po::value<int>(&port)->default_value(43184), "Рабочий порт сервера")
            ("database,d", po::value<std::string>(&dbPath)->default_value("auth_base.txt"), "Путь к базе пользователей");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        
        if (vm.count("help")) {
            std::cout << "Сервер обработки векторов\n" << desc << std::endl;
            return 0;
        }

        po::notify(vm);

        Server server(port, dbPath);
        server.start();

    } catch (const boost::program_options::error& e) {
        std::cerr << "Ошибка интерфейса командной строки: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка сервера: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

