#pragma  once

#include "Parameters.h"
#include "../../Il2cpp/il2cpp-types.h"

#define DO_FUNC(r, n, p) extern r (*n) p
#define DO_VAR(r, n) extern r n

#include "DefInterface.h"

#undef DO_VAR
#undef DO_FUNC