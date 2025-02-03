cmake_minimum_required(VERSION 3.30)

set(target_name "Coplt_Graphics_Native_Api")

add_library(${target_name} STATIC
    Common.h
    Object.h
)
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
add_library(Coplt::Graphics::Api ALIAS ${target_name})
target_link_libraries(${target_name} PUBLIC cpptrace::cpptrace)
