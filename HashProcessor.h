#pragma once
#include <string>

class HashProc {
private:
    std::string authDatabasePath;
    std::string getPasswordForUser(const std::string& login) const;

public:
    HashProc(const std::string& dbPath = "auth_base.txt");

    bool verifyMD5(const std::string& login, const std::string& saltStr, const std::string& clientHashStr) const;
};

