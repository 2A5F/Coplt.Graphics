#pragma once

#include "Common.h"
#include "String.h"
#include "Types.h"

namespace Coplt
{
    enum class FLogLevel : u32
    {
        Fatal = 0,
        Error,
        Warning,
        Info,
        Debug,
        Trace,
    };

    enum class FLogType : u32
    {
        Common,
        DirectX,
    };

    using FLoggerEnableFn = b8 COPLT_CDECL(void* object, FLogLevel level, FLogType type);
    using FLoggerFn = void COPLT_CDECL(void* object, FLogLevel level, FLogType type, const FMessage& msg);
    using FLoggerDropFn = void COPLT_CDECL(void* object);

    struct FLogger
    {
        void* p_object{};
        FLoggerEnableFn* pf_enable{};
        FLoggerFn* pf_logger{};
        FLoggerDropFn* pf_drop{};
    };
}
