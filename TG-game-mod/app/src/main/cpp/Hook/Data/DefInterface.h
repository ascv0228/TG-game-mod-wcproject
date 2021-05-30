#pragma once

DO_FUNC(void, WorldModel_Param_Create, (Il2CppObject * instance));

DO_FUNC(Il2CppObject *, get_CommonUI, (Il2CppObject * instance));
DO_FUNC(void, OpenDialog, (Il2CppObject * instance, int32_t type, Il2CppString *body, Il2CppObject *onComplete, bool isEncoding));

DO_FUNC(void, XorLong_Create, (Il2CppObject * instance, int64_t val));
DO_FUNC(int64_t, XorLong_Get, (Il2CppObject * instance));
DO_FUNC(void, XorLong_Set, (Il2CppObject * instance, int64_t value));

DO_FUNC(void, XorInt_Create, (Il2CppObject * instance, int32_t val));
DO_FUNC(int32_t, XorInt_Get, (Il2CppObject * instance));
DO_FUNC(void, XorInt_Set, (Il2CppObject * instance, int32_t value));

DO_FUNC(void, CompleteMissionSimple, (Il2CppObject * instance, bool isForceTalk));
DO_FUNC(void, CompleteMission, (Il2CppObject * instance));
DO_FUNC(bool, IsCoop, (Il2CppObject * instance));

DO_FUNC(void, set_CameraDegH, (Il2CppObject * instance, float value));
DO_FUNC(void, set_CameraDegV, (Il2CppObject * instance, float value));
DO_FUNC(void, set_CameraDistanceMax, (Il2CppObject * instance, float value));

DO_FUNC(void, set_PlayingTime, (Il2CppObject * instance, Il2CppObject *value));
DO_FUNC(void, set_RestTime, (Il2CppObject * instance, Il2CppObject *value));
DO_FUNC(void, set_QuestGameType, (Il2CppObject * instance, Il2CppObject *value));
DO_FUNC(void, set_ClearFloorNum, (Il2CppObject * instance, Il2CppObject *value));
DO_FUNC(void, set_TotalActionTime, (Il2CppObject * instance, Il2CppObject *value));
DO_FUNC(void, set_TotalFoundByEnemyCount, (Il2CppObject * instance, Il2CppObject *value));

DO_FUNC(bool, IsEnemy, (Il2CppObject * instance));

DO_FUNC(int32_t, GetTakeActionPlayerId, (Il2CppObject * instance, Il2CppObject *taker));

DO_FUNC(void, AddEnemies, (Il2CppObject * instance, int32_t id, int32_t conditionType));
DO_FUNC(void, AddTreasures, (Il2CppObject * instance, int32_t id, int32_t conditionType));
DO_FUNC(void, AddObjects, (Il2CppObject * instance, int32_t id, int32_t conditionType));
DO_FUNC(int32_t, get_CurrentCharacterSlotNo, (Il2CppObject * instance));
DO_FUNC(int32_t, get_PlayerSlotNo, (Il2CppObject * instance));
DO_FUNC(Il2CppString *, GetLocalAccessId, (Il2CppObject * instance));
DO_FUNC(Il2CppString *, get_from_user_id, (Il2CppObject * instance));

DO_FUNC(void, Cookie_setCookieToken, (Il2CppString * cookieToken));
DO_FUNC(void, OutGameManager_ChangeScene, (Il2CppObject * instance, int32_t scene, bool forceChange));
DO_FUNC(void, GlobalDataManager_CreateSaveData, (Il2CppObject * instance));
DO_FUNC(void, SaveData_SetString, (int32_t key, Il2CppString *val));

DO_VAR(CoopManager_Fields, cm_fields);
DO_VAR(CoopContributeLogModel_Request_Fields, contribute_log_fields);
DO_VAR(InGameManager_Fields, igm_fields);
DO_VAR(InGameManager_Result_Fields, igm_result_fields);
DO_VAR(QuestGenerateModel_Param_Fields, qp_fields);
DO_VAR(QuestGenerateModel_RankingEventInfo_Fields, qr_fields);
DO_VAR(QuestGenerateModel_RankingEventInfo_Ghost_Fields, rank_ghost_fields);
DO_VAR(QuestGenerateModel_EnemyParamBox_Fields, param_box_fields);
DO_VAR(QuestGenerateModel_Stage_Fields, qs_fields);
DO_VAR(QuestGenerateModel_Stage_EnemyParam_Fields, enemy_param_fields);
DO_VAR(QuestGenerateModel_Stage_Enemy_Fields, enemy_fields);
DO_VAR(QuestGenerateModel_Stage_TreasureBox_Fields, treasure_box_fields);
DO_VAR(QuestGenerateModel_Stage_DropInfo_Fields, drop_fields);
DO_VAR(QuestGenerateModel_Stage_PlacementObject_Fields, object_fields);
DO_VAR(QuestCompleteSendParam_Fields, complete_fields);
DO_VAR(QuestRankingModel_SendParam_Fields, ranking_fields);
DO_VAR(QuestGenerateModel_TimeBorder_Fields, time_border_fields);
DO_VAR(QuestGenerateModel_ScoreBorder_Fields, score_border_fields);
DO_VAR(QuestPlayingDataController_QuestPlayingData_Fields, quest_playing_data_fields);
DO_VAR(QuestPlayingDataController_Fields, quest_playing_data_ctrl_fields);
DO_VAR(GlobalDataManager_Fields, gdm_fields);
DO_VAR(GlobalDataManager_CommonSaveData_Fields, gdm_save_fields);
DO_VAR(GlobalDataManager_CommonSaveData_Read_Fields, gdm_save_read_fields);
DO_VAR(GlobalDataManager_Account_Fields, gdm_acc_fields);
DO_VAR(PlayerModel_Param_Fields, player_param_fields);
DO_VAR(UserInfoModel_Param_Fields, user_info_fields);
DO_VAR(SaveData_Key_Fields, sv_key_fields);

DO_VAR(Il2CppClass *, UI_Common_Class);
DO_VAR(Il2CppClass *, Action_Class);
DO_VAR(Il2CppClass *, XorInt_Class);
DO_VAR(Il2CppClass *, XorLong_Class);
DO_VAR(Il2CppClass *, WorldModel_Param_Class);
DO_VAR(FieldInfo *, KtbWebSocket_inst);
DO_VAR(FieldInfo *, GlobalDataManager_inst);
DO_VAR(FieldInfo *, OutGameManager_inst);
DO_VAR(FieldInfo *, UIManager_inst);
DO_VAR(FieldInfo *, InGameManager_inst);
DO_VAR(FieldInfo *, StageManager_inst);
DO_VAR(FieldInfo *, CoopManager_inst);
