cmake_minimum_required(VERSION 3.30)
project(nanosffi_static)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE STATIC_SOURCES src/*.cpp)
file(GLOB_RECURSE STATIC_HEADERS include/*.h include/*.hpp)

# Create the static library
add_library(nanosffi_static STATIC ${STATIC_SOURCES} include/nanosffi/api.h src/api.cpp src/binder.h)

# Include paths
target_include_directories(nanosffi_static
        PUBLIC ${CMAKE_SOURCE_DIR}/include
        PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# Installation
install(TARGETS nanosffi_static
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib
        PUBLIC_HEADER DESTINATION include
)

set_target_properties(nanosffi_static PROPERTIES
        PUBLIC_HEADER "${STATIC_HEADERS}"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist
)

# 🔹 Force static linking on MSVC
if (MSVC)
    set_target_properties(nanosffi_static PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

# 🔹 Force static linking on MinGW
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -static")
endif()

# 🔹 If on windows, link against deps/lib/minhook.x64.lib and include deps/include/win
if (WIN32)
    target_link_libraries(nanosffi_static PRIVATE ${CMAKE_SOURCE_DIR}/deps/lib/minhook.x64.lib)
    target_include_directories(nanosffi_static PRIVATE ${CMAKE_SOURCE_DIR}/deps/include/win)
endif()