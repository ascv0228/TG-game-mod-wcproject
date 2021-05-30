#include <dlfcn.h>

#include <dobby.h>
#include <Logger.h>
#include <obfuscate.hpp>
#include <string>
#include <BridgeFunc.h>

#include "il2cpp-appdata.h"
#include "il2cpp-init.h"
#include "../Const/Const_Il2cpp.h"
#include "../Const/Const_General.h"

#define DO_API(r, n, p) r(*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

Il2CppDefaults il2cpp_defaults = {};

void il2cpp_init_api(void *il2cpp_handle)
{
    if (il2cpp_handle)
    {
#define DO_API(r, n, p) n = (r(*) p)RET_FUNC2(dlsym, il2cpp_handle, Const::Il2cpp::n)
#include "il2cpp-api-functions.h"
#undef DO_API
    }
    else
    {

#define DO_API(r, n, p) n = (r(*) p)RET_FUNC2(DobbySymbolResolver, Const::General::libil2cpp_so, Const::Il2cpp::n)
#include "il2cpp-api-functions.h"
#undef DO_API
    }
}