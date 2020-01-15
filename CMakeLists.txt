
cmake_minimum_required(VERSION 3.15)

project(spl_net_Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 11)

link_libraries("-lboost_system")

add_executable(spl_net_Client
        include/connectionHandler.h
        include/ClientDB.h
        include/KeyboardListenerTask.h
        include/Protocol.h
        include/ServerListenerTask.h
        src/connectionHandler.cpp
        src/ClientDB.cpp
        src/KeyboardListenerTask.cpp
        src/Protocol.cpp
        src/ServerListenerTask.cpp
        src/StompBookClubClient.cpp
        CMakeLists.txt
        )