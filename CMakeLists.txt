cmake_minimum_required(VERSION 3.14)

project(CpuMonitoringClient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

add_executable(CpuMonitoringClient
  main.cpp
  TcpClient.h
  TcpClient.cpp
  WindowsHardwareInfo.h
  WindowsHardwareInfo.cpp
)
target_link_libraries(CpuMonitoringClient Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

# Dependencies
# System Libraries
if (WIN32)
    target_link_libraries(CpuMonitoringClient "WbemUuid.lib" "Ole32" "credui" ) # "comsuppw" )
endif ()

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Zc:strictStrings")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXXFLAGS} -Zc:strictStrings")

#message(CMAKE_C_FLAGS=${CMAKE_C_FLAGS})
#message(CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})
