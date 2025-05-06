cmake_minimum_required(VERSION 3.15)
project(ComputerClub)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(ComputerClub
        src/main.cpp
        src/Event.cpp
        src/Client.cpp
        src/Table.cpp
        src/ComputerClub.cpp
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(ComputerClubTests
        tests/tests.cpp
        src/Event.cpp
        src/Client.cpp
        src/Table.cpp
        src/ComputerClub.cpp
)

target_link_libraries(ComputerClubTests GTest::GTest GTest::Main pthread)

add_test(NAME ComputerClubTests COMMAND ComputerClubTests)