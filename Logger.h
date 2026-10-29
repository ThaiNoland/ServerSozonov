#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger {
private:
    std::string logFilePath;
    std::ofstream logFile;
    std::mutex logMutex;

public:
    Logger(const std::string& path = "server.log");
    ~Logger();
    void writeLog(const std::string& msg, bool isCritical = false);
};

