#include <cstdio>
#include <cstring>

#include <fstream>
#include <sstream>
#include <vector>

#include <Logger.h>
#include <Utils.h>
#include <string>
#include <BridgeFunc.h>

#include "AccountHandler.h"
#include "../Data/Path.h"
#include "../../Hook/Data/Defs.h"
#include "../../Hook/Data/Config.h"
#include "../../Il2cpp/Il2cppApi/Field.hpp"
#include "../../Il2cpp/Il2cppApi/Utils.hpp"
#include "../../Il2cpp/Il2cppApi/Helper.hpp"
#include "../../Const/Const_General.h"

bool AccountHandler::toAccountScene()
{
    bool status = false;

    Il2CppThread *il2cpp_thread = il2cpp_thread_attach(Il2CppApi::Helper::il2cpp_domain);

    Il2CppObject *GDM_inst = nullptr;
    Il2CppObject *OGM_inst = nullptr;
    il2cpp_field_static_get_value(GlobalDataManager_inst, &GDM_inst);
    il2cpp_field_static_get_value(OutGameManager_inst, &OGM_inst);

    if (GDM_inst && OGM_inst)
    {
        LOGD("enter");

        GlobalDataManager_CreateSaveData(GDM_inst);
        OutGameManager_ChangeScene(OGM_inst, 18, true);

        status = true;
    }

    il2cpp_thread_detach(il2cpp_thread);

    return status;
}

std::pair<bool, AccountHandler::Account> AccountHandler::getAccount()
{
    bool status = false;

    Il2CppObject *GDM_inst = nullptr;
    il2cpp_field_static_get_value(GlobalDataManager_inst, &GDM_inst);

    LOGD("get CM_inst %p", GDM_inst);

    AccountHandler::Account acc = {};

    if (GDM_inst)
    {
        auto GDM_field = Il2CppApi::Field<Il2CppObject *>(GDM_inst);

        auto m_Account = GDM_field.getField<Il2CppObject *>(gdm_fields.m_Account);
        if (m_Account)
        {

            auto token = m_Account.getStringField(gdm_acc_fields.token);
            if (token.get() != nullptr && token.to_string() != Const::General::empty)
            {

                auto userHash = m_Account.getStringField(gdm_acc_fields.userHash);
                auto userIdHash = m_Account.getStringField(gdm_acc_fields.userIdHash);
                auto mailAddress = m_Account.getStringField(gdm_acc_fields.mailAddress);
                auto gmailAddress = m_Account.getStringField(gdm_acc_fields.gmailAddress);
#ifdef TW
                auto apHash = m_Account.getStringField(gdm_acc_fields.apHash);
#endif

                acc = {
                    .token = token.to_string(),
                    .userHash = userHash.to_string(),
                    .userIdHash = userIdHash.to_string(),
                    .mailAddress = mailAddress.to_string(),
                    .gmailAddress = gmailAddress.to_string(),
#ifdef TW
                    .apHash = apHash.to_string(),
#endif
                };

                status = true;
            }
        }
    }

    return std::make_pair(status, acc);
}

bool AccountHandler::switchAccount(std::string &tag)
{
    bool status = false;

    int32_t idx = this->findIndex(tag);

    if (idx >= 0)
    {
        auto selected_acc = _datas[idx].second;

        Il2CppThread *il2cpp_thread = il2cpp_thread_attach(Il2CppApi::Helper::il2cpp_domain);

        Il2CppObject *GDM_inst = nullptr;
        Il2CppObject *OGM_inst = nullptr;
        il2cpp_field_static_get_value(GlobalDataManager_inst, &GDM_inst);
        il2cpp_field_static_get_value(OutGameManager_inst, &OGM_inst);

        if (GDM_inst && OGM_inst)
        {
            GlobalDataManager_CreateSaveData(GDM_inst);

            auto GDM_field = Il2CppApi::Field<Il2CppObject *>(GDM_inst);
            auto m_Account = GDM_field.getField<Il2CppObject *>(gdm_fields.m_Account);

            auto token = m_Account.getStringField(gdm_acc_fields.token);
            auto userHash = m_Account.getStringField(gdm_acc_fields.userHash);
            auto userIdHash = m_Account.getStringField(gdm_acc_fields.userIdHash);
            auto mailAddress = m_Account.getStringField(gdm_acc_fields.mailAddress);
            auto gmailAddress = m_Account.getStringField(gdm_acc_fields.gmailAddress);
#ifdef TW
            auto apHash = m_Account.getStringField(gdm_acc_fields.apHash);
#endif

            token.set(selected_acc.token);
            userHash.set(selected_acc.userHash);
            userIdHash.set(selected_acc.userIdHash);
            mailAddress.set(selected_acc.mailAddress);
            gmailAddress.set(selected_acc.gmailAddress);
#ifdef TW
            apHash.set(selected_acc.apHash);
#endif

            char buff[0xfff];
#ifdef TW
            std::string save_data_fmt = Const::General::tw_save_data_formatter;
            sprintf(buff, save_data_fmt.c_str(),
                    selected_acc.token.c_str(), selected_acc.userHash.c_str(), selected_acc.userIdHash.c_str(),
                    selected_acc.mailAddress.c_str(), selected_acc.gmailAddress.c_str(), selected_acc.apHash.c_str());
#elif defined(JP)
            std::string save_data_fmt = Const::General::jp_save_data_formatter;
            sprintf(buff, save_data_fmt.c_str(),
                    selected_acc.token.c_str(), selected_acc.userHash.c_str(), selected_acc.userIdHash.c_str(),
                    selected_acc.mailAddress.c_str(), selected_acc.gmailAddress.c_str());
#endif

            SaveData_SetString(6, il2cpp_string_new(buff));

            Cookie_setCookieToken(token.get());

            //Il2CppObject *world_obj = il2cpp_object_new(WorldModel_Param_Class);
            //WorldModel_Param_Create(world_obj);
            //m_CurrentWorld.set(world_obj);

            OutGameManager_ChangeScene(OGM_inst, 18, true);

            status = true;
        }

        il2cpp_thread_detach(il2cpp_thread);
    }

    return status;
}

int32_t AccountHandler::findIndex(std::string &tag)
{
    int32_t len = _datas.size();
    for (int32_t i = 0; i < len; ++i)
    {
        if (_datas[i].first == tag)
        {
            return i;
        }
    }
    return -1;
}

bool AccountHandler::addAccount(std::string &tag)
{
    int32_t idx = this->findIndex(tag);

    if (idx >= 0)
    {
        return false;
    }

    auto acc = this->getAccount();

    if (acc.first)
    {
        _datas.emplace_back(std::make_pair(tag, acc.second));
    }

    return acc.first;
}

bool AccountHandler::deleteAccount(std::string &tag)
{
    int32_t idx = this->findIndex(tag);

    if (idx < 0)
    {
        return false;
    }

    _datas.erase(_datas.cbegin() + idx);

    return true;
}

std::vector<std::string> AccountHandler::loadAll()
{
    std::string outPath = safe_extPath + Const::General::account_file;

    std::vector<std::string> ret_vec;

    std::stringstream buff;
    std::ifstream fin(outPath);
    if (fin && fin.good())
    {
        buff << fin.rdbuf();
        fin.close();

        std::string content = this->convertData(buff.str());
        const char *line_start = content.c_str();
#ifdef TW
        char tag[0xff],
            token[0xff], userHash[0xff], userIdHash[0xff],
            mailAddress[0xff], gmailAddress[0xff], apHash[0xff];
        std::string pattern = Const::General::tw_read_pattern;
        while (sscanf(line_start, pattern.c_str(),
                      tag, token, userHash, userIdHash, mailAddress, gmailAddress, apHash) == 7)
        {
#elif defined(JP)
        char tag[0xff],
            token[0xff], userHash[0xff], userIdHash[0xff],
            mailAddress[0xff], gmailAddress[0xff];
        std::string pattern = Const::General::jp_read_pattern;
        while (sscanf(line_start, pattern.c_str(),
                      tag, token, userHash, userIdHash, mailAddress, gmailAddress) == 6)
        {
#endif
            line_start = strchr(line_start, '\n');

            if (!line_start)
                break;

            ++line_start;

            std::string resolveTag = resolveAttribute(tag);
            _datas.emplace_back(std::make_pair(resolveTag, AccountHandler::Account{
                                                               .token = resolveAttribute(token),
                                                               .userHash = resolveAttribute(userHash),
                                                               .userIdHash = resolveAttribute(userIdHash),
                                                               .mailAddress = resolveAttribute(mailAddress),
                                                               .gmailAddress = resolveAttribute(gmailAddress),
#ifdef TW
                                                               .apHash = resolveAttribute(apHash),
#endif
                                                           }));
            ret_vec.emplace_back(resolveTag);

#ifdef TW
            LOGD("info: %s %s %s %s %s %s %s",
                 tag,
                 token, userHash, userIdHash,
                 mailAddress, gmailAddress, apHash);
#elif defined(JP)
            LOGD("info: %s %s %s %s %s %s",
                 tag,
                 token, userHash, userIdHash,
                 mailAddress, gmailAddress);
#endif
        }
    }

    return ret_vec;
}

bool AccountHandler::saveAll()
{
    std::string outPath = safe_extPath + Const::General::account_file;

    std::ofstream fout(outPath, std::ios::binary);
    if (fout && fout.good())
    {
        fout << this->convertData(this->to_string());
        fout.close();
        return true;
    }
    return false;
}

std::string AccountHandler::to_string()
{
    std::stringstream ret_buff;

    for (auto &data : _datas)
    {
        std::string tag = data.first;
        AccountHandler::Account acc = data.second;
        char buff[0xfff];
#ifdef TW
        std::string fmt = Const::General::tw_write_pattern;
        sprintf(buff, fmt.c_str(),
                resolveAttribute(tag).c_str(),
                resolveAttribute(acc.token).c_str(), resolveAttribute(acc.userHash).c_str(), resolveAttribute(acc.userIdHash).c_str(),
                resolveAttribute(acc.mailAddress).c_str(), resolveAttribute(acc.gmailAddress).c_str(), resolveAttribute(acc.apHash).c_str());
#elif defined(JP)
        std::string fmt = Const::General::jp_write_pattern;
        sprintf(buff, fmt.c_str(),
                resolveAttribute(tag).c_str(),
                resolveAttribute(acc.token).c_str(), resolveAttribute(acc.userHash).c_str(), resolveAttribute(acc.userIdHash).c_str(),
                resolveAttribute(acc.mailAddress).c_str(), resolveAttribute(acc.gmailAddress).c_str());
#endif
        ret_buff << buff;
    }

    return ret_buff.str();
}

std::string AccountHandler::resolveAttribute(std::string &str)
{
    return resolveAttribute(str.c_str());
}

std::string AccountHandler::resolveAttribute(const char *str)
{
    if(!RET_FUNC2(strcmp, Const::General::empty, str)){
        return Const::General::replace_empty_content;
    } else if(!RET_FUNC2(strcmp, Const::General::replace_empty_content, str)) {
        return Const::General::empty;
    }
    return str;
}

std::string AccountHandler::convertData(std::string str)
{
    int32_t enc_len = sizeof(this->CONVERT_TABLE) / sizeof(uint8_t);

    int32_t len = str.size();
    for (int32_t i = 0; i < len; ++i)
    {
        str[i] = str[i] ^ this->CONVERT_TABLE[i % enc_len];
    }

    return str;
}