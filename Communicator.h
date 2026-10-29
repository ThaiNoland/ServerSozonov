#pragma once
#include <string>
#include <vector>

class Communicator {
private:
    int clientSocket;

public:
    Communicator(int socketFd);
    ~Communicator();

    bool receiveAuthMessage(std::string& login, std::string& salt, std::string& clientHash);

    void sendResponse(const std::string& status);

    bool receiveVectorData(std::vector<float>& vectorData);

    void sendResult(float result);
};

