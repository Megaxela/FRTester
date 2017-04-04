cmake_minimum_required(VERSION 3.6)
#include(ExternalProject)
project(FRTester)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../${CMAKE_PROJECT_NAME}_executable")
set(CMAKE_CXX_FLAGS "-ansi -pedantic -Wabi -Wall -Wextra -Weffc++ -Woverloaded-virtual -Winit-self -Wunreachable-code ")
set(CMAKE_CXX_STANDARD 11)

link_directories(${CMAKE_LIBRARY_PATH})

#------------------------
# Setuping driver
#------------------------

add_subdirectory(shared_libs/frdrvcross)

#------------------------
# Libraries setup
#------------------------

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "/home/megaxela/Development/Projects/C++/FRTester_executable/")

#-------------
# Includes
#-------------
include_directories(${CMAKE_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(libraries)
include_directories(include)
include_directories(shared_libs/frdrvcross/include)
include_directories(tests)

#-----------------------
# Files
#-----------------------

set(QT_RESOURCES
        res/resources.qrc)

set(UI_FILES
        ui/mainwindow.ui)

set(UNMANAGED_FILES
        src/Windows/Controllers/FNStateCommandsTabController.cpp include/Windows/Controllers/FNStateCommandsTabController.h)

set(MAIN_FILE
        main.cpp
        )

set(QOBJECT_HEADERS
        include/Executor/TestingExecutor.h
        include/Executor/TestLoggerWaiter.h
        include/Windows/MainWindow.h
        include/Windows/Controllers/AbstractTabController.h
        include/Windows/Controllers/ConnectControllers/ConnectCOMTabController.h
        include/Windows/Controllers/TestControllers/TestsEditorController.h
        include/Windows/Controllers/TestControllers/UnitTestsController.h
        include/Windows/Widgets/QBaseNumberLineEdit.h
        include/Windows/Widgets/QHexLineEdit.h
        include/Windows/Widgets/QManualTestsListWidget.h
        include/Windows/Widgets/QTestActionsTreeWidget.h
        include/Windows/Widgets/QTestDisplayTreeWidget.h
        include/Windows/Widgets/QTestsTreeWidget.h
        include/Windows/Widgets/QTestsTreeWidgetMenu.h
        )

set(INCLUDE_FILES
        include/DriverHolder.h
        include/Windows/Controllers/CommandsTabController.h
        include/Windows/Controllers/CommandsControllers/StateCommandsTabController.h
        include/Tools/StableController.h
        include/Windows/Controllers/CommandsControllers/RegisterOperationsV1CommandsTabController.h
        include/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h
        include/Tools/TypeConverters.h
        include/Testing/TestCore.h
        include/Testing/TestDriver.h
        include/Testing/AbstractTriggerTest.h
        include/Testing/AbstractTest.h
        include/Testing/TestEnvironment.h
        include/Testing/TestLogger.h
        include/TestDriverHolder.h
        include/Windows/Controllers/SettingsController.h
        include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h
        include/Windows/Controllers/ConnectTabController.h
        include/Windows/Controllers/RootController.h
        include/Windows/Controllers/ConnectControllers/ConnectTCPTabController.h
        include/Windows/Controllers/CommandsControllers/FNCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.h
        include/Windows/Controllers/TablesTabController.h
        include/Tools/Codecs.h
        include/Tools/StdExtend.h
        include/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.h
        include/Testing/FROperations.h
        include/Windows/Controllers/CommandsControllers/DeviceCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/DeviceMoneyCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.h
        include/Windows/Widgets/QNumberLineEdit.h
        include/Testing/SettingsSystem.h
        include/Testing/TestingTools.h
        include/Testing/TestDataValue.h
        include/Windows/Controllers/LogParserTabController.h
        include/Testing/ManualTests/ManualTest.h
        include/Testing/ManualTests/AbstractTestAction.h
        include/Testing/ManualTests/Actions/BeepTestAction.h
        include/Testing/ManualTests/ManualTestFabric.h
        include/Windows/Widgets/QActionTreeWidgetItem.h
        include/Tools/GUIHelper.h
        include/Testing/ManualTests/Actions/LongStateRequestTestAction.h
        include/Testing/ManualTests/Actions/WhileTestAction.h
        include/Testing/ManualTests/Actions/TimeoutTestAction.h
        include/Testing/ManualTests/Actions/SendRawTestAction.h
        include/Testing/ManualTests/Actions/NotifyWindowTestAction.h
        include/Testing/ManualTests/Actions/ExitIfTestAction.h
        include/Testing/ManualTests/Actions/IfTestAction.h
        include/Testing/ManualTests/Actions/OpenShiftTestAction.h
        include/Testing/ManualTests/Actions/ShiftCloseTestAction.h
        include/Testing/ManualTests/Actions/SellTestAction.h
        include/Testing/ManualTests/Actions/BuyTestAction.h
        include/Testing/ManualTests/Actions/ReturnSellTestAction.h
        include/Testing/ManualTests/Actions/ReturnBuyTestAction.h
        include/Testing/ManualTests/Actions/ShortStateRequest.h
        include/Testing/ManualTests/Actions/CloseCheckTestAction.h
        include/Testing/ManualTests/Actions/ResumePrintingTestAction.h
        include/Testing/ManualTests/Actions/CancelCheckTestAction.h
        include/Testing/ManualTests/Actions/StandardStringPrintTestAction.h
        include/Testing/ManualTests/Actions/DocumentHeaderPrintTestAction.h
        include/Testing/ManualTests/Actions/CutCheckTestAction.h
        include/Testing/ManualTests/Actions/ScrollTestAction.h
        include/Testing/ManualTests/Actions/FontStringPrintTestAction.h
        include/Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.h
        include/Testing/ManualTests/Actions/SectionsReportTestAction.h
        include/Testing/ManualTests/Actions/TaxesReportTestAction.h
        include/Testing/ManualTests/Actions/CashierReportTestAction.h
        include/Testing/ManualTests/Actions/PayinTestAction.h
        include/Testing/ManualTests/Actions/PayoutTestAction.h
        include/Testing/ManualTests/Actions/PrintClichesTestAction.h
        include/Testing/ManualTests/Actions/PrintDocumentEndTestAction.h
        include/Testing/ManualTests/Actions/PrintAdsTestAction.h
        include/Testing/ManualTests/Actions/PrintNonZeroSumsTestAction.h
        include/Testing/ManualTests/Actions/PingTestAction.h
        include/Testing/ManualTests/Actions/RebootTestAction.h
        include/Testing/ManualTests/Actions/PrintBarcodeTestAction.h
        include/Testing/ManualTests/Fabric.h
        include/Testing/ManualTests/FabricRegistrator.h
        include/Testing/StaticTestFabric.h
        include/Testing/ManualTests/TestActionFabric.h
        include/Testing/StaticTriggerFabric.h
        include/Testing/ManualTests/Actions/WaitForPrintingFinishTestAction.h
        include/Tools/ParsedLog.h
        include/Tools/LoggerProcessor.h
        include/Testing/ManualTests/Actions/WriteTableStringTestAction.h
        include/Testing/ManualTests/Actions/WriteTableBinTestAction.h
        include/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h
        include/Windows/Controllers/CommandsControllers/RegisterOperationsV2CommandsTabController.h
        )

set(STATIC_TESTS
        tests/Tests/CycleTest.h
        tests/Tests/OperationTest.h
        tests/Triggers/OperationTrigger.h
        tests/Triggers/CheckCloseTrigger.h
        tests/Tests/CycleTest.cpp
        tests/Tests/OperationTest.cpp
        tests/Triggers/OperationTrigger.cpp
        tests/Triggers/CheckCloseTrigger.cpp
        tests/Tests/CheckLoaderTest.cpp
        tests/Tests/CheckLoaderTest.h
        tests/Tests/WriteShitCashierNameTest.cpp
        tests/Tests/WriteShitCashierNameTest.h
        tests/Tests/NonfiscalRequesting.cpp
        tests/Tests/NonfiscalRequesting.h
        tests/Tests/TableFiscalStorageLinesTest.cpp
        tests/Tests/TableFiscalStorageLinesTest.h
        tests/Triggers/ZReportTrigger.h
        tests/Triggers/ZReportTrigger.cpp
        tests/Tests/BarcodePrintingTest.cpp
        tests/Tests/BarcodePrintingTest.h
        tests/Tests/CheckFontTest.cpp
        tests/Tests/CheckFontTest.h
        tests/Tests/MagnitCheckTest.cpp tests/Tests/MagnitCheckTest.h
        tests/Tests/MessageBoxTest.cpp tests/Tests/MessageBoxTest.h
        tests/Tests/Bug0017479Test.cpp
        tests/Tests/Bug0017479Test.h
        tests/Tests/Bug0017515Test.cpp
        tests/Tests/Bug0017515Test.h
        tests/Tests/Bug0017483Test.cpp
        tests/Tests/Bug0017483Test.h
        tests/Tests/CrazyStatusRequestTest.cpp
        tests/Tests/CrazyStatusRequestTest.h
        tests/Tests/QuickMiniposTest.h
        tests/Tests/QuickMiniposTest.cpp
        )

set(SOURCE_FILES
        src/DriverHolder.cpp
        src/Tools/StableController.cpp
        src/Tools/TypeConverters.cpp
        src/Testing/TestCore.cpp
        src/Testing/TestDriver.cpp
        src/Testing/AbstractTriggerTest.cpp
        src/Testing/AbstractTest.cpp
        src/Executor/TestingExecutor.cpp
        src/Testing/TestEnvironment.cpp
        src/Testing/TestLogger.cpp
        src/Executor/TestLoggerWaiter.cpp
        src/TestDriverHolder.cpp
        src/Tools/Codecs.cpp
        src/Tools/StdExtend.cpp
        src/Testing/FROperations.cpp
        src/Testing/SettingsSystem.cpp
        src/Tools/GUIHelper.cpp
        src/Tools/ParsedLog.cpp
        src/Tools/LoggerProcessor.cpp
        libraries/json.hpp
        src/Testing/TestingTools.cpp
        src/Testing/TestDataValue.cpp
        src/Testing/ManualTests/ManualTest.cpp
        src/Testing/ManualTests/AbstractTestAction.cpp
        src/Testing/ManualTests/Actions/BeepTestAction.cpp
        src/Testing/ManualTests/ManualTestFabric.cpp
        src/Testing/ManualTests/Actions/LongStateRequestTestAction.cpp
        src/Testing/ManualTests/Actions/WhileTestAction.cpp
        src/Testing/ManualTests/Actions/TimeoutTestAction.cpp
        src/Testing/ManualTests/Actions/SendRawTestAction.cpp
        src/Testing/ManualTests/Actions/NotifyWindowTestAction.cpp
        src/Testing/ManualTests/Actions/ExitIfTestAction.cpp
        src/Testing/ManualTests/Actions/IfTestAction.cpp
        src/Testing/ManualTests/Actions/OpenShiftTestAction.cpp
        src/Testing/ManualTests/Actions/ShiftCloseTestAction.cpp
        src/Testing/ManualTests/Actions/SellTestAction.cpp
        src/Testing/ManualTests/Actions/BuyTestAction.cpp
        src/Testing/ManualTests/Actions/ReturnSellTestAction.cpp
        src/Testing/ManualTests/Actions/ReturnBuyTestAction.cpp
        src/Testing/ManualTests/Actions/ShortStateRequest.cpp
        src/Testing/ManualTests/Actions/CloseCheckTestAction.cpp
        src/Testing/ManualTests/Actions/ResumePrintingTestAction.cpp
        src/Testing/ManualTests/Actions/CancelCheckTestAction.cpp
        src/Testing/ManualTests/Actions/StandardStringPrintTestAction.cpp
        src/Testing/ManualTests/Actions/DocumentHeaderPrintTestAction.cpp
        src/Testing/ManualTests/Actions/CutCheckTestAction.cpp
        src/Testing/ManualTests/Actions/ScrollTestAction.cpp
        src/Testing/ManualTests/Actions/FontStringPrintTestAction.cpp
        src/Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.cpp
        src/Testing/ManualTests/Actions/SectionsReportTestAction.cpp
        src/Testing/ManualTests/Actions/TaxesReportTestAction.cpp
        src/Testing/ManualTests/Actions/CashierReportTestAction.cpp
        src/Testing/ManualTests/Actions/PayinTestAction.cpp
        src/Testing/ManualTests/Actions/PayoutTestAction.cpp
        src/Testing/ManualTests/Actions/PrintClichesTestAction.cpp
        src/Testing/ManualTests/Actions/PrintDocumentEndTestAction.cpp
        src/Testing/ManualTests/Actions/PrintAdsTestAction.cpp
        src/Testing/ManualTests/Actions/PrintNonZeroSumsTestAction.cpp
        src/Testing/ManualTests/Actions/PingTestAction.cpp
        src/Testing/ManualTests/Actions/RebootTestAction.cpp
        src/Testing/ManualTests/Actions/PrintBarcodeTestAction.cpp
        src/Testing/ManualTests/Fabric.cpp
        src/Testing/ManualTests/FabricRegistrator.cpp
        src/Testing/ManualTests/Actions/WaitForPrintingFinishTestAction.cpp
        src/Testing/ManualTests/Actions/WriteTableStringTestAction.cpp
        src/Testing/ManualTests/Actions/WriteTableBinTestAction.cpp
        src/Windows/MainWindow.cpp
        src/Windows/RootController.cpp
        src/Windows/Widgets/QNumberLineEdit.cpp
        src/Windows/Widgets/QBaseNumberLineEdit.cpp
        src/Windows/Widgets/QTestActionsTreeWidget.cpp
        src/Windows/Widgets/QTestDisplayTreeWidget.cpp
        src/Windows/Widgets/QActionTreeWidgetItem.cpp
        src/Windows/Widgets/QManualTestsListWidget.cpp
        src/Windows/Widgets/QHexLineEdit.cpp
        src/Windows/Widgets/QTestsTreeWidgetMenu.cpp
        src/Windows/Widgets/QTestsTreeWidget.cpp
        src/Windows/Controllers/ConnectControllers/ConnectTCPTabController.cpp
        src/Windows/Controllers/CommandsControllers/FNCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/RegisterOperationsV2CommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/DeviceCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/DeviceMoneyCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/PrintCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.cpp
        src/Windows/Controllers/ConnectControllers/ConnectCOMTabController.cpp
        src/Windows/Controllers/AbstractTabController.cpp
        src/Windows/Controllers/CommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/StateCommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/RegisterOperationsV1CommandsTabController.cpp
        src/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.cpp
        src/Windows/Controllers/TestControllers/UnitTestsController.cpp
        src/Windows/Controllers/SettingsController.cpp
        src/Windows/Controllers/ConnectTabController.cpp
        src/Windows/Controllers/TablesTabController.cpp
        src/Windows/Controllers/LogParserTabController.cpp
        src/Windows/Controllers/TestControllers/TestsEditorController.cpp
        tests/Tests/DocumentSyncTest.cpp tests/Tests/DocumentSyncTest.h)

#------------------------
# Setuping Qt
#------------------------

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
qt5_add_resources(RESOURCE_ADDED ${QT_RESOURCES})


qt5_wrap_cpp(UI_SOURCE
        ${QOBJECT_HEADERS}
)

qt5_wrap_ui(
        UI_HDRS
        ${UI_FILES}
)

#-----------------------
# Adding to executable
#-----------------------

add_executable(FRTester
        ${UI_HDRS}
        ${UI_SOURCE}
        ${MAIN_FILE}
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
        ${UNMANAGED_FILES}
        ${STATIC_TESTS}
        ${RESOURCE_ADDED}
        ${QOBJECT_HEADERS}
)

# FRTester needs frdrvcross
add_dependencies(FRTester frdrvcross)

#-------------
# Linkage
#-------------
message("${CMAKE_SOURCE_DIR}/frdrvcross/lib")
target_link_libraries(FRTester -L"${CMAKE_SOURCE_DIR}/build/libs" -lfrdrvcross)

add_custom_command(TARGET FRTester PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/build/libs $<TARGET_FILE_DIR:FRTester>)

qt5_use_modules(FRTester Core Widgets Gui)