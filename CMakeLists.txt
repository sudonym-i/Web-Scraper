# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(webscrape.exe)

# Add executable target (replace main.cpp with your actual source files)
add_executable(webscrape.exe main.cpp scraper.cpp)

# Specify where to find dependencies from vcpkg (if not set globally)
set(CMAKE_TOOLCHAIN_FILE /home/isaac/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "vcpkg toolchain file")



# Find dependencies
find_package(CURL REQUIRED)

# Link dependencies to your project
target_link_libraries(webscrape.exe CURL::libcurl)
