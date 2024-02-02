
cmake_minimum_required(VERSION 3.13)
project(rpi)
set(CMAKE_CXX_STANDARD 14)

find_package(PahoMqttCpp REQUIRED)
add_executable(rpi main.cpp
        Client.cpp
        Client.h
        mycallback.cpp
        mycallback.h
        control.cpp
        control.h
        )
target_link_libraries(  rpi  PahoMqttCpp::paho-mqttpp3)
install(TARGETS rpi DESTINATION ${CMAKE_INSTALL_BINDIR})
