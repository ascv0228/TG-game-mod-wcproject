#pragma once

class Config
{
public:
    static bool isHack;
    static bool isEnterWin;
    static bool isSkipTalk;
    static bool isSkipTutorial;
    static int dropRate;
    static bool isKillEnemy;
    static bool isInvincible;
    static int abilityRate;
    static int speedRate;
    static bool isGodMode;
    static int addTime;
    static int Injection_Type;
    static int Injection_Id;
    static int Injection_Num;

    // 進階設定
    static bool isEnableAdv;
    static bool isCloseEnemyKill;
    static bool isCloseTreasureKill;
    static bool isCloseObjectKill;
    static int advDropRate;
    static int advAbilityRate;
    static bool isCloseHPRate;
    static bool isCloseSPRate;
    static bool isCloseATKRate;
    static bool isCloseDEFRate;
    static bool isCloseHITRate;
    static bool isHideDamage;
    static bool isInfiniteSp;
};
