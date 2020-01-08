cmake_minimum_required(VERSION 3.15)
project(spl_net_Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(spl_net_Client main.cpp src/echoClient.cpp src/connectionHandler.cpp)