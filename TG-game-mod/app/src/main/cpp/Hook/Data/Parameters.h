#pragma once

#include <vector>

enum DropType
{
    enemy,
    treasure,
    object,
};

enum ItemType
{
    Item,
    Treasure,
    PassiveItem,
    Weapon,
    Ornament,
    Card,
    Accessory,
    ParamBox,
    DestroyEnemy,
    DestroyObject,
    OpenTreasure
};

struct SaveData_Key_Fields
{
    int32_t value__;
};

struct GlobalDataManager_Fields
{
    int32_t m_Player;
    int32_t m_Account;
    int32_t m_CurrentWorld;
    int32_t m_CommonSaveData;
};

struct GlobalDataManager_CommonSaveData_Fields
{
    int32_t read;
};

struct GlobalDataManager_CommonSaveData_Read_Fields
{
    int32_t titleSkipVersionName;
};

struct GlobalDataManager_Account_Fields
{
    int32_t token;
    int32_t userHash;
    int32_t userIdHash;
    int32_t mailAddress;
    int32_t gmailAddress;
#ifdef TW
    int32_t apHash;
#endif
};

struct PlayerModel_Param_Fields
{
    int32_t info;
};

struct UserInfoModel_Param_Fields
{
    int32_t id;
};

struct QuestPlayingDataController_Fields
{
    int32_t questPlayingDatas;
};

struct QuestPlayingDataController_QuestPlayingData_Fields
{
    int32_t struct_size;
    int32_t player;
    int32_t totalTakeDamageValue;
};

struct CoopManager_Fields
{
    int32_t m_contributeLog;
};

struct CoopContributeLogModel_Request_Fields
{
    int32_t userId;
    int32_t damage;
    int32_t actionTime;
    int32_t playingTime;
};

struct InGameManager_Fields
{
    int32_t players;
    int32_t result;
    int32_t generate;
    int32_t playingTimer;
    int32_t actionTimer;
};

struct InGameManager_Result_Fields
{
    int32_t gold;
    int32_t soul;
    int32_t cardIds;
    int32_t weaponIds;
    int32_t ornamentIds;
    int32_t itemIds;
    int32_t destroyEnemyIds;
    int32_t destroyObjectIds;
    int32_t openTreasureIds;
    int32_t spTreasureIds;
    int32_t passiveItemIds;

    int32_t questGameVal;
    int32_t questStringGameVal;
    int32_t MyDamageAmountToRaidBoss;
    int32_t TotalDamageAmountToRaidBoss;
    int32_t paramBoxIds;

    int32_t randomDropAccessoryGroupIds;

    int32_t stageDropData;
};

struct QuestGenerateModel_Param_Fields
{
    int32_t timeAttackFlag;
    int32_t timeAttackVal;
    int32_t questGameType;
    int32_t questGameVal;
    int32_t questGameVal2;
    int32_t totalFloor;
    int32_t stageDatas;
    int32_t paramBoxs;
    int32_t rankingEvent;
    int32_t scoreRankList;
    int32_t timeRankList;
};

struct QuestGenerateModel_RankingEventInfo_Fields
{
    int32_t ghosts;
};

struct QuestGenerateModel_RankingEventInfo_Ghost_Fields
{
    int32_t sTimes;
};

struct QuestGenerateModel_EnemyParamBox_Fields
{
    int32_t boxId;
};

struct QuestGenerateModel_Stage_Fields
{
    int32_t stageEnemyParams;
    int32_t stageEnemies;
    int32_t stageTreasures;
    int32_t stageObjects;
};

struct QuestGenerateModel_Stage_DropInfo_Fields
{
    int32_t id;
    int32_t dropId;
    int32_t dropNum;
    int32_t spDropId;
    int32_t treasureId;
    int32_t randomDropGroupId;
    int32_t cardId;
    int32_t cardNum;
    int32_t weaponId;
    int32_t ornamentId;
    int32_t itemId;
    int32_t itemNum;
    int32_t passiveItemId;
    int32_t addScoreValue;
    int32_t conditionType;
};

struct QuestGenerateModel_Stage_EnemyParam_Fields
{
    int32_t smonId;
    int64_t hp;
    int32_t exp;
    int32_t gold;
    int32_t paramBoxId;
    int32_t scoreKey1;
    int32_t scoreVal1;
    int32_t scoreVal2;
};

struct QuestGenerateModel_Stage_Enemy_Fields
{
    int32_t id;
    int32_t smonId;
    int32_t raidBoss;

    int32_t dropInfos;
};

struct QuestGenerateModel_Stage_TreasureBox_Fields
{
    int32_t scoreKey1;
    int32_t scoreVal1;
    int32_t dropInfos;
};

struct QuestGenerateModel_Stage_PlacementObject_Fields
{
    int32_t id;
    int32_t scoreKey1;
    int32_t scoreVal1;

    int32_t dropInfos;
};

struct QuestCompleteSendParam_Fields
{
    int32_t myDamageAmountToRaidBoss;
    int32_t totalDamageAmountToRaidBoss;
    int32_t totalRestHpOfRaidBoss;
};

struct QuestRankingModel_SendParam_Fields
{
    int32_t stageClearTimes;
};

struct StageDropData
{
    DropType type;
    int64_t id;
    int32_t conditionType;
};

struct DropParam
{
    bool isDrop;

    int32_t timeAttackFlag;
    int32_t timeAttackVal;
    int32_t questGameType;
    int32_t questGameVal;
    int32_t questGameVal2;
    int32_t totalFloor;

    int32_t gold;
    int32_t soul;
    std::vector<int32_t> enemy_list;
    std::vector<int32_t> treasure_list;
    std::vector<int32_t> object_list;
    std::vector<int32_t> weapon_list;
    std::vector<int32_t> card_list;
    std::vector<int32_t> item_list;
    std::vector<int32_t> ornament_list;
    std::vector<int32_t> passive_item_list;
    std::vector<int32_t> sp_item_list;
    std::vector<int32_t> param_box_list;
    std::vector<int32_t> random_drop_list;
    std::vector<StageDropData> stageDropData;
    bool isRaidBoss;

    int32_t judgeTime;
    int32_t score;
    int32_t enemyValue;
    int32_t enemyTime;

    uint64_t totalDamage;
};

struct RankParam
{
    std::vector<int32_t> ranking_time_list;
};

struct QuestGenerateModel_TimeBorder_Fields
{
    int32_t judgeTime;
    int32_t rank;
};

struct QuestGenerateModel_ScoreBorder_Fields
{
    int32_t id;
    int32_t score;
    int32_t rank;
    int32_t se;
};
