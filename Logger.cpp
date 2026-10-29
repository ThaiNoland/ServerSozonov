#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& path) : logFilePath(path) {
    logFile.open(logFilePath, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::writeLog(const std::string& msg, bool isCritical) {
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    
    std::string prefix = isCritical ? " [CRITICAL] " : " [INFO] ";
    std::string logLine = ss.str() + prefix + msg;

    std::cout << logLine << std::endl;
    if (logFile.is_open()) {
        logFile << logLine << std::endl;
    }
}

