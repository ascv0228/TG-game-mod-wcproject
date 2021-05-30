#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <dlfcn.h>

#include <dobby.h>
#include <Utils.h>
#include <BridgeFunc.h>

#include "HookEntry.h"
#include "HookFuncs.h"
#include "Data/AppInfo.h"
#include "Data/Defs.h"
#include "../Il2cpp/il2cpp-init.h"
#include "../Il2cpp/il2cpp-appdata.h"
#include "../Il2cpp/Il2cppApi/Helper.hpp"
#include "../Const/Const_Assembly.h"
#include "../Const/Const_NameSpaze.h"
#include "../Const/Const_Klass.h"
#include "../Const/Const_Method.h"
#include "../Const/Const_Field.h"
#include "../Const/Const_General.h"

bool il2cpp_hook(const MethodInfo *method, void *new_func, void **old_func)
{
        //auto method = il2cpp_find_method(assembly_name, namespaze, klasses, func_name, argsCount);

        if (!method)
                return false;

        DobbyHook((void *)method->methodPointer, new_func, old_func);

        LOGD("hack game hook method %s: RVA->%p VA->%p",
             method->name, (void *)((uint64_t)method->methodPointer - AppInfo::il2cpp_base), method->methodPointer);

        return true;
}
void load_fields()
{
        /*auto CLASS(UIManager,
               Const::Assembly::Assembly_CSharp_dll,
               Const::NameSpaze::empty,
               {SAFE_CSTR(Const::Klass::UIManager)});
    CLASS(UI_Common_Class,
               Const::Assembly::Assembly_CSharp_dll,
               Const::NameSpaze::empty,
               {SAFE_CSTR(Const::Klass::UI_Common)});
    FIELD_SN_ADDR(UIManager_inst, UIManager, instance);
    METHOD( Const::Assembly::Assembly_CSharp_dll,
            Const::NameSpaze::empty,
            {SAFE_CSTR(Const::Klass::UIManager)},
            get_CommonUI, 0);
    METHOD( Const::Assembly::Assembly_CSharp_dll,
            Const::NameSpaze::empty,
            {SAFE_CSTR(Const::Klass::UI_Common)},
            OpenDialog, 4);
    HOOK( Const::Assembly::Assembly_CSharp_dll,
            Const::NameSpaze::empty,
            {SAFE_CSTR(Const::Klass::UI_Common)},
            OpenDialog, 4);*/

        /*LoadClass(xxx,
          Const::Assembly::Assembly_CSharp_dll,
          Const::NameSpaze::empty,
          KS(SAFE_CSTR(Const::Klass::KtbWebSocket)));*/

        auto CLASS(KtbWebSocket,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::KtbWebSocket));
        FIELD_SN_ADDR(KtbWebSocket_inst, KtbWebSocket, instance);

        auto CLASS(GlobalDataManager,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::GlobalDataManager));
        FIELD_SN_ADDR(GlobalDataManager_inst, GlobalDataManager, instance);
        FIELD_OFFSET(gdm_fields, GlobalDataManager, m_Player);
        FIELD_OFFSET(gdm_fields, GlobalDataManager, m_Account);
        FIELD_OFFSET(gdm_fields, GlobalDataManager, m_CurrentWorld);
        FIELD_OFFSET(gdm_fields, GlobalDataManager, m_CommonSaveData);

        auto CLASS(GlobalDataManager_CommonSaveData,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::GlobalDataManager, Const::Klass::CommonSaveData));
        FIELD_OFFSET(gdm_save_fields, GlobalDataManager_CommonSaveData, read);

        auto CLASS(GlobalDataManager_CommonSaveData_Read,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::GlobalDataManager, Const::Klass::CommonSaveData, Const::Klass::Read));
        FIELD_OFFSET(gdm_save_read_fields, GlobalDataManager_CommonSaveData_Read, titleSkipVersionName);

        auto CLASS(GlobalDataManager_Account,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::GlobalDataManager, Const::Klass::Account));
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, token);
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, userHash);
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, userIdHash);
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, mailAddress);
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, gmailAddress);
#ifdef TW
        FIELD_OFFSET(gdm_acc_fields, GlobalDataManager_Account, apHash);
#endif
        auto CLASS(PlayerModel_Param,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::PlayerModel, Const::Klass::Param));
        FIELD_OFFSET(player_param_fields, PlayerModel_Param, info);

        auto CLASS(UserInfoModel_Param,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::UserInfoModel, Const::Klass::Param));
        FIELD_OFFSET(user_info_fields, UserInfoModel_Param, id);

        auto CLASS(QuestPlayingDataController,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestPlayingDataController));
        FIELD_OFFSET(quest_playing_data_ctrl_fields, QuestPlayingDataController, questPlayingDatas);

        auto CLASS(QuestPlayingDataController_QuestPlayingData,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestPlayingDataController, Const::Klass::QuestPlayingData));
        FIELD_OFFSET(quest_playing_data_fields, QuestPlayingDataController_QuestPlayingData, player);
        FIELD_OFFSET(quest_playing_data_fields, QuestPlayingDataController_QuestPlayingData, totalTakeDamageValue);
        quest_playing_data_fields.struct_size = 0x28;

        auto CLASS(CoopManager,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::CoopManager});
        FIELD_SN_ADDR(CoopManager_inst, CoopManager, instance);
        FIELD_OFFSET(cm_fields, CoopManager, m_contributeLog);

        auto CLASS(CoopContributeLogModel_Request,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::CoopContributeLogModel, Const::Klass::Request));
        FIELD_OFFSET(contribute_log_fields, CoopContributeLogModel_Request, userId);
        FIELD_OFFSET(contribute_log_fields, CoopContributeLogModel_Request, damage);
        FIELD_OFFSET(contribute_log_fields, CoopContributeLogModel_Request, actionTime);
        FIELD_OFFSET(contribute_log_fields, CoopContributeLogModel_Request, playingTime);

        auto CLASS(StageManager,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageManager});
        FIELD_SN_ADDR(StageManager_inst, StageManager, instance);

        auto CLASS(InGameManager,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::InGameManager));
        FIELD_SN_ADDR(InGameManager_inst, InGameManager, instance);
        FIELD_OFFSET(igm_fields, InGameManager, players);
        FIELD_OFFSET(igm_fields, InGameManager, result);
        FIELD_OFFSET(igm_fields, InGameManager, generate);
        FIELD_OFFSET(igm_fields, InGameManager, playingTimer);
        FIELD_OFFSET(igm_fields, InGameManager, actionTimer);

        auto CLASS(InGameManager_Result,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::InGameManager, Const::Klass::Result));
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, gold);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, soul);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, cardIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, weaponIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, ornamentIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, itemIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, destroyEnemyIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, destroyObjectIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, openTreasureIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, spTreasureIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, passiveItemIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, paramBoxIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, randomDropAccessoryGroupIds);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, questGameVal);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, questStringGameVal);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, MyDamageAmountToRaidBoss);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, TotalDamageAmountToRaidBoss);
        FIELD_OFFSET(igm_result_fields, InGameManager_Result, stageDropData);

        auto CLASS(QuestGenerateModel_Param,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Param));
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, stageDatas);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, timeAttackFlag);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, timeAttackVal);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, questGameType);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, questGameVal);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, questGameVal2);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, totalFloor);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, paramBoxs);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, rankingEvent);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, scoreRankList);
        FIELD_OFFSET(qp_fields, QuestGenerateModel_Param, timeRankList);

        auto CLASS(QuestGenerateModel_RankingEventInfo,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::RankingEventInfo));
        FIELD_OFFSET(qr_fields, QuestGenerateModel_RankingEventInfo, ghosts);

        auto CLASS(QuestGenerateModel_RankingEventInfo_Ghost,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::RankingEventInfo, Const::Klass::Ghost));
        FIELD_OFFSET(rank_ghost_fields, QuestGenerateModel_RankingEventInfo_Ghost, sTimes);

        auto CLASS(QuestGenerateModel_EnemyParamBox,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::EnemyParamBox));
        FIELD_OFFSET(param_box_fields, QuestGenerateModel_EnemyParamBox, boxId);

        auto CLASS(QuestGenerateModel_Stage,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage));
        FIELD_OFFSET(qs_fields, QuestGenerateModel_Stage, stageEnemyParams);
        FIELD_OFFSET(qs_fields, QuestGenerateModel_Stage, stageEnemies);
        FIELD_OFFSET(qs_fields, QuestGenerateModel_Stage, stageTreasures);
        FIELD_OFFSET(qs_fields, QuestGenerateModel_Stage, stageObjects);

        auto CLASS(QuestGenerateModel_Stage_DropInfo,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage, Const::Klass::DropInfo));
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, id);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, dropId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, dropNum);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, spDropId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, treasureId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, randomDropGroupId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, cardId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, cardNum);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, weaponId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, ornamentId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, itemId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, itemNum);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, passiveItemId);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, addScoreValue);
        FIELD_OFFSET(drop_fields, QuestGenerateModel_Stage_DropInfo, conditionType);

        auto CLASS(QuestGenerateModel_Stage_EnemyParam,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage, Const::Klass::EnemyParam));
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, smonId);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, hp);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, gold);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, exp);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, paramBoxId);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, scoreKey1);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, scoreVal1);
        FIELD_OFFSET(enemy_param_fields, QuestGenerateModel_Stage_EnemyParam, scoreVal2);

        auto CLASS(QuestGenerateModel_Stage_Enemy,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage, Const::Klass::Enemy));
        FIELD_OFFSET(enemy_fields, QuestGenerateModel_Stage_Enemy, id);
        FIELD_OFFSET(enemy_fields, QuestGenerateModel_Stage_Enemy, smonId);
        FIELD_OFFSET(enemy_fields, QuestGenerateModel_Stage_Enemy, raidBoss);
        FIELD_OFFSET(enemy_fields, QuestGenerateModel_Stage_Enemy, dropInfos);

        auto CLASS(QuestGenerateModel_Stage_TreasureBox,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage, Const::Klass::TreasureBox));
        FIELD_OFFSET(treasure_box_fields, QuestGenerateModel_Stage_TreasureBox, scoreKey1);
        FIELD_OFFSET(treasure_box_fields, QuestGenerateModel_Stage_TreasureBox, scoreVal1);
        FIELD_OFFSET(treasure_box_fields, QuestGenerateModel_Stage_TreasureBox, dropInfos);

        auto CLASS(QuestGenerateModel_Stage_PlacementObject,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::Stage, Const::Klass::PlacementObject));
        FIELD_OFFSET(object_fields, QuestGenerateModel_Stage_PlacementObject, id);
        FIELD_OFFSET(object_fields, QuestGenerateModel_Stage_PlacementObject, scoreKey1);
        FIELD_OFFSET(object_fields, QuestGenerateModel_Stage_PlacementObject, scoreVal1);
        FIELD_OFFSET(object_fields, QuestGenerateModel_Stage_PlacementObject, dropInfos);

        auto CLASS(QuestCompleteSendParam,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::QuestCompleteSendParam});
        FIELD_OFFSET(complete_fields, QuestCompleteSendParam, myDamageAmountToRaidBoss);
        FIELD_OFFSET(complete_fields, QuestCompleteSendParam, totalDamageAmountToRaidBoss);
        FIELD_OFFSET(complete_fields, QuestCompleteSendParam, totalRestHpOfRaidBoss);

        auto CLASS(QuestRankingModel_SendParam,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestRankingModel, Const::Klass::SendParam));
        FIELD_OFFSET(ranking_fields, QuestRankingModel_SendParam, stageClearTimes);

        auto CLASS(QuestGenerateModel_TimeBorder,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::TimeBorder));
        FIELD_OFFSET(time_border_fields, QuestGenerateModel_TimeBorder, judgeTime);
        FIELD_OFFSET(time_border_fields, QuestGenerateModel_TimeBorder, rank);

        auto CLASS(QuestGenerateModel_ScoreBorder,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestGenerateModel, Const::Klass::ScoreBorder));
        FIELD_OFFSET(score_border_fields, QuestGenerateModel_ScoreBorder, rank);
        FIELD_OFFSET(score_border_fields, QuestGenerateModel_ScoreBorder, score);

        auto CLASS(OutGameManager,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::OutGameManager));
        FIELD_SN_ADDR(OutGameManager_inst, OutGameManager, instance);
}

void load_class()
{
        CLASS(XorLong_Class,
              Const::Assembly::Assembly_CSharp_dll,
              Const::NameSpaze::empty,
              KS(Const::Klass::XorLong));

        CLASS(XorInt_Class,
              Const::Assembly::Assembly_CSharp_dll,
              Const::NameSpaze::empty,
              KS(Const::Klass::XorInt));

        CLASS(WorldModel_Param_Class,
              Const::Assembly::Assembly_CSharp_dll,
              Const::NameSpaze::empty,
              KS(Const::Klass::WorldModel, Const::Klass::Param));

        /*CLASS(Action_Class,
          Const::Assembly::mscorlib_dll,
          Const::NameSpaze::System,
          KS(SAFE_CSTR(Const::Klass::Action_1)));*/
}

void load_methods()
{
        METHOD_SN(WorldModel_Param_Class, WorldModel_Param_Create, _ctor, 0);

        auto CLASS(Coop_Model_Base_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::Coop_Model_Base));
        METHOD(Coop_Model_Base_Class, get_from_user_id, 0);

        auto CLASS(CoopManager_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::CoopManager));
        METHOD(CoopManager_Class, GetLocalAccessId, 0);

        auto CLASS(Player_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::Player));
        METHOD(Player_Class, get_PlayerSlotNo, 0);
        METHOD(Player_Class, IsEnemy, 0);

        auto CLASS(InGameManager_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::InGameManager));
        METHOD(InGameManager_Class, get_CurrentCharacterSlotNo, 0);
        METHOD(InGameManager_Class, IsCoop, 0);

        auto CLASS(QuestPlayingDataController_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestPlayingDataController));
        METHOD(QuestPlayingDataController_Class, GetTakeActionPlayerId, 1);

        auto CLASS(StageManager_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageManager});
        METHOD(StageManager_Class, CompleteMission, 0);
        METHOD(StageManager_Class, CompleteMissionSimple, 1);

        METHOD_SN(XorInt_Class, XorInt_Get, get_value, 0);
        METHOD_SN(XorInt_Class, XorInt_Set, set_value, 1);
        METHOD_SN(XorInt_Class, XorInt_Create, _ctor, 1);

        METHOD_SN(XorLong_Class, XorLong_Get, get_Value, 0);
        METHOD_SN(XorLong_Class, XorLong_Set, set_Value, 1);
        METHOD_SN(XorLong_Class, XorLong_Create, _ctor, 1);

        auto CLASS(StageData_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageData});
        METHOD(StageData_Class, set_CameraDegH, 1);
        METHOD(StageData_Class, set_CameraDegV, 1);
        METHOD(StageData_Class, set_CameraDistanceMax, 1);

        auto CLASS(InGameManager_Result_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::InGameManager, Const::Klass::Result));
        METHOD(InGameManager_Result_Class, set_PlayingTime, 1);
        METHOD(InGameManager_Result_Class, set_RestTime, 1);
        METHOD(InGameManager_Result_Class, set_QuestGameType, 1);
        METHOD(InGameManager_Result_Class, set_ClearFloorNum, 1);
        METHOD(InGameManager_Result_Class, set_TotalActionTime, 1);
        METHOD(InGameManager_Result_Class, set_TotalFoundByEnemyCount, 1);

        auto CLASS(StageDropData_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageDropData});
        METHOD(StageDropData_Class, AddEnemies, 2);
        METHOD(StageDropData_Class, AddTreasures, 2);
        METHOD(StageDropData_Class, AddObjects, 2);

        auto CLASS(Cookie_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::Cookie));
        METHOD_SN(Cookie_Class, Cookie_setCookieToken, setCookieToken, 1);

        auto CLASS(OutGameManager_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::OutGameManager));
        METHOD_SN(OutGameManager_Class, OutGameManager_ChangeScene, ChangeScene, 2);

        auto CLASS(GlobalDataManager_class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::GlobalDataManager));
        METHOD_SN(GlobalDataManager_class, GlobalDataManager_CreateSaveData, CreateSaveData, 0);

        auto CLASS(SaveData_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::SaveData));
        METHOD_SN(SaveData_Class, SaveData_SetString, SetString, 2);
}

void load_hook()
{
        auto CLASS(StageManagerStateStageIn_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageManagerStateStageIn});
        auto LoadMethod(StageManagerStateStageIn_Class, Start, 0);
        HOOK_P(StageManagerStateStageIn_Class_Start, StageManagerStateStageIn, Start);

        /*auto CLASS(StageManagerStateStageClear_Class,
               Const::Assembly::Assembly_CSharp_dll,
               Const::NameSpaze::empty,
               {SAFE_CSTR(Const::Klass::StageManagerStateStageClear)});
    auto LoadMethod(StageManagerStateStageClear_Class, Start, 0);
    HOOK_P(StageManagerStateStageClear_Class_Start, StageManagerStateStageClear, Start);*/

        auto CLASS(QuestCompleteSendParam_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::QuestCompleteSendParam});
        auto LoadMethod(QuestCompleteSendParam_Class, SetParameter, 1);
        HOOK(QuestCompleteSendParam_Class_SetParameter, SetParameter);

        auto CLASS(Monster_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::Monster});
        auto LoadMethod(Monster_Class, Start, 0);
        HOOK_P(Monster_Class_Start, Monster, Start);

        auto CLASS(TreasureBox_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::TreasureBox});
        auto LoadMethod(TreasureBox_Class, Start, 0);
        HOOK_P(TreasureBox_Class_Start, TreasureBox, Start);

        auto CLASS(BreakableObjectBase_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::BreakableObjectBase});
        auto LoadMethod(BreakableObjectBase_Class, Start, 0);
        HOOK_P(BreakableObjectBase_Class_Start, BreakableObjectBase, Start);

        auto CLASS(Player_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::Player});
        auto LoadMethod(Player_Class, Start, 0);
        HOOK_P(Player_Class_Start, Player, Start);
        auto LoadMethod(Player_Class, GetRunSpeed, 0);
        HOOK(Player_Class_GetRunSpeed, GetRunSpeed);
        auto LoadMethod(Player_Class, Damage, 1);
        HOOK(Player_Class_Damage, Damage);
        auto LoadMethod(Player_Class, SpDamage, 2);
        HOOK(Player_Class_SpDamage, SpDamage);
        auto LoadMethod(Player_Class, AddSp, 6);
        HOOK(Player_Class_AddSp, AddSp);

        auto CLASS(Character_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::Character});
        auto LoadMethod(Character_Class, IsDamageHit, 1);
        HOOK(Character_Class_IsDamageHit, IsDamageHit);

        auto CLASS(CatchParam_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::CatchParam});
        auto LoadMethod(CatchParam_Class, Catch, 1);
        HOOK(CatchParam_Class_Catch, Catch);

        auto CLASS(Coop_Model_Sp_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::Coop_Model_Sp});
        auto LoadMethod(Coop_Model_Sp_Class, _ctor, 2);
        HOOK_P(Coop_Model_Sp_Class__ctor, Coop_Model_Sp, _ctor);

        auto CLASS(UI_Talk_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::UI_Talk});
        auto LoadMethod(UI_Talk_Class, OpenWindow, 0);
        HOOK_P(UI_Talk_Class_OpenWindow, UI_Talk, OpenWindow);

        auto CLASS(CharacterController_Class,
                   Const::Assembly::UnityEngine_PhysicsModule_dll,
                   Const::NameSpaze::UnityEngine,
                   {Const::Klass::CharacterController});
        auto LoadMethod(CharacterController_Class, set_stepOffset, 1);
        HOOK(CharacterController_Class_set_stepOffset, set_stepOffset);
        auto LoadMethod(CharacterController_Class, set_slopeLimit, 1);
        HOOK(CharacterController_Class_set_slopeLimit, set_slopeLimit);

        auto CLASS(StageManager_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageManager});
        auto LoadMethod(StageManager_Class, SetupCamera, 2);
        HOOK(StageManager_Class_SetupCamera, SetupCamera);
        auto LoadMethod(StageManager_Class, IsBossMap, 0);
        HOOK(StageManager_Class_IsBossMap, IsBossMap);

        auto CLASS(StageData_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   {Const::Klass::StageData});
        auto LoadMethod(StageData_Class, get_IsBoss, 0);
        HOOK(StageData_Class_get_IsBoss, get_IsBoss);

        auto CLASS(UnitModel_Param_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::UnitModel, Const::Klass::Param));
        auto LoadMethod(UnitModel_Param_Class, get_TownAddHPRate, 0);
        HOOK(UnitModel_Param_Class_get_TownAddHPRate, get_TownAddHPRate);
        auto LoadMethod(UnitModel_Param_Class, get_TownAddSPRate, 0);
        HOOK(UnitModel_Param_Class_get_TownAddSPRate, get_TownAddSPRate);
        auto LoadMethod(UnitModel_Param_Class, GetBaseAtk, 0);
        HOOK(UnitModel_Param_Class_GetBaseAtk, GetBaseAtk);
        auto LoadMethod(UnitModel_Param_Class, get_TownAddDefRate, 0);
        HOOK(UnitModel_Param_Class_get_TownAddDefRate, get_TownAddDefRate);
        auto LoadMethod(UnitModel_Param_Class, get_TownAddHitRate, 0);
        HOOK(UnitModel_Param_Class_get_TownAddHitRate, get_TownAddHitRate);

        auto CLASS(Tutorual_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::Tutorual));
        auto LoadMethod(Tutorual_Class, get_CurrentStep, 0);
        HOOK(Tutorual_Class_get_CurrentStep, get_CurrentStep);

        auto CLASS(QuestPlayingDataController_Class,
                   Const::Assembly::Assembly_CSharp_dll,
                   Const::NameSpaze::empty,
                   KS(Const::Klass::QuestPlayingDataController));
        auto LoadMethod(QuestPlayingDataController_Class, AddTotalTakeDamageValue, 3);
        HOOK(QuestPlayingDataController_Class_AddTotalTakeDamageValue, AddTotalTakeDamageValue);

        /*HOOK(   Const::Assembly::Assembly_CSharp_dll,
        Const::NameSpaze::empty,
        KS(SAFE_CSTR(Const::Klass::Player)),
        UpdateDie, 1);*/

        /*HOOK(   Const::Assembly::Assembly_CSharp_dll,
            Const::NameSpaze::empty,
            KS(SAFE_CSTR(Const::Klass::Player)),
             OnForceKnockDownDamageReaction, 1);*/
        /*HOOK(   Const::Assembly::Assembly_CSharp_dll,
            Const::NameSpaze::empty,
            KS(SAFE_CSTR(Const::Klass::TournamentStateResult)),
            RequestRankingInfo, 2);*/
}

bool sub_hack_thread()
{
        LOGD("hack game thread: %d", getpid());

        void *handle = RET_FUNC2(dlopen, Const::General::libil2cpp_so, RTLD_NOLOAD);
        LOGD("handle: %p", handle);

        sleep(3);

        il2cpp_init_api(handle);

        AppInfo::isApiInited = true;

        LOGD("hack game il2cpp inited, %d", RET_FUNC1(isLibraryLoaded, Const::General::libil2cpp_so));

        Il2CppApi::Helper::init();

        load_class();
        load_methods();
        load_fields();
        load_hook();

        AppInfo::isHookInited = true;

        return true;
}
