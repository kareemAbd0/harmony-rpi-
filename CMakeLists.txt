cmake_minimum_required(VERSION 3.13)
project(harmony_rpi)
set(CMAKE_CXX_STANDARD 14)

# Find packages
find_package(PahoMqttCpp REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}
      callback
      client
      control
  )

# Add your source files
add_executable(${PROJECT_NAME} 
    main.cpp
    callback/mycallback.cpp
    client/client.cpp
    control/control.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PahoMqttCpp::paho-mqttpp3)

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
