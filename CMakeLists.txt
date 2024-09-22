cmake_minimum_required(VERSION 3.15)
project(CrowSample)

# Specify C++11 (or higher) standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(CrowSample main.cpp)

# Add the include directories for ASIO and Crow
target_include_directories(CrowSample PUBLIC /usr/local/include /Users/casr/crowcpp/Crow-1.2.0/include)

# Link pthread for multi-threading (required by Crow and ASIO)
target_link_libraries(CrowSample PUBLIC pthread)
