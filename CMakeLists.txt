cmake_minimum_required(VERSION 3.30)
project(FlappyBara)

set(CMAKE_CXX_STANDARD 26)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Make sure Cmake knows where to look for includes in our project
include_directories(${CMAKE_SOURCE_DIR}/includes ${CMAKE_SOURCE_DIR}/vendor)

# Remove console for Release builds
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} WIN32
            src/main.cpp
            includes/main.hpp
            includes/constants.hpp
            src/AudioResourceManager.cpp
            includes/AudioResourceManager.hpp
            src/Game.cpp
            includes/Game.hpp
            src/TextureResourceManager.cpp
            includes/TextureResourceManager.hpp
            src/Logger.cpp
            includes/Logger.hpp
    )
else()
    add_executable(${PROJECT_NAME}
            src/main.cpp
            includes/main.hpp
            includes/constants.hpp
            src/AudioResourceManager.cpp
            includes/AudioResourceManager.hpp
            src/Game.cpp
            includes/Game.hpp
            src/TextureResourceManager.cpp
            includes/TextureResourceManager.hpp
            src/Logger.cpp
            includes/Logger.hpp
    )
endif()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)