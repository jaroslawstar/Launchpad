cmake_minimum_required(VERSION 3.31.6)
project(Launchpad VERSION 2.0 LANGUAGES C CXX OBJC OBJCXX)

set(SOURCE_FILES src/main.cpp src/header.h src/BlurView.mm)  # Define source files

# Enforce the C++ standard, and disable extensions.
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()


set(CMAKE_C_STANDARD 11)
#set(CMAKE_BUILD_TYPE Release)         # Specify build type (Debug/Release)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

# Enable ARC and Objective-C++ flags for .mm files
set_source_files_properties(
    BlurView.mm
    PROPERTIES
        COMPILE_FLAGS "-fobjc-arc -ObjC++"
)

# Find Qt
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Find SFML package
find_package(SFML 3.0.0 REQUIRED COMPONENTS Graphics Window System Audio Network)

# Find Boost
find_package(Boost REQUIRED COMPONENTS filesystem system)

# Find lz4
find_package(PkgConfig REQUIRED)
pkg_check_modules(lz4 REQUIRED IMPORTED_TARGET liblz4)

# Find mongo-cxx

# Find LibreSSL
find_package(LibreSSL REQUIRED)

# Add the executable
add_executable(Launchpad ${SOURCE_FILES})  # Create an executable




# Link libraries
target_link_libraries(Launchpad
    PUBLIC Qt6::Widgets
    PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio
    PUBLIC Boost::filesystem Boost::system
    PUBLIC PkgConfig::lz4
    PUBLIC LibreSSL::Crypto LibreSSL::TLS LibreSSL::SSL
    "-framework Cocoa"
    "-framework AppKit"

)

add_custom_target(run
    COMMAND Launchpad
    DEPENDS Launchpad
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

set_target_properties(Launchpad PROPERTIES
    CXX_STANDARD 20
    OUTPUT_NAME "Launchpad"
)
