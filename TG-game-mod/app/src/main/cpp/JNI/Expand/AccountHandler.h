#pragma once

#include <map>
#include <string>
#include <vector>

class AccountHandler {
private:
    static constexpr uint8_t CONVERT_TABLE[] = {
            0xfe, 0x25, 0xd9, 0xb9, 0xf8,
            0x46, 0xbc, 0x54, 0x84, 0x58,
            0xc8, 0x4e, 0x5d
    };

    struct Account {
        std::string token;
        std::string userHash;
        std::string userIdHash;
        std::string mailAddress;
        std::string gmailAddress;
#ifdef TW
        std::string apHash;
#endif
    };
    std::vector<std::pair<std::string, AccountHandler::Account>> _datas;
public :
    std::pair<bool, AccountHandler::Account> getAccount();

    bool toAccountScene();

    bool switchAccount(std::string &tag);

    bool addAccount(std::string &tag);

    bool deleteAccount(std::string &tag);

    std::vector<std::string> loadAll();

    bool saveAll();

    std::string to_string();

    std::string resolveAttribute(std::string &str);

    std::string resolveAttribute(const char *str);

    int32_t findIndex(std::string &tag);

    std::string convertData(std::string str);
};