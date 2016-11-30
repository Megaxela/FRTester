cmake_minimum_required(VERSION 3.6)
project(FRTester)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../${CMAKE_PROJECT_NAME}_executable")
set(CMAKE_CXX_STANDARD 11)

#------------------------
# Setuping Qt
#------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

include_directories(driver)

#-----------------------
# Files
#-----------------------
qt5_wrap_ui(
        UIS_HDRS
        ui/mainwindow.ui
)


set(SOURCE_FILES main.cpp
        include/MainWindow.cpp
        include/MainWindow.h
        include/DriverHolder.cpp
        include/DriverHolder.h)

add_executable(FRTester ${SOURCE_FILES} ${UIS_HDRS})
qt5_use_modules(FRTester Core Widgets Gui)
target_link_libraries(FRTester ${Qt5Widgets_LIBRARIES})