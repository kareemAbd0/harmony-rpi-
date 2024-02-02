
cmake_minimum_required(VERSION 3.13)
project(harmony-rpi)
set(CMAKE_CXX_STANDARD 14)

find_package(PahoMqttCpp REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(OpenSSL REQUIRED)
add_executable(${PROJECT_NAME} main.cpp
        Client.cpp
        Client.h
        mycallback.cpp
        mycallback.h
        control.cpp
        control.h
        )
target_link_libraries(  ${PROJECT_NAME}  PahoMqttCpp::paho-mqttpp3)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
