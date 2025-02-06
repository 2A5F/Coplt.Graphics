#pragma once

#include <directx/dxgiformat.h>

#include "../../Api/FFI/TextureFormat.h"

namespace Coplt {

    inline DXGI_FORMAT ToDXGIFormat(FTextureFormat value)
    {
        return static_cast<DXGI_FORMAT>(value);
    }

}
