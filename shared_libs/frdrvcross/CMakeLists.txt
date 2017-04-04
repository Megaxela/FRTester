cmake_minimum_required(VERSION 3.6)
project(frdrvcross)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -pthread )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../FRTester_executable")

include_directories(include)

set(MAIN_FILE
        include/main.cpp)

set(HEADER_FILES
        include/FRDriver.h
        include/Tools/TTY.h
        include/Tools/Time.h
        include/Tools/byte.h
        include/Tools/Logger.h
        include/Tools/Platform.h
        include/Tools/ByteArray.h
        include/Tools/SystemTools.h
        include/Tools/ByteArrayReader.h
        include/Tools/NetworkTools.h
        include/Tools/IPv4Address.h
        include/Abstract/Protocol.h
        include/Abstract/PhysicalInterface.h
        include/Implementation/COMInterface.h
        include/Implementation/TCPInterface.h
        include/Implementation/TestProtocol.h
        include/Implementation/DefaultProtocol.h
        cython/driver.h)

set(SOURCE_FILES
        src/Tools/TTY.cpp
        src/Tools/NetworkTools.cpp
        src/Tools/ByteArray.cpp
        src/Tools/SystemTools.cpp
        src/FRDriver.cpp
        src/Abstract/Protocol.cpp
        src/Tools/Logger.cpp
        src/Tools/IPv4Address.cpp
        src/Tools/ByteArrayReader.cpp
        src/Abstract/PhysicalInterface.cpp
        src/Implementation/COMInterface.cpp
        src/Implementation/TCPInterface.cpp
        src/Implementation/DefaultProtocol.cpp
        src/Tools/Time.cpp
        src/Implementation/TestProtocol.cpp
)

add_library(frdrvcross SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

#set_target_properties(frdrvcross PROPERTIES COMPILE_FLAGS "-m32 " LINK_FLAGS "-m32")