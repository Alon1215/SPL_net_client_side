cmake_minimum_required(VERSION 3.15)
project(spl_net_Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(spl_net_Client main.cpp src/echoClient.cpp src/StompBookClubClient.cpp src/connectionHandler.cpp src/ClientDB.cpp include/ClientDB.h src/ServerListenerTask.cpp include/ServerListenerTask.h include/Protocol.cpp include/Protocol.h include/Database.cpp include/Database.h)