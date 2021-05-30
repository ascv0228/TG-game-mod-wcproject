#pragma  once

#include <cstdint>

#include "il2cpp-types.h"

#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API
