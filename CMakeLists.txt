cmake_minimum_required(VERSION 3.30)

include(FetchContent)
FetchContent_Declare(
    cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG v0.7.5 # <HASH or TAG>
)

project(Coplt_Graphics_Native)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-march=skylake)
add_definitions(-DCPPTRACE_STATIC_DEFINE)
add_definitions(-Wno-everything)

FetchContent_MakeAvailable(cpptrace)

add_subdirectory(Coplt.Graphics.Native/Api)
