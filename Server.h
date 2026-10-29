#pragma once
#include <string>

class Server {
private:
    int serverSocket;
    int port;
    std::string dbPath;
    bool isRunning;

public:
    Server(int serverPort = 43184, const std::string& authDb = "auth_base.txt");
    ~Server();

    void start();
    void stop();
    void handleClient(int clientSocketFd);
};

