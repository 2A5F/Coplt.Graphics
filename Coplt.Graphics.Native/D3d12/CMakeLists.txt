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
    Include/Utils.h
    FFI/Queue.h
    Src/Queue.cc
    Src/Queue.h
    FFI/Output.h
    Src/Output.cc
    Src/Output.h
    Include/TextureFormat.h
    Src/Command.cc
    Src/Command.h
    FFI/Pipeline.h
    Src/GraphicsPipeline.h
    Src/GraphicsPipeline.cc
    FFI/Layout.h
    Src/Layout.h
    Src/Layout.cc
    Include/ShaderVisibility.h
    Include/PipelineState.h
    Src/CmdListPack.h
)
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "Coplt.Graphics.Native.D3d12")
add_library(Coplt::Graphics::D3d12 ALIAS ${target_name})
target_compile_definitions(${target_name} PRIVATE -DFFI_SRC -DNOMINMAX)

set(BUILD_XAUDIO_WIN10 OFF)
set(BUILD_DXIL_SHADERS OFF)

add_subdirectory(ThirdParty/DirectX-Headers)
add_subdirectory(ThirdParty/D3D12MemoryAllocator)
add_subdirectory(ThirdParty/DirectXTK12)

target_link_libraries(${target_name}
    PUBLIC
    Coplt::Graphics::Api
    Microsoft::DirectX-Headers
    DirectXTK12
    D3D12MemoryAllocator
)
