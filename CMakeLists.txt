cmake_minimum_required(VERSION 3.29)

include(FetchContent)
FetchContent_Declare(
    cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG v0.7.5
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.1.3
)
FetchContent_Declare(
    mimalloc
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v2.1.7
)

project(Coplt_Graphics_Native)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-march=skylake)
add_definitions(-Wno-everything)
add_definitions(-DCPPTRACE_STATIC_DEFINE)
add_definitions(-DGLM_FORCE_INLINE)
add_definitions(-DGLM_FORCE_ALIGNED_GENTYPES)
add_definitions(-DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES)
add_definitions(-DGLM_FORCE_INTRINSICS)
add_definitions(-DGLM_FORCE_SWIZZLE)
add_definitions(-DGLM_FORCE_LEFT_HANDED)
add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
add_definitions(-DGLM_FORCE_UNRESTRICTED_GENTYPE)
add_definitions(-DGLM_FORCE_QUAT_DATA_XYZW)

set(MI_OVERRID OFF)

FetchContent_MakeAvailable(cpptrace)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(mimalloc)

add_subdirectory(Coplt.Graphics.Native/Api)
add_subdirectory(Coplt.Graphics.Native/D3d12)
