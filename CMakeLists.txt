
cmake_minimum_required(VERSION 3.13)
project(harmony_rpi)
set(CMAKE_CXX_STANDARD 14)

find_package(PahoMqttCpp REQUIRED)
add_executable(harmony_rpi main.cpp
        client.cpp
        Client.h
        mycallback.cpp
        mycallback.h)
target_link_libraries(  harmony_rpi  PahoMqttCpp::paho-mqttpp3)
