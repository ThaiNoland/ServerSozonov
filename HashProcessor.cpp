#include "HashProcessor.h"
#include <fstream>
#include <sstream>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/cryptlib.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>

HashProc::HashProc(const std::string& dbPath) : authDatabasePath(dbPath) {}

std::string HashProc::getPasswordForUser(const std::string& login) const {
    std::ifstream file(authDatabasePath);
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            size_t delimiterPos = line.find(':');
            if (delimiterPos != std::string::npos) {
                std::string currentLogin = line.substr(0, delimiterPos);
                if (currentLogin == login) {
                    return line.substr(delimiterPos + 1);
                }
            }
        }
    }
    return "";
}

bool HashProc::verifyMD5(const std::string& login, const std::string& saltStr, const std::string& clientHashStr) const {
    std::string password = getPasswordForUser(login);
    if (password.empty()) {
        return false;
    }

    std::string dataToHash = saltStr + password;
    std::string computedHash;

    try {
        CryptoPP::Weak::MD5 md5;
        CryptoPP::StringSource ss(dataToHash, true,
            new CryptoPP::HashFilter(md5,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(computedHash),
                    true 
                )
            )
        );

        return (computedHash == clientHashStr);
    } catch (...) {
        return false;
    }
}

