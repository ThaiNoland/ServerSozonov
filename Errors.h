#pragma once
#include <stdexcept>
#include <string>

class Error : public std::runtime_error {
public:
    Error(const std::string& msg) : std::runtime_error(msg) {}
};

class AuthError : public Error {
public:
    AuthError(const std::string& msg) : Error("Ошибка аутентификации: " + msg) {}
};

class CritServerError : public Error {
public:
    CritServerError(const std::string& msg) : Error("Критическая ошибка сервера: " + msg) {}
};

class InputError : public Error {
public:
    InputError(const std::string& msg) : Error("Ошибка формата данных: " + msg) {}
};

