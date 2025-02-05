FetchContent_Declare(
    DirectX-Headers
    GIT_REPOSITORY https://github.com/Microsoft/DirectX-Headers
    GIT_TAG v1.615.0
)

set(target_name "Coplt_Graphics_Native_D3d12")

add_library(${target_name} SHARED
    Src/Library.cc
    Src/Instance.cc
    Src/Instance.h
    FFI/Library.h
    FFI/Instance.h
    FFI/Device.h
    Src/Device.cc
    Src/Device.cc
    Src/Device.h
)
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "Coplt.Graphics.Native.D3d12")
add_library(Coplt::Graphics::D3d12 ALIAS ${target_name})
target_compile_definitions(${target_name} PRIVATE -DFFI_SRC)

set(BUILD_XAUDIO_WIN10 OFF)
set(BUILD_DXIL_SHADERS OFF)

FetchContent_MakeAvailable(DirectX-Headers)
add_subdirectory(ThirdParty/D3D12MemoryAllocator)
add_subdirectory(ThirdParty/DirectXTK12)

target_link_libraries(${target_name}
    PUBLIC
    Coplt::Graphics::Api
    Microsoft::DirectX-Headers
    DirectXTK12
    D3D12MemoryAllocator
)

