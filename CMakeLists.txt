cmake_minimum_required(VERSION 3.20)

project(MatchLab
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(matchlab_core
    src/Match.cpp
    src/Team.cpp
    src/League.cpp
    src/CsvMatchReader.cpp
)

target_include_directories(matchlab_core PUBLIC include)

include(CTest)

if(BUILD_TESTING)
    add_executable(matchlab_tests tests/LeagueTests.cpp)
    target_link_libraries(matchlab_tests PRIVATE matchlab_core)
    add_test(NAME matchlab_tests COMMAND matchlab_tests)
endif()
