cmake_minimum_required(VERSION 3.10)
project(Axis)

set(CMAKE_CXX_STANDARD 11)

add_executable(Axis_server server/main.cpp server/init.h server/signalHandler.h server/messageHandler.h)

add_executable(Axis_cli cli/main.cpp cli/init.h cli/sendMessage.h)