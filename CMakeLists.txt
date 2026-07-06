cmake_minimum_required(VERSION 3.20)
project(toon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static library — everything except main.cpp
add_library(toon_lib STATIC
    src/error.cpp
    src/json.cpp
    src/lexer.cpp
    src/parser.cpp
    src/serializer.cpp
    src/token.cpp
    src/value.cpp
)

# PUBLIC so consumers of toon_lib get the include path automatically
target_include_directories(toon_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(toon_lib PUBLIC cxx_std_17)

# CLI executable
add_executable(toon src/main.cpp)
target_link_libraries(toon PRIVATE toon_lib)
