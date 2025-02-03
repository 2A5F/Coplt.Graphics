cmake_minimum_required(VERSION 3.30)

set(target_name "Coplt_Graphics_Native_Api")

add_library(${target_name} STATIC
    FFI/Common.h
    FFI/Object.h
    Include/Object.h
    Src/Object.cc
    FFI/String.h
    Src/String.cc
    Src/String.h
    FFI/Instance.h
    FFI/Types.h
    FFI/Device.h
)
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
add_library(Coplt::Graphics::Api ALIAS ${target_name})
target_compile_definitions(${target_name} PRIVATE -DFFI_SRC)
target_link_libraries(${target_name}
    PUBLIC
    cpptrace::cpptrace
    glm::glm
    mimalloc-static
)
