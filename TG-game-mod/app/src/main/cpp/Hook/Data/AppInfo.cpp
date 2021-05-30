#include "AppInfo.h"
#include "../../Const/Const_Field.h"

bool AppInfo::isAppLoaded = false;
bool AppInfo::isApiInited = false;
bool AppInfo::isHookInited = false;
std::string AppInfo::version = "";
bool AppInfo::isSendWin = false;
int64_t AppInfo::il2cpp_base = -1;