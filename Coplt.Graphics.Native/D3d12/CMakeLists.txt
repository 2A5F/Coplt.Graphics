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
    Src/Output.cc
    Src/Output.h
    Include/GraphicsFormat.h
    FFI/Pipeline.h
    Src/GraphicsPipeline.h
    Src/GraphicsPipeline.cc
    Src/Layout.h
    Src/Layout.cc
    Include/ShaderVisibility.h
    Include/PipelineState.h
    Src/Buffer.h
    Src/Buffer.cc
    FFI/Resource.h
    Src/Resource.h
    Src/Resource.cc
    Src/Context.h
    Src/Context.cc
    Include/States.h
    Src/Binding.h
    Src/Binding.cc
    Include/View.h
    FFI/Adapter.h
    Src/Adapter.h
    Src/Adapter.cc
    Src/View.cc
    Src/Image.h
    Src/Image.cc
    Include/Resource.h
    FFI/Sampler.h
    Src/Sampler.h
    Src/Sampler.cc
    Include/Sampler.h
    Src/Isolate.h
    Src/Isolate.cc
    Src/Record.h
    Src/Record.cc
    Src/SwapChain.h
    Src/SwapChain.cc
    Src/Barrier.h
    Src/Barrier.cc
    Include/ResState.h
    Src/ResState.cc
    Src/DescriptorManager.h
    Src/DescriptorManager.cc
)
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "Coplt.Graphics.Native.D3d12")
add_library(Coplt::Graphics::D3d12 ALIAS ${target_name})
target_compile_definitions(${target_name} PRIVATE -DFFI_SRC -DNOMINMAX -DUSE_PIX)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/Include/WinPixEventRuntime)

set(BUILD_XAUDIO_WIN10 OFF)
set(BUILD_DXIL_SHADERS OFF)

add_subdirectory(ThirdParty/DirectX-Headers)
add_subdirectory(ThirdParty/D3D12MemoryAllocator)

target_link_libraries(${target_name}
    PUBLIC
    Coplt::Graphics::Api
    Microsoft::DirectX-Headers
    D3D12MemoryAllocator
)

target_link_libraries(${target_name}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.lib
)
