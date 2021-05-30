#include <vector>
#include <random>
#include <map>

#include <Logger.h>
#include <Seed.h>
#include <BridgeFunc.h>

#include "HookFuncs.h"
#include "Data/Config.h"
#include "Data/AppInfo.h"
#include "Data/Defs.h"
#include "../Il2cpp/il2cpp-appdata.h"
#include "../Il2cpp/Il2cppApi/Array.hpp"
#include "../Il2cpp/Il2cppApi/List.hpp"
#include "../Il2cpp/Il2cppApi/Field.hpp"
#include "../Il2cpp/Il2cppApi/String.hpp"
#include "../Il2cpp/Il2cppApi/Utils.hpp"
#include "../Const/Const_Klass.h"
#include "../Const/Const_Method.h"
#include "../Const/Const_Field.h"
#include "../Const/Const_General.h"

#define BYTE int8_t

std::vector<int32_t> XorIntListToVector(Il2CppApi::Il2CppList *il2cpp_xorint_list)
{
    std::vector<int32_t> vec;

    auto xorint_list = Il2CppApi::List<Il2CppObject *>(il2cpp_xorint_list);
    for (int32_t i = 0; i < xorint_list.size(); ++i)
    {
        auto id = XorInt_Get(xorint_list.data().get(i));
        vec.emplace_back(id);
    }

    return vec;
}

RankParam Ranking(Il2CppApi::Field<Il2CppObject *> &IGM_field)
{
    LOGD("Ranking");

    RankParam param = {};

    auto quest = IGM_field.getField<Il2CppObject *>(igm_fields.generate);

    auto ranking_event = quest.getField<Il2CppObject *>(qp_fields.rankingEvent);
    if (ranking_event)
    {
        auto _ghosts = ranking_event.getField<Il2CppArray *>(qr_fields.ghosts);

        if (_ghosts)
        {
            int32_t min_total_time = -1;
            auto ghosts = Il2CppApi::Array<Il2CppObject *>(_ghosts.get());
            for (int32_t i = 0; i < ghosts.length(); ++i)
            {
                auto ghost = Il2CppApi::Field<Il2CppObject *>(*ghosts.getAddr(i));
                if (!ghost)
                    continue;

                auto _sTimes = ghost.getField<Il2CppArray *>(rank_ghost_fields.sTimes);
                if (_sTimes)
                {
                    std::vector<int32_t> param_time;
                    int32_t total_time = -1;
                    auto sTimes = Il2CppApi::Array<int32_t>(_sTimes.get());
                    for (int32_t j = 0; j < sTimes.length(); ++i)
                    {
                        int32_t time = sTimes.get(j);
                        total_time += time;
                        param_time.emplace_back(time);
                        LOGD("time -> %d", time);
                    }
                    if (min_total_time < 0 || total_time < min_total_time)
                    {
                        min_total_time = total_time;
                        param.ranking_time_list = param_time;
                    }
                }
            }
        }
    }

    return param;
}

void inject_one(std::vector<int32_t> &vec, int32_t MAX_)
{
    if (Config::Injection_Num == 0 || Config::Injection_Id == 0)
        return;
    for (int i = 0; i < std::min(Config::Injection_Num, MAX_); i++)
    {
        vec.emplace_back(Config::Injection_Id);
    }
}

void inject_all(int type, DropParam &param)
{
    const int32_t MAX_ITEM = 1000;
    const int32_t MAX_TREASURE = 100;
    const int32_t MAX_PASSIVE_ITEM = 1000;
    const int32_t MAX_WEAPON = 100;
    const int32_t MAX_ORNAMENT = 100;
    const int32_t MAX_CARD = 100;
    const int32_t MAX_RANDOM_ACCESSORY = 10;
    const int32_t MAX_PARAM_BOX = 1000;
    const int32_t MAX_DESTROY_EMENY = 40;
    const int32_t MAX_DESTROY_OBJECT = 40;
    const int32_t MAX_OPEN_TREASURE = 40;

    switch (type)
    {
    case ItemType::Item:
        LOGD("Enter Inject Item");
        inject_one(param.item_list, MAX_ITEM);
        break;
    case ItemType::Treasure:
        LOGD("Enter Inject Treasure");
        inject_one(param.sp_item_list, MAX_TREASURE);
        break;
    case ItemType::PassiveItem:
        LOGD("Enter Inject PassiveItem");
        inject_one(param.passive_item_list, MAX_PASSIVE_ITEM);
        break;
    case ItemType::Weapon:
        LOGD("Enter Inject Weapon");
        inject_one(param.weapon_list, MAX_WEAPON);
        break;
    case ItemType::Ornament:
        LOGD("Enter Inject Ornament");
        inject_one(param.ornament_list, MAX_ORNAMENT);
        break;
    case ItemType::Card:
        LOGD("Enter Inject Card");
        inject_one(param.card_list, MAX_CARD);
        break;
    case ItemType::Accessory:
        LOGD("Enter Inject Accessory");
        inject_one(param.random_drop_list, MAX_RANDOM_ACCESSORY);
        break;
    case ItemType::ParamBox:
        LOGD("Enter Inject ParamBox");
        inject_one(param.param_box_list, MAX_PARAM_BOX);
        break;
    case ItemType::DestroyEnemy:
        LOGD("Enter Inject DestroyEnemy");
        inject_one(param.enemy_list, MAX_DESTROY_EMENY);
        break;
    case ItemType::DestroyObject:
        LOGD("Enter Inject DestroyObject");
        inject_one(param.object_list, MAX_DESTROY_OBJECT);
        break;
    case ItemType::OpenTreasure:
        LOGD("Enter Inject OpenTreasure");
        inject_one(param.treasure_list, MAX_OPEN_TREASURE);
        break;
    }
}

void Drop(Il2CppApi::Il2CppList *dropInfos, DropParam &param, DropType type, int64_t type_id)
{
    auto dropInfoList = Il2CppApi::List<Il2CppObject *>(dropInfos);
    for (int32_t k = 0; k < dropInfoList.size(); ++k)
    {
        auto drop = Il2CppApi::Field<Il2CppObject *>(*dropInfoList.data().getAddr(k));
        if (!drop)
            continue;

        auto id = drop.getField<int32_t>(drop_fields.id).get();
        auto dropId = drop.getField<int32_t>(drop_fields.dropId).get();
        auto dropNum = drop.getField<int32_t>(drop_fields.dropNum).get();
        auto spDropId = drop.getField<int32_t>(drop_fields.spDropId).get();
        auto treasureId = drop.getField<int32_t>(drop_fields.treasureId).get();
        auto randomDropId = drop.getField<int32_t>(drop_fields.randomDropGroupId).get();
        auto cardId = drop.getField<int32_t>(drop_fields.cardId).get();
        auto cardNum = drop.getField<int32_t>(drop_fields.cardNum).get();
        auto weaponId = drop.getField<int32_t>(drop_fields.weaponId).get();
        auto ornamentId = drop.getField<int32_t>(drop_fields.ornamentId).get();
        auto itemId = drop.getField<int32_t>(drop_fields.itemId).get();
        auto itemNum = drop.getField<int32_t>(drop_fields.itemNum).get();
        auto passiveItemId = drop.getField<int32_t>(drop_fields.passiveItemId).get();
        auto addScoreValue = drop.getField<int32_t>(drop_fields.addScoreValue).get();
        auto conditionType = drop.getField<int32_t>(drop_fields.conditionType).get();

        if (addScoreValue > 0)
        {
            param.enemyValue += addScoreValue;
        }

        switch (dropId)
        {
        case 2:
            param.gold += dropNum;
            break;
        case 3:
            param.soul += dropNum;
            break;
        case 102:
            param.enemyValue += addScoreValue;

            if (type == DropType::treasure)
            {
                type_id = id;
            }

            param.stageDropData.push_back({type,
                                           type_id,
                                           conditionType});
            break;
        }

        if (id > 0)
        {
            int32_t drop_max = 1;
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.treasure_list.emplace_back(id);
            }
        }

        /*if(spDropId > 0) {
            int32_t drop_max = std::max(1, dropNum);
            for(int32_t i = 0; i < drop_max; ++i) {
                param->sp_item_list.emplace_back(spDropId);
            }
        }*/

        if (treasureId > 0)
        {
            int32_t drop_max = 1;
            if (spDropId > 0)
            {
                for (int32_t i = 0; i < drop_max; ++i)
                {
                    param.sp_item_list.emplace_back(treasureId);
                }
            }
            else
            {
                for (int32_t i = 0; i < drop_max; ++i)
                {
                    param.treasure_list.emplace_back(treasureId);
                }
            }
        }

        if (randomDropId > 0)
        {
            int32_t drop_max = std::max(1, dropNum);
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.random_drop_list.emplace_back(randomDropId);
            }
        }

        if (cardId > 0)
        {
            int32_t drop_max = std::max(1, std::max(dropNum, cardNum));
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.card_list.emplace_back(cardId);
            }
        }

        if (weaponId > 0)
        {
            int32_t drop_max = std::max(1, dropNum);
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.weapon_list.emplace_back(weaponId);
            }
        }

        if (ornamentId > 0)
        {
            int32_t drop_max = std::max(1, dropNum);
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.ornament_list.emplace_back(ornamentId);
            }
        }

        if (itemId > 0)
        {
            int32_t drop_max = std::max(1, std::max(itemNum, dropNum));
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.item_list.emplace_back(itemId);
            }
        }

        if (passiveItemId > 0)
        {
            int32_t drop_max = std::max(1, dropNum);
            for (int32_t i = 0; i < drop_max; ++i)
            {
                param.passive_item_list.emplace_back(passiveItemId);
            }
        }

        LOGD("drop -> id: %d, dropId: %d, dropNum: %d, spDropId: %d, treasureId: %d, randomDropId: %d, cardId: %d, cardNum: %d, weaponId: %d, ornamentId: %d, itemId: %d, itemNum: %d, passiveItemId: %d",
             id, dropId, dropNum, spDropId, treasureId, randomDropId, cardId, cardNum, weaponId, ornamentId, itemId, itemNum, passiveItemId);
    }
}

void DropScore(DropParam &param, int32_t key, int32_t val1, int32_t val2 = 0)
{
    switch (key)
    {
    case 0:
        break;
    case 1:
        param.enemyTime += val1 + val2;
        break;
    case 9:
        param.enemyValue += val1 + val2;
        break;
    default:
        param.enemyValue += val1 + val2;
        break;
    }
}

void DropAll(Il2CppApi::Field<Il2CppObject *> &IGM, DropParam &param)
{
    LOGD("DropAll");

    param.isDrop = true;

    auto quest = IGM.getField<Il2CppObject *>(igm_fields.generate);

    auto timeAttackFlag = quest.getField<int32_t>(qp_fields.timeAttackFlag);
    auto timeAttackVal = quest.getField<int32_t>(qp_fields.timeAttackVal);
    auto questGameType = quest.getField<int32_t>(qp_fields.questGameType);
    auto questGameVal = quest.getField<int32_t>(qp_fields.questGameVal);
    auto questGameVal2 = quest.getField<int32_t>(qp_fields.questGameVal2);
    auto totalFloor = quest.getField<int32_t>(qp_fields.totalFloor);

    param.timeAttackFlag = timeAttackFlag.get();
    param.timeAttackVal = timeAttackVal.get();
    param.questGameType = questGameType.get();
    param.questGameVal = questGameVal.get();
    param.questGameVal2 = questGameVal2.get();
    param.totalFloor = totalFloor.get();

    auto _scoreRankList = quest.getField<Il2CppApi::Il2CppList *>(qp_fields.scoreRankList);
    auto scoreRankList = Il2CppApi::List<Il2CppObject *>(_scoreRankList.get());
    int32_t max_score_rank = -1;

    for (int32_t i = 0; i < scoreRankList.size(); ++i)
    {
        auto score_rank = Il2CppApi::Field<Il2CppObject *>(*scoreRankList.data().getAddr(i));
        if (!score_rank)
            continue;

        int32_t rank = score_rank.getField<int32_t>(score_border_fields.rank).get();
        if (rank > max_score_rank)
        {
            max_score_rank = rank;
            param.score = score_rank.getField<int32_t>(score_border_fields.score).get();
        }
    }

    auto _timeRankList = quest.getField<Il2CppApi::Il2CppList *>(qp_fields.timeRankList);
    auto timeRankList = Il2CppApi::List<Il2CppObject *>(_timeRankList.get());
    int32_t max_time_rank = -1;

    for (int32_t i = 0; i < timeRankList.size(); ++i)
    {
        auto time_rank = Il2CppApi::Field<Il2CppObject *>(*timeRankList.data().getAddr(i));
        if (!time_rank)
            continue;

        int32_t rank = time_rank.getField<int32_t>(time_border_fields.rank).get();
        if (rank > max_time_rank)
        {
            max_time_rank = rank;
            param.judgeTime = time_rank.getField<int32_t>(time_border_fields.judgeTime).get();
        }
    }

    auto _stageList = quest.getField<Il2CppApi::Il2CppList *>(qp_fields.stageDatas);
    auto stageList = Il2CppApi::List<Il2CppObject *>(_stageList.get());

    for (int32_t i = 0; i < stageList.size(); ++i)
    {
        auto stage = Il2CppApi::Field<Il2CppObject *>(*stageList.data().getAddr(i));
        if (!stage)
            continue;

        std::map<int32_t, QuestGenerateModel_Stage_EnemyParam_Fields> enemyParams;
        auto _enemy_param_List = stage.getField<Il2CppApi::Il2CppList *>(qs_fields.stageEnemyParams);
        auto enemy_param_List = Il2CppApi::List<Il2CppObject *>(_enemy_param_List.get());

        for (int32_t j = 0; j < enemy_param_List.size(); ++j)
        {
            auto enemy_param = Il2CppApi::Field<Il2CppObject *>(*enemy_param_List.data().getAddr(j));
            if (!enemy_param)
                continue;

            LOGD("enemy_param ->");

            auto smonId_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.smonId);
            if (smonId_obj)
            {
                int32_t smonId = XorInt_Get(smonId_obj.get());

                if (smonId)
                {
                    QuestGenerateModel_Stage_EnemyParam_Fields ep = {};

                    auto hp_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.hp);
                    if (hp_obj)
                    {
                        ep.hp = XorLong_Get(hp_obj.get());
                        LOGD("hp: %lld", XorLong_Get(hp_obj.get()));
                    }

                    auto gold_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.gold);
                    if (gold_obj)
                    {
                        ep.gold = XorInt_Get(gold_obj.get());
                        LOGD("gold: %d", XorInt_Get(gold_obj.get()));
                    }

                    auto exp_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.exp);
                    if (exp_obj)
                    {
                        ep.exp = XorInt_Get(exp_obj.get());
                        LOGD("exp: %d", XorInt_Get(exp_obj.get()));
                    }

                    auto paramBoxId_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.paramBoxId);
                    if (paramBoxId_obj)
                    {
                        int32_t paramBoxId = XorInt_Get(paramBoxId_obj.get());
                        if (paramBoxId > 0)
                        {
                            ep.paramBoxId = XorInt_Get(paramBoxId_obj.get());
                            LOGD("paramBoxId: %d", XorInt_Get(paramBoxId_obj.get()));
                        }
                    }

                    auto scoreKey1_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.scoreKey1);
                    if (scoreKey1_obj)
                    {
                        ep.scoreKey1 = XorInt_Get(scoreKey1_obj.get());
                        LOGD("scoreKey1: %d", XorInt_Get(scoreKey1_obj.get()));

                        auto scoreVal1_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.scoreVal1);
                        if (scoreVal1_obj)
                        {
                            ep.scoreVal1 = XorInt_Get(scoreVal1_obj.get());
                            LOGD("scoreVal1: %d", XorInt_Get(scoreVal1_obj.get()));
                        }

                        auto scoreVal2_obj = enemy_param.getField<Il2CppObject *>(enemy_param_fields.scoreVal2);
                        if (scoreVal2_obj)
                        {
                            ep.scoreVal2 = XorInt_Get(scoreVal2_obj.get());
                            LOGD("scoreVal2: %d", XorInt_Get(scoreVal2_obj.get()));
                        }
                    }

                    enemyParams[smonId] = ep;
                }
            }
        }

        auto _enemy_List = stage.getField<Il2CppApi::Il2CppList *>(qs_fields.stageEnemies);
        auto enemy_List = Il2CppApi::List<Il2CppObject *>(_enemy_List.get());
        for (int32_t j = 0; j < enemy_List.size(); ++j)
        {
            auto enemy = Il2CppApi::Field<Il2CppObject *>(*enemy_List.data().getAddr(j));
            if (!enemy)
                continue;

            int32_t smonId = enemy.getField<int32_t>(enemy_fields.smonId).get();
            auto ep = enemyParams[smonId];

            param.totalDamage += ep.hp;
            param.gold += ep.gold;
            param.soul += ep.exp;

            DropScore(param, ep.scoreKey1, ep.scoreVal1, ep.scoreVal2);

            if (ep.paramBoxId > 0)
            {
                param.param_box_list.emplace_back(ep.paramBoxId);
            }

            auto id = enemy.getField<int32_t>(enemy_fields.id).get();
            auto raidBoss = enemy.getField<int32_t>(enemy_fields.raidBoss).get();

            LOGD("enemy -> id: %d, raidBoss: %d, gold: %d, exp: %d, paramBoxId: %d", id, raidBoss, ep.gold, ep.exp, ep.paramBoxId);

            param.enemy_list.emplace_back(id);

            if (raidBoss)
                param.isRaidBoss = true;

            auto dropInfos = enemy.getField<Il2CppApi::Il2CppList *>(enemy_fields.dropInfos);
            Drop(dropInfos.get(), param, DropType::enemy, id);
        }

        auto _tresure_box_List = stage.getField<Il2CppApi::Il2CppList *>(qs_fields.stageTreasures);
        auto tresure_box_List = Il2CppApi::List<Il2CppObject *>(_tresure_box_List.get());
        for (int32_t j = 0; j < tresure_box_List.size(); ++j)
        {
            auto treasure_box = Il2CppApi::Field<Il2CppObject *>(*tresure_box_List.data().getAddr(j));
            if (!treasure_box)
                continue;

            auto scoreKey1 = treasure_box.getField<int32_t>(treasure_box_fields.scoreKey1).get();
            auto scoreVal1 = treasure_box.getField<int32_t>(treasure_box_fields.scoreVal1).get();

            DropScore(param, scoreKey1, scoreVal1);
            LOGD("treasure_box ->");

            auto dropInfos = treasure_box.getField<Il2CppApi::Il2CppList *>(treasure_box_fields.dropInfos);
            Drop(dropInfos.get(), param, DropType::treasure, -1);
        }

        auto _object_List = stage.getField<Il2CppApi::Il2CppList *>(qs_fields.stageObjects);
        auto object_List = Il2CppApi::List<Il2CppObject *>(_object_List.get());
        for (int32_t j = 0; j < object_List.size(); ++j)
        {
            auto object = Il2CppApi::Field<Il2CppObject *>(*object_List.data().getAddr(j));
            if (!object)
                continue;

            auto id = object.getField<int32_t>(object_fields.id).get();
            auto scoreKey1 = object.getField<int32_t>(object_fields.scoreKey1).get();
            auto scoreVal1 = object.getField<int32_t>(object_fields.scoreVal1).get();

            DropScore(param, scoreKey1, scoreVal1);

            LOGD("object -> id: %d", id);
            param.object_list.emplace_back(id);

            auto dropInfos = object.getField<Il2CppApi::Il2CppList *>(object_fields.dropInfos);
            Drop(dropInfos.get(), param, DropType::object, id);
        }
    }

    auto _paramBoxList = quest.getField<Il2CppArray *>(qp_fields.paramBoxs);
    auto paramBoxList = Il2CppApi::Array<Il2CppObject *>(_paramBoxList.get());
    for (int32_t i = 0; i < paramBoxList.length(); ++i)
    {
        auto paramBox = Il2CppApi::Field<Il2CppObject *>(*paramBoxList.getAddr(i));
        if (!paramBox)
            continue;

        auto boxId_obj = paramBox.getField<Il2CppObject *>(param_box_fields.boxId);
        if (boxId_obj)
        {
            param.param_box_list.emplace_back(XorInt_Get(boxId_obj.get()));
            LOGD("boxId => %d", XorInt_Get(boxId_obj.get()));
        }
    }
}

void SetPlusResult(Il2CppApi::Il2CppList *il2cpp_list, std::vector<int32_t> &result, int32_t rate, const char *debug_info = "")
{
    const MethodInfo *method = RET_FUNC3(il2cpp_class_get_method_from_name, il2cpp_list->klass, Const::Method::Add, 1);

    for (auto id : result)
    {
        Il2CppObject *il2cpp_object = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_object, id);
        void *params[] = {il2cpp_object};
        for (int32_t i = 0; i < rate; ++i)
        {
            Il2CppApi::Utils::invoke(method, il2cpp_list, params);
        }
        LOGD("%s %d set after", debug_info, id);
    }
}

void SetContributeLog(int32_t actTime, int32_t playTime)
{
    LOGD("SetContributeLog");

    Il2CppObject *CM_inst = nullptr;
    il2cpp_field_static_get_value(CoopManager_inst, &CM_inst);
    LOGD("get CM_inst %p", CM_inst);

    if (CM_inst)
    {
        auto CM_field = Il2CppApi::Field<Il2CppObject *>(CM_inst);
        auto contributeLog = CM_field.getField<Il2CppObject *>(cm_fields.m_contributeLog);

        if (contributeLog)
        {
            int32_t userId = contributeLog.getField<int32_t>(contribute_log_fields.userId).get();
            auto actionTime = contributeLog.getField<int32_t>(contribute_log_fields.actionTime);
            auto playingTime = contributeLog.getField<int32_t>(contribute_log_fields.playingTime);

            if (userId > 0)
            {
                actionTime.set(actTime);
                playingTime.set(playTime);
            }
        }
    }
}

void SetCompleteResult(Il2CppApi::Field<Il2CppObject *> &IGM_field, DropParam &param)
{
    LOGD("Enter SetCompleteResult");

    int32_t rate = Config::isEnableAdv ? (int)Config::dropRate * (int)Config::advDropRate : (int)Config::dropRate;
    rate = param.isDrop ? rate : rate - 1;

    auto result_field = IGM_field.getField<Il2CppObject *>(igm_fields.result);

    auto playingTimer = IGM_field.getField<float>(igm_fields.playingTimer);
    auto actionTimer = IGM_field.getField<float>(igm_fields.actionTimer);

    auto gold = result_field.getField<Il2CppObject *>(igm_result_fields.gold);
    auto soul = result_field.getField<Il2CppObject *>(igm_result_fields.soul);
    auto cardIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.cardIds);
    auto weaponIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.weaponIds);
    auto ornamentIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.ornamentIds);
    auto itemIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.itemIds);
    auto destroyEnemyIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.destroyEnemyIds);
    auto destroyObjectIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.destroyObjectIds);
    auto openTreasureIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.openTreasureIds);
    auto spTreasureIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.spTreasureIds);
    auto passiveItemIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.passiveItemIds);
    auto paramBoxIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.paramBoxIds);
    auto randomDropAccessoryGroupIds = result_field.getField<Il2CppApi::Il2CppList *>(igm_result_fields.randomDropAccessoryGroupIds);

    auto questGameVal = result_field.getField<Il2CppObject *>(igm_result_fields.questGameVal);
    auto questStringGameVal = result_field.getStringField(igm_result_fields.questStringGameVal);

    auto stageDropData = result_field.getField<Il2CppObject *>(igm_result_fields.stageDropData);

    const int32_t MAX_GOLD = 700000;
    const int32_t MAX_SOUL = 70000;

    if (param.isDrop)
    {
        LOGD("origin gold addr %d soul addr %d", gold.get(), soul.get());

        Il2CppObject *il2cpp_obj = nullptr;

        if (!gold)
        {
            il2cpp_obj = il2cpp_object_new(XorInt_Class);
            XorInt_Create(il2cpp_obj, 0);
            gold.set(il2cpp_obj);
        }

        if (!soul)
        {
            il2cpp_obj = il2cpp_object_new(XorInt_Class);
            XorInt_Create(il2cpp_obj, 0);
            soul.set(il2cpp_obj);
        }

        LOGD("origin gold %d soul %d", param.gold, param.soul);

        int32_t correct_gold = std::min(std::max(param.gold * rate, 0), MAX_GOLD);
        XorInt_Set(gold.get(), correct_gold);
        LOGD("set gold %d", XorInt_Get(gold.get()));

        int32_t correct_soul = std::min(std::max(param.soul * rate, 0), MAX_SOUL);
        XorInt_Set(soul.get(), correct_soul);
        LOGD("set soul %d", XorInt_Get(soul.get()));

        Il2CppObject *XorInt_List = (Il2CppObject *)cardIds.get();
        auto method = RET_FUNC3(il2cpp_class_get_method_from_name, XorInt_List->klass, Const::Method::Clear, 0);

        // --- begin
        // --- 將在關卡內使用直接勝利前取得的道具清除
        //method->invoker_method(method->methodPointer, method, cardIds, nullptr);
        //method->invoker_method(method->methodPointer, method, weaponIds, nullptr);
        //method->invoker_method(method->methodPointer, method, ornamentIds, nullptr);
        Il2CppApi::Utils::invoke(method, itemIds.get(), nullptr);
        //method->invoker_method(method->methodPointer, method, destroyEnemyIds, nullptr);
        //method->invoker_method(method->methodPointer, method, destroyObjectIds, nullptr);
        //method->invoker_method(method->methodPointer, method, openTreasureIds, nullptr);
        //method->invoker_method(method->methodPointer, method, spTreasureIds, nullptr);
        //method->invoker_method(method->methodPointer, method, passiveItemIds, nullptr);
        //method->invoker_method(method->methodPointer, method, paramBoxIds, nullptr);
        //method->invoker_method(method->methodPointer, method, randomDropAccessoryGroupIds, nullptr);
        // --- end

        // --- begin   // 這裡
        Il2CppApi::Utils::invoke(method, destroyEnemyIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, destroyObjectIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, spTreasureIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, cardIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, weaponIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, ornamentIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, openTreasureIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, passiveItemIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, paramBoxIds.get(), nullptr);
        Il2CppApi::Utils::invoke(method, randomDropAccessoryGroupIds.get(), nullptr);
        // --- end

        for (auto &dropData : param.stageDropData)
        {
            switch (dropData.type)
            {
            case DropType::enemy:
                AddEnemies(stageDropData.get(), dropData.id, dropData.conditionType);
                break;
            case DropType::treasure:
                AddTreasures(stageDropData.get(), dropData.id, dropData.conditionType);
                break;
            case DropType::object:
                AddObjects(stageDropData.get(), dropData.id, dropData.conditionType);
                break;
            }
        }

        int32_t playing_time = 0;
        int32_t action_time = 0;
        int32_t rest_time = 0;
        int32_t quest_game_val = 0;
        int32_t addTime = Config::addTime;
        switch (param.questGameType)
        {
        case 3: // TowerAttack
            playing_time = Seed().GenRandomU32(30000, 60000);
            rest_time = std::max(0, param.questGameVal * 1000 + param.enemyTime - playing_time);
            break;
        case 6: // TowerTimeAttack
            playing_time = Seed().GenRandomU32(20000, 50000);
            rest_time = std::max(0, param.questGameVal * 1000 + param.enemyTime - playing_time);
            break;
        case 9: // HitScoreAttack
            quest_game_val = param.enemyValue;
            break;
        case 17: // DangerousTimeAttack
            playing_time = param.timeAttackVal * 1000;
            break;
        case 20: // DamageAttack
        {
            uint64_t damage = 0;

            if (param.score <= 0 && damage > 0)
            {
                damage = param.totalDamage - Seed().GenRandomU64(param.totalDamage / 10, param.totalDamage / 5);
            }
            else if (param.score > param.totalDamage)
            {
                damage = param.score + Seed().GenRandomU64(param.totalDamage / 10, param.totalDamage / 5);
            }
            else if (param.score < param.totalDamage)
            {
                for (int32_t i = 10; i >= 1; i -= 2)
                {
                    if (param.totalDamage / i > param.score)
                    {
                        damage = Seed().GenRandomU64((uint64_t)param.score, param.totalDamage / i);
                    }
                }
                if (!damage)
                {
                    damage = Seed().GenRandomU64((uint64_t)param.score, param.totalDamage);
                }
            }
            else
            {
                damage = param.totalDamage;
            }

            questStringGameVal.set(std::to_string(damage));
        }
        break;
        case 22: // SurvivalEndless
            playing_time = param.judgeTime + Seed().GenRandomU32(10000, 30000);
            rest_time = playing_time;
            break;
        case 24: // BossDestroyNumScoreAttack
            if (param.questGameVal > 0)
            {
                playing_time = std::max(0, param.questGameVal * 1000 - (int)Seed().GenRandomU32(2000, 10000));
            }
            quest_game_val = param.score;
            break;

        DEFAULT_CASE:
        default: // 未測試的關卡類型 (已測 PerformanceScoreAttack)
            if (param.judgeTime > 0)
            {
                LOGD("judgeTime > 0");
                playing_time = param.judgeTime + Seed().GenRandomU32(10000, 30000);
                rest_time = playing_time;
            }
            else if (param.timeAttackFlag)
            {
                LOGD("timeAttackFlag > 0");
                playing_time = Seed().GenRandomU32(20000, 40000);
                rest_time = std::max(0, param.timeAttackVal * 1000 + param.enemyTime - playing_time);
            }

            if (param.score > 0)
            {
                LOGD("score > 0");
                quest_game_val = param.score;
            }
            else
            {
                LOGD("score <= 0");
                quest_game_val = param.questGameVal;
            }
            break;
        }

        if (playing_time <= 0)
        {
            playing_time = Seed().GenRandomU32(30000, 60000) + addTime;
        }

        action_time = playing_time - Seed().GenRandomU32(2000, 8000);

        if (action_time <= 0)
        {
            action_time = playing_time;
        }

        Il2CppObject *result = result_field.get();

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, rest_time);
        set_RestTime(result, il2cpp_obj);

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, playing_time);
        set_PlayingTime(result, il2cpp_obj);

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, action_time);
        set_TotalActionTime(result, il2cpp_obj);

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, param.enemy_list.size());
        set_TotalFoundByEnemyCount(result, il2cpp_obj);

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, param.totalFloor);
        set_ClearFloorNum(result, il2cpp_obj);

        il2cpp_obj = il2cpp_object_new(XorInt_Class);
        XorInt_Create(il2cpp_obj, quest_game_val);
        questGameVal.set(il2cpp_obj);

        actionTimer.set(action_time / 1000.0f);
        playingTimer.set(playing_time / 1000.0f);

        SetContributeLog(actionTimer.get(), playingTimer.get());
    }
    else
    {
        LOGD("origin gold addr %d soul addr %d", gold.get(), soul.get());

        if (!gold)
        {
            Il2CppObject *il2cpp_obj = il2cpp_object_new(XorInt_Class);
            XorInt_Create(il2cpp_obj, 0);
            gold.set(il2cpp_obj);
        }

        if (!soul)
        {
            Il2CppObject *il2cpp_obj = il2cpp_object_new(XorInt_Class);
            XorInt_Create(il2cpp_obj, 0);
            soul.set(il2cpp_obj);
        }

        LOGD("origin gold %d soul %d", XorInt_Get(gold.get()), XorInt_Get(soul.get()));

        int32_t correct_gold = std::min(std::max(XorInt_Get(gold.get()) * (rate + 1), 0), MAX_GOLD);
        XorInt_Set(gold.get(), correct_gold);
        LOGD("set gold %d", XorInt_Get(gold.get()));

        int32_t correct_soul = std::min(std::max(XorInt_Get(soul.get()) * (rate + 1), 0), MAX_SOUL);
        XorInt_Set(soul.get(), correct_soul);
        LOGD("set soul %d", XorInt_Get(soul.get()));

        // --- begin
        // --- 因為未使用直接勝利所以須先取得目前已獲得的所要加倍的道具類型
        // --- 不加倍的話則不取得
        param.card_list = XorIntListToVector(cardIds.get());
        param.weapon_list = XorIntListToVector(weaponIds.get());
        param.ornament_list = XorIntListToVector(ornamentIds.get());
        param.item_list = XorIntListToVector(itemIds.get());
        param.enemy_list = XorIntListToVector(destroyEnemyIds.get());
        param.object_list = XorIntListToVector(destroyObjectIds.get());
        param.treasure_list = XorIntListToVector(openTreasureIds.get());
        param.sp_item_list = XorIntListToVector(spTreasureIds.get());
        param.passive_item_list = XorIntListToVector(passiveItemIds.get());
        param.param_box_list = XorIntListToVector(paramBoxIds.get());
        param.random_drop_list = XorIntListToVector(randomDropAccessoryGroupIds.get());
        // --- end
    }

    LOGD("Set list before");
    if (Config::Injection_Num != 0 && Config::Injection_Id != 0)
        inject_all(Config::Injection_Type, param);

    // --- begin
    // --- 將已取得的道具數量乘上倍率加入內存
    // --- 由於未開啟直接勝利時內存原先已有因此倍率為rate - 1
    VOID_FUNC4(SetPlusResult, cardIds.get(), param.card_list, 1, Const::Field::cardIds);
    VOID_FUNC4(SetPlusResult, weaponIds.get(), param.weapon_list, 1, Const::Field::weaponIds);
    VOID_FUNC4(SetPlusResult, ornamentIds.get(), param.ornament_list, 1, Const::Field::ornamentIds);
    VOID_FUNC4(SetPlusResult, itemIds.get(), param.item_list, rate, Const::Field::itemIds);
    VOID_FUNC4(SetPlusResult, destroyEnemyIds.get(), param.enemy_list, 1, Const::Field::destroyEnemyIds);
    VOID_FUNC4(SetPlusResult, destroyObjectIds.get(), param.object_list, 1, Const::Field::destroyObjectIds);
    VOID_FUNC4(SetPlusResult, openTreasureIds.get(), param.treasure_list, 1, Const::Field::openTreasureIds);
    VOID_FUNC4(SetPlusResult, spTreasureIds.get(), param.sp_item_list, 1, Const::Field::spTreasureIds);
    VOID_FUNC4(SetPlusResult, passiveItemIds.get(), param.passive_item_list, 1, Const::Field::passiveItemIds);
    VOID_FUNC4(SetPlusResult, paramBoxIds.get(), param.param_box_list, 1, Const::Field::paramBoxIds);
    VOID_FUNC4(SetPlusResult, randomDropAccessoryGroupIds.get(), param.random_drop_list, 1, Const::Field::randomDropGroupId);
    // --- end

    LOGD("Exit SetCompleteResult");
}

void SetCompleteResultAfter(Il2CppApi::Field<Il2CppObject *> &complete_field)
{
    LOGD("Enter SetCompleteResultAfter");

    auto myDamageAmountToRaidBoss = complete_field.getStringField(complete_fields.myDamageAmountToRaidBoss);
    auto totalDamageAmountToRaidBoss = complete_field.getStringField(complete_fields.totalDamageAmountToRaidBoss);
    auto totalRestHpOfRaidBoss = complete_field.getStringField(complete_fields.totalRestHpOfRaidBoss);

    std::string total = totalDamageAmountToRaidBoss.to_string();
    if (total == Const::General::empty)
    {
        total = Const::General::num_0;
    }

    std::string rest = totalRestHpOfRaidBoss.to_string();
    if (rest == Const::General::empty)
    {
        rest = Const::General::num_0;
    }
    LOGD("total = %s, rest = %s", total.c_str(), rest.c_str());

    int64_t new_total = std::stoll(total) + std::stoll(rest);
    LOGD("new total = %s", std::to_string(new_total).c_str());
    Il2CppString *new_total_str = il2cpp_string_new(std::to_string(new_total).c_str());

    myDamageAmountToRaidBoss.set(new_total_str);
    totalDamageAmountToRaidBoss.set(new_total_str);
    totalRestHpOfRaidBoss.set(Const::General::num_0);
}

bool is_player(Il2CppObject *player_instance)
{
    return !RET_FUNC2(strcmp, player_instance->klass->name, Const::Klass::Player) && !IsEnemy(player_instance) ||
           !RET_FUNC2(strcmp, player_instance->klass->name, Const::Klass::TownCharacter);
}

bool is_host()
{
    LOGD("Check Host");

    Il2CppObject *CM_inst = nullptr;
    il2cpp_field_static_get_value(CoopManager_inst, &CM_inst);
    LOGD("get CM_inst %p", CM_inst);

    if (!CM_inst)
        return false;

    const MethodInfo *method_isHost = RET_FUNC3(il2cpp_class_get_method_from_name, CM_inst->klass, Const::Method::IsHostPlayer, 0);
    void *isHost_b = Il2CppApi::Utils::invoke(method_isHost, CM_inst, nullptr);
    bool isHost = Il2CppApi::Utils::unbox<bool>((Il2CppObject *)isHost_b);

    LOGD("Host = %d", isHost);

    return isHost;
}

bool is_coop()
{
    LOGD("Check Coop");

    Il2CppObject *IGM_inst = nullptr;
    il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
    LOGD("get IGM_inst %p", IGM_inst);

    if (!IGM_inst)
        return false;

    return IsCoop(IGM_inst);
}

bool is_in_game()
{
    LOGD("Check in_game");

    Il2CppObject *IGM_inst = nullptr;
    il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
    LOGD("get IGM_inst %p", IGM_inst);

    return (bool)IGM_inst;
}

bool get_playing_status()
{
    return !is_coop() || is_host();
}

HOOK_DEF(void, AddTotalTakeDamageValue, Il2CppObject *instance, Il2CppObject *owner, Il2CppObject *attacker, Il2CppObject *damage)
{
    LOGD("AddTotalTakeDamageValue");

    if (!Config::isHack || !Config::isHideDamage)
    {
        old_AddTotalTakeDamageValue(instance, owner, attacker, damage);
        return;
    }

    Il2CppObject *IGM_inst = nullptr;
    il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
    LOGD("get IGM_inst %p", IGM_inst);
    if (IGM_inst)
    {
        auto IGM_field = Il2CppApi::Field<Il2CppObject *>(IGM_inst);
        auto thiz_field = Il2CppApi::Field<Il2CppObject *>(instance);

        int32_t curr_id = get_CurrentCharacterSlotNo(IGM_inst);
        Il2CppArray *players = IGM_field.getField<Il2CppArray *>(igm_fields.players).get();
        if (players && players->max_length > curr_id)
        {
            Il2CppObject *curr_player = (Il2CppObject *)players->vector[curr_id];
            LOGD("curr_player: %p", curr_player);

            Il2CppArray *datas = thiz_field.getField<Il2CppArray *>(quest_playing_data_ctrl_fields.questPlayingDatas).get();
            int32_t tackerId = GetTakeActionPlayerId(instance, attacker);
            void *my_data = nullptr;

            if (datas && datas->max_length > tackerId)
            {
                int32_t offset = tackerId * quest_playing_data_fields.struct_size;
                void *player = *(void **)((BYTE *)datas->vector + offset);
                LOGD("player -> %p %p %p", player, datas->vector, (BYTE *)datas->vector + offset);
                if (player && player == curr_player)
                {
                    my_data = datas->vector + offset;
                    LOGD("match!");
                }
            }

            if (my_data)
            {
                auto my_data_field = Il2CppApi::Field<void *>(my_data);

                LOGD("enter my_data");
                LOGD("enter totalTakeDamageValue %p", quest_playing_data_fields.totalTakeDamageValue);
                auto idamage = my_data_field.getField<int64_t>(
                    quest_playing_data_fields.totalTakeDamageValue);

                int64_t old_damage = idamage.get();
                old_AddTotalTakeDamageValue(instance, owner, attacker, damage);
                int64_t new_damage = idamage.get();

                int64_t safe_damage = old_damage + Seed().GenRandomU32(1000, 10000);
                if (idamage.get() > safe_damage)
                {
                    LOGD("change");
                    idamage.set(safe_damage);
                }

                LOGD("old = %lld, new = %lld, curr = %lld", old_damage, new_damage, idamage.get());
                return;
            }
        }
    }

    old_AddTotalTakeDamageValue(instance, owner, attacker, damage);
}

HOOK_DEF(void, StageManagerStateStageIn_Start, Il2CppObject *instance)
{
    LOGD("Enter StageManagerStateStageIn_Start");

    if (instance && Config::isHack && Config::isEnterWin)
    {
        LOGD("Enter isEnterWin");

        Il2CppObject *SM_inst = nullptr;
        il2cpp_field_static_get_value(StageManager_inst, &SM_inst);
        LOGD("get SM_inst %p", SM_inst);

        if (is_coop())
        {
            if (is_host())
            {
                LOGD("Enter CompleteMission");
                AppInfo::isSendWin = true;
                CompleteMission(SM_inst);
                return;
            }
        }
        else
        {
            LOGD("Enter CompleteMissionSimple");
            AppInfo::isSendWin = true;
            CompleteMissionSimple(SM_inst, false);
            return;
        }
    }

    old_StageManagerStateStageIn_Start(instance);
}

HOOK_DEF(void, StageManagerStateStageClear_Start, Il2CppObject *instance)
{
    LOGD("Enter StageManagerStateStageClear_Start");

    old_StageManagerStateStageClear_Start(instance);
}

HOOK_DEF(void, OpenDialog, Il2CppObject *instance, int32_t type, Il2CppString *body, Il2CppObject *onComplete, bool isEncoding)
{
    LOGD("Enter OpenDialog");

    std::string show_str = std::string((char *)body->chars, body->length);

    if (onComplete)
    {
        LOGD("hook action klass %s %s %d", instance->klass->name, onComplete->klass->name, body->length);
    }
    else
    {
        LOGD("hook action klass == NULL");
    }

    LOGD("hook inst klass %s", instance->klass->name);
    LOGD("hook str status %p", body);
    LOGD("hook str length %d %s", body->length, show_str.c_str());
    LOGD("hook type & isEncoding = %d & %d", type, isEncoding);

    old_OpenDialog(instance, type, body, onComplete, isEncoding);
}

HOOK_DEF(void, SetParameter, Il2CppObject *instance, Il2CppObject *result)
{
    LOGD("Enter SetParameter");
    DropParam dropParam = {};

    bool isHack = Config::isHack;

    Il2CppObject *IGM_inst = nullptr;
    il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
    LOGD("get IGM_inst %p", IGM_inst);

    auto IGM_field = Il2CppApi::Field<Il2CppObject *>(IGM_inst);
    auto thiz_field = Il2CppApi::Field<Il2CppObject *>(instance);

    if (AppInfo::isSendWin || isHack && Config::isEnterWin)
    { // 開啟秒關或秒關已生效的情況
        DropAll(IGM_field, dropParam);
        SetCompleteResult(IGM_field, dropParam);
        AppInfo::isSendWin = false;
    }
    else if (isHack)
    { // 未開啟秒關且秒關未生效但有啟用作弊情況
        SetCompleteResult(IGM_field, dropParam);
        dropParam.isDrop = dropParam.isDrop || Config::isKillEnemy;
    }

    old_SetParameter(instance, result);

    if (dropParam.isDrop)
    {
        SetCompleteResultAfter(thiz_field);
    }
}

HOOK_DEF(void, RequestRankingInfo, Il2CppObject *instance, Il2CppObject *sendParam, Il2CppObject *onComplete)
{
    LOGD("Enter RequestRankingInfo");

    if (instance)
    {
        Il2CppObject *IGM_inst = nullptr;
        il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
        LOGD("get IGM_inst %p", IGM_inst);

        if (IGM_inst && Config::isHack && Config::isEnterWin)
        {
            LOGD("Enter Rank Excute");

            auto IGM_field = Il2CppApi::Field<Il2CppObject *>(IGM_inst);
            auto sendParam_field = Il2CppApi::Field<Il2CppObject *>(sendParam);

            auto stageClearTimes = sendParam_field.getField<Il2CppArray *>(ranking_fields.stageClearTimes);
            LOGD("stageClearTimes type %s", stageClearTimes.get()->obj.klass->name);

            auto rank_param = Ranking(IGM_field);

            Il2CppArray *_array = il2cpp_array_new_specific(stageClearTimes.get()->obj.klass, rank_param.ranking_time_list.size());
            auto array = Il2CppApi::Array<int32_t>(_array);
            for (int32_t i = 0; i < array.length(); ++i)
            {
                array.set(i, rank_param.ranking_time_list[i]);
                LOGD("set -> %d", i);
            }

            stageClearTimes.set((Il2CppArray *)(intptr_t)array);
        }
    }

    old_RequestRankingInfo(instance, sendParam, onComplete);
}

HOOK_DEF(void, UI_Talk_OpenWindow, Il2CppObject *instance)
{
    LOGD("Enter UI_Talk_OpenWindow");

    old_UI_Talk_OpenWindow(instance);

    if (Config::isHack && Config::isSkipTalk)
    {

        const MethodInfo *method_skip = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::OnButtonSkip, 0);
        Il2CppApi::Utils::invoke(method_skip, instance, nullptr);
        const MethodInfo *method = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::OnButtonExit, 0);
        Il2CppApi::Utils::invoke(method, instance, nullptr);
    }
}

HOOK_DEF(int32_t, get_CurrentStep, Il2CppObject *instance)
{
    LOGD("Enter get_CurrentStep");

    if (Config::isHack && Config::isSkipTutorial)
    {
        return -1;
    }

    return old_get_CurrentStep(instance);
}

HOOK_DEF(int32_t, Damage, Il2CppObject *instance, Il2CppObject *damage)
{
    LOGD("Enter Damage");

    if (Config::isHack && Config::isInvincible && is_player(instance))
    {
        LOGD("Enter isInvincible");
        return 0;
    }

    return old_Damage(instance, damage);
}

HOOK_DEF(void, SpDamage, Il2CppObject *instance, Il2CppObject *spDamageData, bool damageReserves)
{
    LOGD("Enter SpDamage");

    if (Config::isHack && Config::isInvincible && is_player(instance))
    {
        LOGD("Enter isInvincible");
        return;
    }

    return old_SpDamage(instance, spDamageData, damageReserves);
}

HOOK_DEF(bool, IsDamageHit, Il2CppObject *instance, Il2CppObject *ac)
{
    LOGD("Enter IsDamageHit");

    if (instance && Config::isHack && Config::isInvincible && is_player(instance))
    {
        LOGD("Enter isInvincible");
        return false;
    }

    return old_IsDamageHit(instance, ac);
}

HOOK_DEF(bool, Catch, Il2CppObject *instance, Il2CppObject *target)
{
    LOGD("Enter Catch");

    if (instance && Config::isHack && Config::isInvincible)
    {
        LOGD("Enter isInvincible");
        return false;
    }

    return old_Catch(instance, target);
}

HOOK_DEF(bool, UpdateDie, Il2CppObject *instance, Il2CppObject *damage)
{
    LOGD("Enter UpdateDie");

    if (instance && Config::isHack && Config::isInvincible)
    {
        LOGD("Enter isInvincible");
        return false;
    }

    return old_UpdateDie(instance, damage);
}

HOOK_DEF(void, Coop_Model_Sp__ctor, Il2CppObject *instance, int32_t value, bool isDownMaxSp)
{
    LOGD("Enter Coop_Model_Sp__ctor");

    if (instance && Config::isHack && Config::isInfiniteSp)
    {
        LOGD("Enter isInfiniteSp");
        value = 99999999;
    }

    old_Coop_Model_Sp__ctor(instance, value, isDownMaxSp);
}

HOOK_DEF(int32_t, AddSp, Il2CppObject *instance, int32_t num, bool fouceHeal, bool isStopRecoveryLimiter, bool isJobHandicap, bool isDrawFont, int32_t healCondition)
{
    LOGD("Enter AddSp");

    if (instance && Config::isHack && Config::isInfiniteSp)
    {
        Il2CppObject *IGM_inst = nullptr;
        il2cpp_field_static_get_value(InGameManager_inst, &IGM_inst);
        LOGD("get IGM_inst %p", IGM_inst);

        if (IGM_inst)
        {
            int32_t curr_id = get_CurrentCharacterSlotNo(IGM_inst);
            int32_t this_id = get_PlayerSlotNo(instance);
            if (curr_id == this_id)
            {
                LOGD("Enter isInfiniteSp");
                num = 99999999;
            }
        }
    }

    return old_AddSp(instance, num, fouceHeal, isStopRecoveryLimiter, isJobHandicap, isDrawFont, healCondition);
}

HOOK_DEF(void, OnForceKnockDownDamageReaction, Il2CppObject *instance, Il2CppObject *damage)
{
    LOGD("Enter OnForceKnockDownDamageReaction");

    if (instance && Config::isHack && Config::isInvincible && is_player(instance))
    {
        LOGD("Enter isInvincible");
        return;
    }

    old_OnForceKnockDownDamageReaction(instance, damage);
}

HOOK_DEF(float, GetRunSpeed, Il2CppObject *instance)
{
    //LOGD("Enter GetRunSpeed");

    if (is_player(instance))
    {
        return old_GetRunSpeed(instance) * (Config::isHack ? (int)Config::speedRate : 1);
    }

    return old_GetRunSpeed(instance);
}

HOOK_DEF(bool, IsBossMap, Il2CppObject *instance)
{
    LOGD("enter IsBossMap");

    if (instance && Config::isHack && Config::isKillEnemy && get_playing_status())
    {
        return false;
    }

    return old_IsBossMap(instance);
}

HOOK_DEF(bool, get_IsBoss, Il2CppObject *instance)
{
    LOGD("enter get_IsBoss");

    if (instance && Config::isHack && Config::isKillEnemy && get_playing_status())
    {
        return false;
    }

    return old_get_IsBoss(instance);
}

HOOK_DEF(void, BreakableObjectBase_Start, Il2CppObject *instance)
{
    LOGD("Enter BreakableObjectBase_Start");

    old_BreakableObjectBase_Start(instance);

    if (Config::isEnableAdv && Config::isCloseObjectKill)
    {
        return;
    }

    if (instance && Config::isHack && Config::isKillEnemy && is_in_game() && get_playing_status())
    {
        LOGD("Enter BreakableObjectBase Death");

        LOGD("hook klass %s", instance->klass->name);

        auto method = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::Death, 0);
        method->invoker_method(method->methodPointer, method, instance, nullptr);
    }
}

HOOK_DEF(void, TreasureBox_Start, Il2CppObject *instance)
{
    LOGD("Enter TreasureBox_Start");

    old_TreasureBox_Start(instance);

    if (Config::isEnableAdv && Config::isCloseTreasureKill)
    {
        return;
    }

    if (instance && Config::isHack && Config::isKillEnemy && is_in_game() && get_playing_status())
    {
        LOGD("Enter TreasureBox Death");

        LOGD("hook klass %s", instance->klass->name);

        auto method = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::Death, 0);
        method->invoker_method(method->methodPointer, method, instance, nullptr);
    }
}

HOOK_DEF(void, Monster_Start, Il2CppObject *instance)
{
    LOGD("Enter Monster_Start");

    old_Monster_Start(instance);

    if (Config::isEnableAdv && Config::isCloseEnemyKill)
    {
        return;
    }

    if (instance && Config::isHack && Config::isKillEnemy && is_in_game() && get_playing_status())
    {
        LOGD("Enter Monster Death");

        LOGD("hook klass %s", instance->klass->name);

        auto method = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::Death, 0);
        method->invoker_method(method->methodPointer, method, instance, nullptr);
    }
}

HOOK_DEF(void, Player_Start, Il2CppObject *instance)
{
    LOGD("Enter Player_Start");

    old_Player_Start(instance);

    if (Config::isEnableAdv && Config::isCloseEnemyKill)
    {
        return;
    }

    LOGD("IsEnemy %d", IsEnemy(instance));

    if (instance && Config::isHack && Config::isKillEnemy && is_in_game() && !is_player(instance) && get_playing_status())
    {
        LOGD("Enter Player Death %d %d", is_player(instance), get_playing_status());

        LOGD("hook klass %s", instance->klass->name);
        auto method = RET_FUNC3(il2cpp_class_get_method_from_name, instance->klass, Const::Method::Death, 0);
        method->invoker_method(method->methodPointer, method, instance, nullptr);
        LOGD("hook klass %s after", instance->klass->name);
    }
}

HOOK_DEF(void, set_slopeLimit, Il2CppObject *instance, float value)
{
    LOGD("Enter set_slopeLimit");

    if (Config::isHack && Config::isGodMode)
    {
        LOGD("Enter isGodMode");
        value = 360;
    }

    old_set_slopeLimit(instance, value);
}

HOOK_DEF(void, set_stepOffset, Il2CppObject *instance, float value)
{
    LOGD("Enter set_stepOffset");

    if (Config::isHack && Config::isGodMode)
    {
        LOGD("Enter isGodMode");
        value = 1000000;
    }

    old_set_stepOffset(instance, value);
}

HOOK_DEF(bool, SetupCamera, Il2CppObject *instance, Il2CppObject *data, float distance)
{
    LOGD("Enter SetupCamera");

    if (Config::isHack && Config::isGodMode)
    {
        distance = 30;
        set_CameraDistanceMax(data, distance);
        set_CameraDegV(data, 1);
        set_CameraDegH(data, 270);
    }

    return old_SetupCamera(instance, data, distance);
}

HOOK_DEF(float, get_TownAddHPRate, Il2CppObject *instance)
{
    LOGD("Enter get_TownAddHPRate");

    float orign = old_get_TownAddHPRate(instance);
    bool isEnableAdv = Config::isEnableAdv;

    if (!Config::isHack || isEnableAdv && Config::isCloseHPRate)
    {
        return orign;
    }

    int32_t abilityRate = Config::abilityRate;
    float ret = orign * abilityRate + abilityRate - 1;

    return isEnableAdv ? ret * Config::advAbilityRate : ret;
}

HOOK_DEF(float, get_TownAddSPRate, Il2CppObject *instance)
{
    LOGD("Enter get_TownAddSPRate");

    float orign = old_get_TownAddSPRate(instance);
    bool isEnableAdv = Config::isEnableAdv;

    if (!Config::isHack || isEnableAdv && Config::isCloseSPRate)
    {
        return orign;
    }

    int32_t abilityRate = Config::abilityRate;
    float ret = orign * abilityRate + abilityRate - 1;

    return isEnableAdv ? ret * Config::advAbilityRate : ret;
}

HOOK_DEF(int32_t, GetBaseAtk, Il2CppObject *instance)
{
    LOGD("Enter GetBaseAtk");

    int32_t orign = old_GetBaseAtk(instance);
    bool isEnableAdv = Config::isEnableAdv;

    if (!Config::isHack || isEnableAdv && Config::isCloseATKRate)
    {
        return orign;
    }

    int32_t abilityRate = Config::abilityRate;
    int32_t ret = orign * abilityRate;

    return isEnableAdv ? ret * Config::advAbilityRate : ret;
}

HOOK_DEF(float, get_TownAddDefRate, Il2CppObject *instance)
{
    LOGD("Enter get_TownAddDefRate");

    float orign = old_get_TownAddDefRate(instance);
    bool isEnableAdv = Config::isEnableAdv;

    if (!Config::isHack || isEnableAdv && Config::isCloseDEFRate)
    {
        return orign;
    }

    int32_t abilityRate = Config::abilityRate;
    float ret = orign * abilityRate + abilityRate - 1;

    return isEnableAdv ? ret * Config::advAbilityRate : ret;
}

HOOK_DEF(float, get_TownAddHitRate, Il2CppObject *instance)
{
    LOGD("Enter get_TownAddHitRate");

    float orign = old_get_TownAddHitRate(instance);
    bool isEnableAdv = Config::isEnableAdv;

    if (!Config::isHack || isEnableAdv && Config::isCloseHITRate)
    {
        return orign;
    }

    int32_t abilityRate = Config::abilityRate;
    float ret = orign * abilityRate + abilityRate - 1;

    return isEnableAdv ? ret * Config::advAbilityRate : ret;
}
