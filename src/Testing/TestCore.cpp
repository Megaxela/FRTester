//
// Created by megaxela on 21.11.16.
//

#include <Testing/TestCore.h>
#include <Testing/SettingsSystem.h>
#include <Testing/TestLogger.h>
#include <Testing/ManualTests/ManualTest.h>
#include <Testing/ManualTests/ManualTestFabric.h>
#include <Testing/StaticTestFabric.h>
#include <Testing/StaticTriggerFabric.h>
#include <Testing/ManualTests/Fabric.h>
#include <Tools/Logger.h>
#include <Tools/Time.h>
#include <Tools/StdExtend.h>
#include <Executor/TestingExecutor.h>
#include <TestDriverHolder.h>
#include <DriverHolder.h>

#define PY_LIST_DELIM ':'

TestCore::TestCore() :
    m_triggers(),
    m_tests(),
    m_environment(new TestEnvironment(
            &TestDriverHolder::driver(),
            &TestLogger::instance(),
            new TestingTools(
                    &TestDriverHolder::driver(),
                    &TestLogger::instance()
            )
    )),
    m_failedTriggers(),
    m_sysAdmPassword(30),
    m_testingExecutor(nullptr),
    m_sharedTests()
{
    m_sysAdmPassword = (uint32_t) std::stoul(SettingsSystem::instance().getValue(
                SettingsSystem::TestsRestoreStatePassword,
                "30"
        ));

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

    m_environment->tools()->m_executor = m_testingExecutor;

    // Загрузка статических тестов

    auto staticTestsNames = StaticTestFabric::instance().getNames();

    for (auto& testName : staticTestsNames)
    {
        auto test = StaticTestFabric::instance().create(testName);

        test->setEnvironment(m_environment);

        addTest(test);
    }

    auto staticTriggersNames = StaticTriggerFabric::instance().getNames();

    for (auto& triggerName : staticTriggersNames)
    {
        auto trigger = StaticTriggerFabric::instance().create(triggerName);

        trigger->setEnvironment(m_environment);

        addTrigger(trigger);
    }

    // Загрузка тестов из shared библиотек
    for (auto& library : m_sharedTests)
    {
        auto testCreator = SystemTools::getFunctionFromLibrary<
                AbstractTest*(TestEnvironment*)
        >(
                library,
                "createTest"
        );

        addTest(TestPtr(testCreator(m_environment)));
    }

    // Загрузка ручных тестов
    auto path = SettingsSystem::instance().getValue(
            SettingsSystem::TestsManualTestsPath,
            "tests/manual"
    );

    auto files = SystemTools::getAllFilesInDir(path);

    for (auto& file : files)
    {
        // Тест ли это
        if (!stdex::ends(file, ".json"))
        {
            continue;
        }

        std::ifstream fileStream(
                SystemTools::Path::join(
                        path,
                        file
                )
        );

        if (!fileStream.is_open())
        {
            Error("Не удалось открыть файл с тестов \"" + file + "\".");
            continue;
        }

        ManualTestPtr manualTest;

        try
        {
            manualTest = ManualTestFabric::createTest(json::parse(fileStream));
        }
        catch (std::invalid_argument e)
        {
            Error("Тест \"" + file + "\" поврежден. Не удалось его распарсить.");
            continue;
        }

        manualTest->setEnvironment(m_environment);
        addTest(manualTest);
    }

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
    Log("Загрузка shared библиотек с тестами.");
    auto sharedTestsDirectory = SettingsSystem::instance()
            .getValue(
                    SettingsSystem::TestsSharedTestsPath,
                    "tests/"
            );

    auto sharedTests = SystemTools::getAllFilesInDir(sharedTestsDirectory);

    for (auto &fileName : sharedTests)
    {
        if (stdex::ends(fileName, LIB_EXTENSION))
        {
            ExcessLog("Файл \"" + fileName + "\" является библиотекой.");
            Log("Загружаем \"" + fileName + "\"...");

            auto lib = SystemTools::loadLibrary(
                    SystemTools::Path::join(
                            sharedTestsDirectory,
                            fileName
                    )
            );

            if (lib == SystemTools::WrongLibrary)
            {
                Error("Не удалось загрузить библиотеку \"" +
                      fileName +
                      "\". Errno: " +
                      std::to_string(errno));
                continue;
            }
        }
        else
        {
            ExcessLog("Файл \"" + fileName + "\" не является библиотекой.");
        }
    }


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
//    if (DriverHolder::driver().physicalInterface() != nullptr)
//    {
//        DriverHolder::driver().physicalInterface()->closeConnection();
//    }
}

void TestCore::setTestEnabled(TestPtr test, bool enabled)
{
    for (auto& el : m_tests)
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

    for (auto& el : m_tests)
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

    for (auto& el : m_triggers)
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
    for (auto& el : m_triggers)
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

void TestCore::setTestExecutor(TestingExecutor *executor)
{
    m_testingExecutor = executor;
}

TestEnvironment *TestCore::environment() const
{
    return m_environment;
}

void TestCore::setUnitTestsController(UnitTestsController *controller)
{
    m_environment->tools()->setUnitTestsController(controller);
}

void TestCore::setRestorePassword(uint32_t password)
{
    SettingsSystem::instance().setValue(
            SettingsSystem::TestsRestoreStatePassword,
            std::to_string(password)
    );

    m_sysAdmPassword = password;
}

uint32_t TestCore::restorePassword() const
{
    return 0;
}
