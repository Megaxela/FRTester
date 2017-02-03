//
// Created by megaxela on 21.11.16.
//

#include <include/Testing/TestCore.h>
#include <include/DriverHolder.h>
#include <tests/Tests/CycleTest.h>
#include <Tools/Logger.h>
#include <include/Tools/Settings.h>
#include <include/Testing/TestLogger.h>
#include <include/TestDriverHolder.h>
#include <include/Tools/Time.h>
#include <tests/Tests/OperationTest.h>
#include <tests/Triggers/OperationTrigger.h>
#include <tests/Triggers/CheckCloseTrigger.h>
#include <tests/Tests/CheckLoaderTest.h>
#include <tests/Tests/WriteShitCashierNameTest.h>
#include <tests/Tests/NonfiscalRequesting.h>
#include <tests/Tests/TableFiscalStorageLinesTest.h>
#include <tests/Triggers/ZReportTrigger.h>

#define PY_LIST_DELIM ':'

TestCore::TestCore() :
    m_sysAdmPassword(30)
{
    m_environment = new TestEnvironment(
            &TestDriverHolder::driver(),
            &TestLogger::instance(),
            new TestingTools(
                    &TestDriverHolder::driver(),
                    &TestLogger::instance()
            )
    );

    init();
}

TestCore::~TestCore()
{
    delete m_environment->tools();
    delete m_environment;

//    Py_Finalize();
}

TestCore &TestCore::instance()
{
    static TestCore core;

    return core;
}

void TestCore::updateDatabase()
{

    m_tests.clear();
    m_triggers.clear();

    // Загрузка статических тестов
    addTest(std::make_shared<CycleTest>(m_environment));
    addTest(std::make_shared<OperationTest>(m_environment));
    addTest(std::make_shared<CheckLoaderTest>(m_environment));
    addTest(std::make_shared<WriteShitCashierNameTest>(m_environment));
    addTest(std::make_shared<NonfiscalRequesting>(m_environment));
    addTest(std::make_shared<TableFiscalStorageLinesTest>(m_environment));

    // Загрузка триггеров
    addTrigger(std::make_shared<OperationTrigger>(m_environment));
    addTrigger(std::make_shared<CheckCloseTrigger>(m_environment));
    addTrigger(std::make_shared<ZReportTrigger>(m_environment));

//    if (Py_IsInitialized())
//    {
//        Log("Переинициализация интерпретатора.");
//
//        deinit();
//        init();
//    }
//
//    std::string testsPath = Settings::instance().getValue(
//            SETTINGS_NAMES_TESTSPATH, "tests"
//    );
//
//    // Загрузка Python тестов
//    std::string pythonTestsPath = SystemTools::Path::join(testsPath, "python");
//
//    if (!testsPath.empty())
//    {
//        // Получение всех файлов в директории
//        std::vector<std::string> testFiles = SystemTools::getAllFilesInDir(pythonTestsPath);
//        if (testFiles.empty())
//        {
//            Error("Папки с тестами не существует или тесты отсутствуют.");
//        }
//
//        for (auto filename : testFiles)
//        {
//            // Название файла слишком мало
//            if (filename.size() <= 3)
//            {
//                continue;
//            }
//
//            // Заканчивается ли файл на .py
//            if (filename.substr(filename.length() - 3, 3) != ".py")
//            {
//                continue;
//            }
//
//            auto loadedTest = PythonTest::loadTest(
//                    m_environment,
//                    filename.substr(0, filename.length() - 3),
//                    pythonTestsPath
//            );
//
//            if (loadedTest != nullptr)
//            {
//                TestData data;
//                data.enabled = true;
//                data.test = loadedTest;
//                m_tests.push_back(data);
//            }
//        }
//    }
//    else
//    {
//        Critical("Путь к тестам пуст.");
//    }
//
//    std::string triggersPath = Settings::instance().getValue(
//            SETTINGS_NAMES_TRIGGERSPATH, "triggers"
//    );
//
//    std::string pythonTriggersPath = SystemTools::Path::join(
//            triggersPath, "python"
//    );
//
//    if (!triggersPath.empty())
//    {
//        // Получение всех файлов в директории
//        std::vector<std::string> testFiles = SystemTools::getAllFilesInDir(pythonTriggersPath);
//        if (testFiles.empty())
//        {
//            Error("Папки с триггерами не существует или тесты отсутствуют.");
//        }
//
//        for (auto filename : testFiles)
//        {
//            // Название файла слишком мало
//            if (filename.size() <= 3)
//            {
//                continue;
//            }
//
//            // Заканчивается ли файл на .py
//            if (filename.substr(filename.length() - 3, 3) != ".py")
//            {
//                continue;
//            }
//
//            auto loadedTrigger = PythonTrigger::loadTrigger(
//                    m_environment,
//                    filename.substr(0, filename.length() - 3),
//                    pythonTriggersPath
//            );
//
//            if (loadedTrigger != nullptr)
//            {
//                TriggerData data;
//                data.trigger = loadedTrigger;
//                data.enabled = true;
//                m_triggers.push_back(data);
//            }
//        }
//    }
//    else
//    {
//        Error("Путь к триггерам пуст.");
//    }
}

void TestCore::getTriggers(const std::string &tag,
                           std::vector<TriggerTestPtr> &v)
{
    for (auto trigger : m_triggers)
    {
        if (trigger.trigger->containsTag(tag))
        {
            v.push_back(trigger.trigger);
        }
    }
}

bool TestCore::restoreFRState()
{
    if (!DriverHolder::driver().checkConnection())
    {
        return false;
    }

    auto state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);

    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        Error("Не удалось получить полное состояние устройства. Прерываем тестирование.");
        return false;
    }

    while (state.posSubMode != 0)
    {
        if (state.posSubMode == 3) // После активного отсутствия бумаги.
        {
            Log("Обнаружено 3 состояние (После активного отсутствия бумаги).");
            if (!DriverHolder::driver().resumePrinting(m_sysAdmPassword))
            {
                Error("Не удалось продолжить печать при 3 состоянии.");
                return false;
            }
        }
        else if (state.posSubMode == 5) // Печать
        {
            Log("Сейчас идет печать.");
            for (auto i = 0; i < 5 && state.posSubMode == 5; ++i)
            {
                state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);

                Time::sleep<std::chrono::seconds>(1);
            }

            if (state.posSubMode == 5)
            {
                Error("Печать продолжается более 5 секунд. Останавливаем тестирование.");
                return false;
            }
        }
        else
        {
            Error("Необрабатываемый подрежим: " + std::to_string(state.posSubMode));
            return false;
        }

        state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);

        if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
        {
            Error("Не удалось получить полное состояние устройства. Прерываем тестирование.");
            return false;
        }
    }

    while (state.posMode != 4)
    {
        if (state.posMode == 8) // Открытый документ
        {
            // Отменяем документ
            if (!DriverHolder::driver().cancelCheck(m_sysAdmPassword))
            {
                Error("Не удалось отменить открытый чек.");
                return false;
            }
        }
        else if (state.posMode == 2 || state.posMode == 3) // Открытая смена
        {
            // Закрываем смену
            if (!DriverHolder::driver().shiftCloseReport(m_sysAdmPassword))
            {
                if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::PreviousCommandPrintingIsActive)
                {
                    Error("Не удалось закрыть смену.");
                    return false;
                }
            }

            Time::sleep<std::chrono::milliseconds>(200);

            Log("Ожидаем окончания печати Z отчета.");
            state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);
            for (auto i = 0; i < 5 && state.posSubMode == 5; ++i)
            {
                state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);

                Time::sleep<std::chrono::seconds>(1);
            }

            if (state.posSubMode == 5)
            {
                Error("Печать продолжается более 5 секунд. Останавливаем тестирование.");
                return false;
            }
        }
        else if (state.posMode == 9) // Ошибка ОЗУ
        {
            Critical("Ошибка ОЗУ. Прерываем тестирование.");
            return false;
        }
        else if (state.posMode == 6) // Ожидание подтверждения даты
        {
            Critical("Ожидание подтверждения даты. Прерываем тестирование.");
            return false;
        }
        else
        {
            Critical("Неопознаное состояние " + std::to_string(state.posMode) + ". Прерываем тестирование.");
            return false;
        }

        state = DriverHolder::driver().fullStateRequest(m_sysAdmPassword);

        if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
        {
            Error("Не удалось получить полное состояние устройства. Прерываем тестирование.");
            return false;
        }
    }

    return true;
}

void TestCore::triggerMistaken(TriggerTestPtr trigger)
{
    m_failedTriggers.push_back(trigger);
}

std::vector<TestPtr> TestCore::getTests()
{
    std::vector<TestPtr> tests;

    for (auto el : m_tests)
    {
        tests.push_back(el.test);
    }

    return tests;
}

std::vector<TriggerTestPtr> TestCore::getTriggers()
{
    std::vector<TriggerTestPtr> triggers;

    for (auto el : m_triggers)
    {
        triggers.push_back(el.trigger);
    }

    return triggers;
}

bool TestCore::hasFailedTriggers()
{
    return !m_failedTriggers.empty();
}

std::vector<TriggerTestPtr> TestCore::getFailedTriggers()
{
    return m_failedTriggers;
}

void TestCore::clearFailedTriggers()
{
    m_failedTriggers.clear();
}

void TestCore::init()
{
//    Py_Initialize();
//    initfrdriver();
//    Py_SetPythonHome("python2/");
//
//    PyObject* sysModule = PyImport_ImportModule("sys");
//    if (sysModule == nullptr)
//    {
//        Critical("Can't import sys.");
//        return;
//    }
//
//
//    PyObject* pathObject = PyObject_GetAttrString(sysModule, "path");
//    if (pathObject == nullptr)
//    {
//        Critical("Can't find path property in `sys` module.");
//        return;
//    }
//
//    PyList_Append(pathObject, PyString_FromString(
//            SystemTools::Path::join(Settings::instance().getValue(
//                    SETTINGS_NAMES_TRIGGERSPATH,
//                    "./triggers/"
//            ), "python").c_str()
//    ));
//
//    PyList_Append(pathObject, PyString_FromString(
//            SystemTools::Path::join(Settings::instance().getValue(
//                    SETTINGS_NAMES_TESTSPATH,
//                    "./tests/"
//            ), "python").c_str()
//    ));
}

void TestCore::deinit()
{
//    Py_Finalize();
}

int quit(void*)
{
//    Py_Exit(0);

    return 0;
}

void TestCore::interruptTesting()
{
//    PyErr_SetInterrupt();
//    PyGILState_STATE state = PyGILState_Ensure();
//    Py_AddPendingCall(&quit, NULL);
//    PyGILState_Release(state);
//    PyErr_SetString(PyExc_KeyboardInterrupt, "...");
    if (DriverHolder::driver().physicalInterface() != nullptr)
    {
        DriverHolder::driver().physicalInterface()->closeConnection();
    }
}

void TestCore::setTestEnabled(TestPtr test, bool enabled)
{
    for (auto el : m_tests)
    {
        if (el.test == test)
        {
            el.enabled = enabled;
        }
    }
}

std::vector<TestPtr> TestCore::getActiveTests()
{
    std::vector<TestPtr> tests;

    for (auto el : m_tests)
    {
        if (el.enabled)
        {
            tests.push_back(el.test);
        }
    }

    return tests;
}

std::vector<TriggerTestPtr> TestCore::getActiveTriggers()
{
    std::vector<TriggerTestPtr> triggers;

    for (auto el : m_triggers)
    {
        if (el.enabled)
        {
            triggers.push_back(el.trigger);
        }
    }

    return triggers;
}

void TestCore::setTriggerEnabled(TriggerTestPtr trigger, bool enabled)
{
    for (auto el : m_triggers)
    {
        if (el.trigger == trigger)
        {
            el.enabled = enabled;
        }
    }
}

void TestCore::addTest(TestPtr test)
{
    TestData data;
    data.test = test;
    data.enabled = true;

    m_tests.push_back(data);
}

void TestCore::addTrigger(TriggerTestPtr trigger)
{
    TriggerData data;
    data.trigger = trigger;
    data.enabled = true;

    m_triggers.push_back(data);
}
