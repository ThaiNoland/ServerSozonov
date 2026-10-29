#include "Server.h"
#include "Communicator.h"
#include "HashProcessor.h"
#include "Vector.h"
#include "Logger.h"
#include "Errors.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

Server::Server(int serverPort, const std::string& authDb) 
    : serverSocket(-1), port(serverPort), dbPath(authDb), isRunning(false) {}

Server::~Server() {
    stop();
}

void Server::stop() {
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
    isRunning = false;
}

void Server::start() {
    Logger logger;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw CritServerError("Не удалось создать сокет сервера");
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        throw CritServerError("Ошибка привязки bind к порту");
    }

    if (listen(serverSocket, 5) == -1) {
        throw CritServerError("Ошибка listen");
    }

    isRunning = true;
    logger.writeLog("Сервер успешно запущен на порту " + std::to_string(port) + "...");

    while (isRunning) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocketFd = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocketFd >= 0) {
            handleClient(clientSocketFd);
        }
    }
}

void Server::handleClient(int clientSocketFd) {
    Logger logger;
    struct timeval timeout {180, 0};
    setsockopt(clientSocketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    Communicator comm(clientSocketFd);
    HashProc hashProcessor(dbPath);

    std::string login, salt, clientHash;
    
    try {
        if (!comm.receiveAuthMessage(login, salt, clientHash) || 
            !hashProcessor.verifyMD5(login, salt, clientHash)) {
            
            logger.writeLog("Провал аутентификации для пользователя: " + login, true);
            comm.sendResponse("ERR");
            return;
        }

        comm.sendResponse("OK");
        logger.writeLog("Пользователь успешно авторизован: " + login);

        std::vector<float> vectorData;
        if (comm.receiveVectorData(vectorData)) {
            Vector mathVector(vectorData);
            float totalSum = mathVector.calculateSum();
            logger.writeLog("Вычислена сумма элементов вектора. Результат: " + std::to_string(totalSum));
            comm.sendResult(totalSum);
        } else {
            throw InputError("Ошибка при чтении данных вектора");
        }
    } catch (const Error& e) {
        logger.writeLog(e.what(), true);
    } catch (const std::exception& e) {
        logger.writeLog("Неизвестная ошибка сессии: " + std::string(e.what()), true);
    }
}

