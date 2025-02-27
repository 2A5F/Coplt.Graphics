#pragma once

#include "../../Api/FFI/Instance.h"

extern "C" COPLT_API void Coplt_Graphics_D3d12_Create_Instance(Coplt::FResult* r, const Coplt::FInstanceCreateOptions& options, Coplt::FInstance** out);
