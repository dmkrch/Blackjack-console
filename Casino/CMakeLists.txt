cmake_minimum_required(VERSION 3.15)
project(Blackjack)

set(CMAKE_CXX_STANDARD 14)

include_directories(../Common)

file (GLOB SRC_FILES *.cpp ../Common/*.cpp)

add_executable(casino ${SRC_FILES})

TARGET_LINK_LIBRARIES( casino
pthread
)