#include "Communicator.h"
#include "Errors.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>

Communicator::Communicator(int socketFd) : clientSocket(socketFd) {}

Communicator::~Communicator() {
    if (clientSocket >= 0) {
        close(clientSocket);
    }
}

bool Communicator::receiveAuthMessage(std::string& login, std::string& salt, std::string& clientHash) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        return false;
    }

    std::string authStr(buffer);
    const size_t saltLen = 16;
    const size_t hashLen = 32;
    
    if (authStr.length() < (saltLen + hashLen)) {
        throw InputError("Некорректный формат строки аутентификации");
    }

    size_t loginLen = authStr.length() - saltLen - hashLen;
    login = authStr.substr(0, loginLen);
    salt = authStr.substr(loginLen, saltLen);
    clientHash = authStr.substr(loginLen + saltLen, hashLen);

    return true;
}

void Communicator::sendResponse(const std::string& status) {
    send(clientSocket, status.c_str(), status.length(), 0);
}

bool Communicator::receiveVectorData(std::vector<float>& vectorData) {
    uint32_t numVectors = 0;
    if (recv(clientSocket, &numVectors, sizeof(numVectors), 0) <= 0) return false;

    for (uint32_t i = 0; i < numVectors; ++i) {
        uint32_t vectorLen = 0;
        if (recv(clientSocket, &vectorLen, sizeof(vectorLen), 0) <= 0) return false;

        std::vector<float> currentVector(vectorLen);
        size_t totalBytes = vectorLen * sizeof(float);
        size_t bytesReceived = 0;
        char* dataPtr = reinterpret_cast<char*>(currentVector.data());

        while (bytesReceived < totalBytes) {
            ssize_t rc = recv(clientSocket, dataPtr + bytesReceived, totalBytes - bytesReceived, 0);
            if (rc <= 0) return false;
            bytesReceived += rc;
        }
        vectorData.insert(vectorData.end(), currentVector.begin(), currentVector.end());
    }
    return true;
}

void Communicator::sendResult(float result) {
    send(clientSocket, &result, sizeof(result), 0);
}

