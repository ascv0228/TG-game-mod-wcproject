#pragma once

#include "Data/Defs.h"

#define HOOK_DEF(ret, func, ...)  \
  ret (*old_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)

#define HOOK_DEC(ret, func, ...)         \
  extern ret (*old_##func)(__VA_ARGS__); \
  extern ret new_##func(__VA_ARGS__)

HOOK_DEC(void, StageManagerStateStageClear_Start, Il2CppObject *instance);

HOOK_DEC(void, OpenDialog, Il2CppObject *instance, int32_t type, Il2CppString *body, Il2CppObject *onComplete, bool isEncoding);

HOOK_DEC(void, StageManagerStateStageIn_Start, Il2CppObject *instance);
HOOK_DEC(void, SetParameter, Il2CppObject *instance, Il2CppObject *result);
HOOK_DEC(void, UI_Talk_OpenWindow, Il2CppObject *instance);

HOOK_DEC(void, BreakableObjectBase_Start, Il2CppObject *instance);
HOOK_DEC(void, TreasureBox_Start, Il2CppObject *instance);
HOOK_DEC(void, Monster_Start, Il2CppObject *instance);
HOOK_DEC(void, Player_Start, Il2CppObject *instance);
HOOK_DEC(bool, IsBossMap, Il2CppObject *instance);
HOOK_DEC(bool, get_IsBoss, Il2CppObject *instance);

HOOK_DEC(float, GetRunSpeed, Il2CppObject *instance);

HOOK_DEC(int32_t, Damage, Il2CppObject *instance, Il2CppObject *damage);
HOOK_DEC(void, SpDamage, Il2CppObject *instance, Il2CppObject *spDamageData, bool damageReserves);
HOOK_DEC(bool, IsDamageHit, Il2CppObject *instance, Il2CppObject *ac);
HOOK_DEC(bool, Catch, Il2CppObject *instance, Il2CppObject *target);
HOOK_DEC(bool, UpdateDie, Il2CppObject *instance, Il2CppObject *damage);
HOOK_DEC(void, OnForceKnockDownDamageReaction, Il2CppObject *instance, Il2CppObject *damage);

HOOK_DEC(void, Coop_Model_Sp__ctor, Il2CppObject *instance, int32_t value, bool isDownMaxSp);
HOOK_DEC(int32_t, AddSp, Il2CppObject *instance, int32_t num, bool fouceHeal, bool isStopRecoveryLimiter, bool isJobHandicap, bool isDrawFont, int32_t healCondition);

HOOK_DEC(bool, SetupCamera, Il2CppObject *instance, Il2CppObject *data, float distance);
HOOK_DEC(void, set_stepOffset, Il2CppObject *instance, float value);
HOOK_DEC(void, set_slopeLimit, Il2CppObject *instance, float value);

HOOK_DEC(float, get_TownAddHPRate, Il2CppObject *instance);
HOOK_DEC(float, get_TownAddSPRate, Il2CppObject *instance);
HOOK_DEC(int32_t, GetBaseAtk, Il2CppObject *instance);
HOOK_DEC(float, get_TownAddDefRate, Il2CppObject *instance);
HOOK_DEC(float, get_TownAddHitRate, Il2CppObject *instance);
HOOK_DEC(float, get_TownAddAgiRate, Il2CppObject *instance);

HOOK_DEC(int32_t, get_CurrentStep, Il2CppObject *instance);

HOOK_DEC(void, RequestRankingInfo, Il2CppObject *instance, Il2CppObject *sendParam, Il2CppObject *onComplete);

HOOK_DEC(void, AddTotalTakeDamageValue, Il2CppObject *instance, Il2CppObject *owner, Il2CppObject *attacker, Il2CppObject *damage);
