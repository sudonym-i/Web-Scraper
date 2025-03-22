# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(webCrawler)

# Add executable target (replace main.cpp with your actual source files)
add_executable(MyProject scraper.cpp)

# Specify where to find dependencies from vcpkg (if not set globally)
# Replace with the correct path to your vcpkg directory
set(CMAKE_TOOLCHAIN_FILE /home/isaac/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "vcpkg toolchain file")

# Find dependencies (For example, Boost)
#find_package(curl REQUIRED)
#find_package(libxml2 REQUIRED)


# Find dependencies
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

# Link dependencies to your project
target_link_libraries(MyProject CURL::libcurl LibXml2::LibXml2)


# Link the Boost libraries to your project
#target_link_libraries(MyProject Boost::Boost)
