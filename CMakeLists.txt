cmake_minimum_required(VERSION 3.20)
project(toon LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically include all .cpp files in src/
file(GLOB SOURCES src/*.cpp)

# Create the executable
add_executable(toon ${SOURCES})

# Enforce C++17 per target (bulletproof)
target_compile_features(toon PRIVATE cxx_std_17)

# Optional: include headers for IDEs
target_include_directories(toon PRIVATE include)
