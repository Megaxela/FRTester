cmake_minimum_required(VERSION 3.6)
project(FRTester)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../${CMAKE_PROJECT_NAME}_executable")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread -D_hypot=hypot")

include_directories(${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

message("${CMAKE_LIBRARY_PATH}")

#------------------------
# Setuping Qt
#------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

include_directories(include)

#-----------------------
# Files
#-----------------------
qt5_wrap_ui(
        UIS_HDRS
        ui/mainwindow.ui
)


set(QT_RESOURCES
        res/resources.qrc)

qt5_add_resources(RESOURCE_ADDED ${QT_RESOURCES})

set(CYTHON_FILES
        cython/frdriver.h
        cython/frdriver.cpp)

set(DRIVER_FILES
        include/FRDriver.h
        include/Tools/TTY.h
        include/Tools/Time.h
        include/Tools/byte.h
        include/Tools/Logger.h
        include/Tools/Platform.h
        include/Tools/ByteArray.h
        include/Tools/SystemTools.h
        include/Abstract/Protocol.h
        include/Abstract/PhysicalInterface.h
        include/Implementation/COMInterface.h
        include/Implementation/TestProtocol.h
        include/Implementation/DefaultProtocol.h
        src/Tools/TTY.cpp
        src/Tools/ByteArray.cpp
        src/Tools/SystemTools.cpp
        src/FRDriver.cpp
        src/Abstract/Protocol.cpp
        src/Tools/Logger.cpp
        src/Abstract/PhysicalInterface.cpp
        src/Implementation/COMInterface.cpp
        src/Implementation/DefaultProtocol.cpp
        src/Tools/Time.cpp
        src/Implementation/TestProtocol.cpp
        )

set(SOURCE_FILES main.cpp
        src/Windows/MainWindow.cpp
        include/Windows/MainWindow.h
        src/DriverHolder.cpp
        include/DriverHolder.h
        src/Windows/Controllers/ConnectControllers/ConnectCOMTabController.cpp
        include/Windows/Controllers/ConnectControllers/ConnectCOMTabController.h
        src/Windows/Controllers/AbstractTabController.cpp
        include/Windows/Controllers/AbstractTabController.h
        src/Windows/Controllers/CommandsTabController.cpp
        include/Windows/Controllers/CommandsTabController.h
        src/Windows/Controllers/CommandsControllers/StateCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/StateCommandsTabController.h
        src/Tools/StableController.cpp
        include/Tools/StableController.h
        src/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h
        src/Tools/TypeConverters.cpp
        include/Tools/TypeConverters.h
        src/Testing/TestCore.cpp
        include/Testing/TestCore.h
        src/Testing/TestDriver.cpp
        include/Testing/TestDriver.h
        src/Testing/AbstractTriggerTest.cpp
        include/Testing/AbstractTriggerTest.h
        src/Testing/AbstractTest.cpp
        include/Testing/AbstractTest.h
        tests/Tests/CycleTest.cpp
        tests/Tests/CycleTest.h
        src/Windows/Controllers/TestControllers/UnitTestsController.cpp
        include/Windows/Controllers/TestControllers/UnitTestsController.h
        src/Executor/TestingExecutor.cpp
        include/Executor/TestingExecutor.h
#        tests/Tests/PythonTest.cpp
#        tests/Tests/PythonTest.h
        src/Testing/TestEnvironment.cpp
        include/Testing/TestEnvironment.h
        src/Tools/Settings.cpp
        include/Tools/Settings.h
        src/Windows/Widgets/QTestsTreeWidget.cpp
        include/Windows/Widgets/QTestsTreeWidget.h
#        tests/Triggers/PythonTrigger.cpp
#        tests/Triggers/PythonTrigger.h
        src/Windows/Widgets/QTestsTreeWidgetMenu.cpp
        include/Windows/Widgets/QTestsTreeWidgetMenu.h
        src/Testing/TestLogger.cpp
        include/Testing/TestLogger.h
        src/Executor/TestLoggerWaiter.cpp
        include/Executor/TestLoggerWaiter.h
        src/TestDriverHolder.cpp
        include/TestDriverHolder.h
        src/Windows/Controllers/SettingsController.cpp
        include/Windows/Controllers/SettingsController.h
        src/Windows/Controllers/CommandsControllers/PrintCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h
        src/Implementation/TCPInterface.cpp
        include/Implementation/TCPInterface.h
        src/Tools/NetworkTools.cpp
        include/Tools/NetworkTools.h
        src/Tools/IPv4Address.cpp
        include/Tools/IPv4Address.h
        src/Windows/Controllers/ConnectTabController.cpp
        include/Windows/Controllers/ConnectTabController.h
        src/Windows/RootController.cpp
        include/Windows/Controllers/RootController.h
        src/Windows/Controllers/ConnectControllers/ConnectTCPTabController.cpp
        include/Windows/Controllers/ConnectControllers/ConnectTCPTabController.h
        src/Tools/ByteArrayReader.cpp
        include/Tools/ByteArrayReader.h
        src/Windows/Controllers/CommandsControllers/FNCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/FNCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.h
        src/Windows/Controllers/TablesTabController.cpp
        include/Windows/Controllers/TablesTabController.h
        src/Tools/Codecs.cpp
        include/Tools/Codecs.h
        src/Tools/StdExtend.cpp
        include/Tools/StdExtend.h
        src/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.h
        tests/Tests/OperationTest.cpp
        tests/Tests/OperationTest.h
        tests/Triggers/OperationTrigger.cpp
        tests/Triggers/OperationTrigger.h
        tests/Triggers/CheckCloseTrigger.cpp
        tests/Triggers/CheckCloseTrigger.h
        src/Testing/FROperations.cpp
        include/Testing/FROperations.h
        src/Windows/Controllers/CommandsControllers/DeviceCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/DeviceCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/DeviceMoneyCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/DeviceMoneyCommandsTabController.h
        tests/Tests/CheckLoaderTest.cpp
        tests/Tests/CheckLoaderTest.h
        src/Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.h
        src/Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.cpp
        include/Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.h
        tests/Tests/WriteShitCashierNameTest.cpp
        tests/Tests/WriteShitCashierNameTest.h
        tests/Tests/NonfiscalRequesting.cpp
        tests/Tests/NonfiscalRequesting.h
        tests/Tests/TableFiscalStorageLinesTest.cpp
        tests/Tests/TableFiscalStorageLinesTest.h
        src/Windows/Widgets/QNumberLineEdit.cpp
        include/Windows/Widgets/QNumberLineEdit.h
        src/Windows/Widgets/QBaseNumberLineEdit.cpp
        include/Windows/Widgets/QBaseNumberLineEdit.h
    )

add_executable(FRTester
        ${SOURCE_FILES}
        ${DRIVER_FILES}
        ${UIS_HDRS}
        ${RESOURCE_ADDED})
qt5_use_modules(FRTester Core Widgets Gui)
# -lpython2.7
target_link_libraries(FRTester ${Qt5Widgets_LIBRARIES} )