cmake_minimum_required(VERSION 3.6)
project(FRTester)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../${CMAKE_PROJECT_NAME}_executable")
set(CMAKE_CXX_FLAGS "-Werror -ansi -pedantic -Wabi -Wall -Wextra -Weffc++ -Woverloaded-virtual -Winit-self -Wunreachable-code ")
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_INCLUDE_PATH} libraries)
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

if (WIN32)

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -D_hypot=hypot")
endif()

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

set(MAIN_FILES
        main.cpp
        )

set(SOURCE_FILES
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
        src/Testing/SettingsSystem.cpp
        include/Testing/SettingsSystem.h
        libraries/json.hpp
        tests/Triggers/ZReportTrigger.cpp
        tests/Triggers/ZReportTrigger.h
        src/Testing/TestingTools.cpp
        include/Testing/TestingTools.h
        src/Testing/TestDataValue.cpp
        include/Testing/TestDataValue.h
        src/Windows/Controllers/LogParserTabController.cpp
        include/Windows/Controllers/LogParserTabController.h
        tests/Tests/BarcodePrintingTest.cpp
        tests/Tests/BarcodePrintingTest.h
        tests/Tests/CheckFontTest.cpp
        tests/Tests/CheckFontTest.h
        src/Windows/Controllers/TestControllers/TestsEditorController.cpp
        include/Windows/Controllers/TestControllers/TestsEditorController.h
        src/Testing/ManualTests/ManualTest.cpp
        include/Testing/ManualTests/ManualTest.h
        src/Testing/ManualTests/AbstractTestAction.cpp
        include/Testing/ManualTests/AbstractTestAction.h
        src/Windows/Widgets/QTestActionsTreeWidget.cpp
        include/Windows/Widgets/QTestActionsTreeWidget.h
        src/Testing/ManualTests/Actions/BeepTestAction.cpp
        include/Testing/ManualTests/Actions/BeepTestAction.h
        src/Testing/ManualTests/ManualTestFabric.cpp
        include/Testing/ManualTests/ManualTestFabric.h
        src/Windows/Widgets/QTestDisplayTreeWidget.cpp
        include/Windows/Widgets/QTestDisplayTreeWidget.h
        src/Windows/Widgets/QActionTreeWidgetItem.cpp
        include/Windows/Widgets/QActionTreeWidgetItem.h
        src/Windows/Widgets/QManualTestsListWidget.cpp
        include/Windows/Widgets/QManualTestsListWidget.h
        src/Tools/GUIHelper.cpp
        include/Tools/GUIHelper.h
        src/Testing/ManualTests/Actions/LongStateRequestTestAction.cpp
        include/Testing/ManualTests/Actions/LongStateRequestTestAction.h
        src/Testing/ManualTests/Actions/WhileTestAction.cpp
        include/Testing/ManualTests/Actions/WhileTestAction.h
        src/Testing/ManualTests/Actions/TimeoutTestAction.cpp
        include/Testing/ManualTests/Actions/TimeoutTestAction.h
        src/Testing/ManualTests/Actions/SendRawTestAction.cpp
        include/Testing/ManualTests/Actions/SendRawTestAction.h
        src/Testing/ManualTests/Actions/NotifyWindowTestAction.cpp
        include/Testing/ManualTests/Actions/NotifyWindowTestAction.h
        src/Testing/ManualTests/Actions/ExitIfTestAction.cpp
        include/Testing/ManualTests/Actions/ExitIfTestAction.h
        src/Testing/ManualTests/Actions/IfTestAction.cpp
        include/Testing/ManualTests/Actions/IfTestAction.h
        tests/Tests/MagnitCheckTest.cpp tests/Tests/MagnitCheckTest.h
        tests/Tests/MessageBoxTest.cpp tests/Tests/MessageBoxTest.h
        src/Windows/Widgets/QHexLineEdit.cpp
        include/Windows/Widgets/QHexLineEdit.h
        src/Testing/ManualTests/Actions/OpenShiftTestAction.cpp
        include/Testing/ManualTests/Actions/OpenShiftTestAction.h
        src/Testing/ManualTests/Actions/ShiftCloseTestAction.cpp
        include/Testing/ManualTests/Actions/ShiftCloseTestAction.h
        src/Testing/ManualTests/Actions/SellTestAction.cpp
        include/Testing/ManualTests/Actions/SellTestAction.h
        src/Testing/ManualTests/Actions/BuyTestAction.cpp
        include/Testing/ManualTests/Actions/BuyTestAction.h
        src/Testing/ManualTests/Actions/ReturnSellTestAction.cpp
        include/Testing/ManualTests/Actions/ReturnSellTestAction.h
        src/Testing/ManualTests/Actions/ReturnBuyTestAction.cpp
        include/Testing/ManualTests/Actions/ReturnBuyTestAction.h
        src/Testing/ManualTests/Actions/ShortStateRequest.cpp
        include/Testing/ManualTests/Actions/ShortStateRequest.h
        src/Testing/ManualTests/Actions/CloseCheckTestAction.cpp
        include/Testing/ManualTests/Actions/CloseCheckTestAction.h
        src/Testing/ManualTests/Actions/ResumePrintingTestAction.cpp
        include/Testing/ManualTests/Actions/ResumePrintingTestAction.h
        src/Testing/ManualTests/Actions/CancelCheckTestAction.cpp
        include/Testing/ManualTests/Actions/CancelCheckTestAction.h
        src/Testing/ManualTests/Actions/StandardStringPrintTestAction.cpp
        include/Testing/ManualTests/Actions/StandardStringPrintTestAction.h
        src/Testing/ManualTests/Actions/DocumentHeaderPrintTestAction.cpp
        include/Testing/ManualTests/Actions/DocumentHeaderPrintTestAction.h
        src/Testing/ManualTests/Actions/CutCheckTestAction.cpp
        include/Testing/ManualTests/Actions/CutCheckTestAction.h
        src/Testing/ManualTests/Actions/ScrollTestAction.cpp
        include/Testing/ManualTests/Actions/ScrollTestAction.h
        src/Testing/ManualTests/Actions/FontStringPrintTestAction.cpp
        include/Testing/ManualTests/Actions/FontStringPrintTestAction.h
        tests/Tests/CrazyStatusRequestTest.cpp
        tests/Tests/CrazyStatusRequestTest.h
        src/Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.cpp
        include/Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.h
        src/Testing/ManualTests/Actions/SectionsReportTestAction.cpp
        include/Testing/ManualTests/Actions/SectionsReportTestAction.h
        src/Testing/ManualTests/Actions/TaxesReportTestAction.cpp
        include/Testing/ManualTests/Actions/TaxesReportTestAction.h
        src/Testing/ManualTests/Actions/CashierReportTestAction.cpp
        include/Testing/ManualTests/Actions/CashierReportTestAction.h
        src/Testing/ManualTests/Actions/PayinTestAction.cpp
        include/Testing/ManualTests/Actions/PayinTestAction.h
        src/Testing/ManualTests/Actions/PayoutTestAction.cpp
        include/Testing/ManualTests/Actions/PayoutTestAction.h
        src/Testing/ManualTests/Actions/PrintClichesTestAction.cpp
        include/Testing/ManualTests/Actions/PrintClichesTestAction.h
        src/Testing/ManualTests/Actions/PrintDocumentEndTestAction.cpp
        include/Testing/ManualTests/Actions/PrintDocumentEndTestAction.h
        src/Testing/ManualTests/Actions/PrintAdsTestAction.cpp
        include/Testing/ManualTests/Actions/PrintAdsTestAction.h
        src/Testing/ManualTests/Actions/PrintNonZeroSumsTestAction.cpp
        include/Testing/ManualTests/Actions/PrintNonZeroSumsTestAction.h
        src/Testing/ManualTests/Actions/PingTestAction.cpp
        include/Testing/ManualTests/Actions/PingTestAction.h
        src/Testing/ManualTests/Actions/RebootTestAction.cpp
        include/Testing/ManualTests/Actions/RebootTestAction.h
        src/Testing/ManualTests/Actions/PrintBarcodeTestAction.cpp
        include/Testing/ManualTests/Actions/PrintBarcodeTestAction.h
        src/Testing/ManualTests/Fabric.cpp
        include/Testing/ManualTests/Fabric.h
        src/Testing/ManualTests/FabricRegistrator.cpp
        include/Testing/ManualTests/FabricRegistrator.h
        include/Testing/ManualTests/TestActionFabric.h
        include/Testing/StaticTestFabric.h
        include/Testing/StaticTriggerFabric.h
        tests/Tests/QuickMiniposTest.cpp
        tests/Tests/QuickMiniposTest.h
        src/Testing/ManualTests/Actions/WaitForPrintingFinishTestAction.cpp
        include/Testing/ManualTests/Actions/WaitForPrintingFinishTestAction.h src/Tools/ParsedLog.cpp include/Tools/ParsedLog.h src/Tools/LoggerProcessor.cpp include/Tools/LoggerProcessor.h src/Testing/ManualTests/Actions/WriteTableStringTestAction.cpp include/Testing/ManualTests/Actions/WriteTableStringTestAction.h src/Testing/ManualTests/Actions/WriteTableBinTestAction.cpp include/Testing/ManualTests/Actions/WriteTableBinTestAction.h)

add_executable(FRTester
        ${MAIN_FILES}
        ${SOURCE_FILES}
        ${DRIVER_FILES}
        ${UIS_HDRS}
        ${RESOURCE_ADDED})

qt5_use_modules(FRTester Core Widgets Gui)
# -lpython2.7
if (WIN32)

else()
    target_link_libraries(FRTester ${Qt5Widgets_LIBRARIES})
endif()