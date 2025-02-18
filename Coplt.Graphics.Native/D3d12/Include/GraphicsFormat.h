#pragma once

#include <directx/dxgiformat.h>

#include "../../Api/FFI/GraphicsFormat.h"

namespace Coplt {

    inline DXGI_FORMAT ToDx(FGraphicsFormat value)
    {
        return static_cast<DXGI_FORMAT>(value);
    }

}
