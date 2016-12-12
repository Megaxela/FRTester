//
// Created by megaxela on 21.11.16.
//

#include <include/Testing/TestCore.h>
#include <include/DriverHolder.h>
#include <tests/Tests/CycleTest.h>
#include <Tools/Logger.h>
#include <Python.h>
#include <include/Tools/Settings.h>
#include <tests/Tests/PythonTest.h>
#include <cython/frdriver.h>
#include <tests/Triggers/PythonTrigger.h>
#include <include/Testing/TestLogger.h>
#include <include/TestDriverHolder.h>

#define PY_LIST_DELIM ':'

TestCore::TestCore()
{
    m_environment = new TestEnvironment(
            &TestDriverHolder::driver(),
            &TestLogger::instance()
    );

    init();
}

TestCore::~TestCore()
{
    delete m_environment;

    Py_Finalize();
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
//    m_tests.push_back(std::make_shared<CycleTest>(m_environment));

    if (Py_IsInitialized())
    {
        Log("Переинициализация интерпретатора.");

        deinit();
        init();
    }

    std::string testsPath = Settings::instance().getValue(
            SETTINGS_NAMES_TESTSPATH, "tests"
    );

    // Загрузка Python тестов
    std::string pythonTestsPath = SystemTools::Path::join(testsPath, "python");

    if (!testsPath.empty())
    {
        // Получение всех файлов в директории
        std::vector<std::string> testFiles = SystemTools::getAllFilesInDir(pythonTestsPath);
        if (testFiles.empty())
        {
            Error("Папки с тестами не существует или тесты отсутствуют.");
        }

        for (auto filename : testFiles)
        {
            // Название файла слишком мало
            if (filename.size() <= 3)
            {
                continue;
            }

            // Заканчивается ли файл на .py
            if (filename.substr(filename.length() - 3, 3) != ".py")
            {
                continue;
            }

            auto loadedTest = PythonTest::loadTest(
                    m_environment,
                    filename.substr(0, filename.length() - 3),
                    pythonTestsPath
            );

            if (loadedTest != nullptr)
            {
                m_tests.push_back(loadedTest);
            }
        }
    }
    else
    {
        Critical("Путь к тестам пуст.");
    }

    std::string triggersPath = Settings::instance().getValue(
            SETTINGS_NAMES_TRIGGERSPATH, "triggers"
    );

    std::string pythonTriggersPath = SystemTools::Path::join(
            triggersPath, "python"
    );

    if (!triggersPath.empty())
    {
        // Получение всех файлов в директории
        std::vector<std::string> testFiles = SystemTools::getAllFilesInDir(pythonTriggersPath);
        if (testFiles.empty())
        {
            Error("Папки с триггерами не существует или тесты отсутствуют.");
        }

        for (auto filename : testFiles)
        {
            // Название файла слишком мало
            if (filename.size() <= 3)
            {
                continue;
            }

            // Заканчивается ли файл на .py
            if (filename.substr(filename.length() - 3, 3) != ".py")
            {
                continue;
            }

            auto loadedTrigger = PythonTrigger::loadTrigger(
                    m_environment,
                    filename.substr(0, filename.length() - 3),
                    pythonTriggersPath
            );

            if (loadedTrigger != nullptr)
            {
                m_triggers.push_back(loadedTrigger );
            }
        }
    }
    else
    {
        Error("Путь к триггерам пуст.");
    }
}

void TestCore::getTriggers(const std::string &tag,
                           std::vector<TriggerTestPtr> &v)
{
    for (auto trigger : m_triggers)
    {
        if (trigger->containsTag(tag))
        {
            v.push_back(trigger);
        }
    }
}

bool TestCore::restoreFRState()
{
    if (!DriverHolder::driver().checkConnection())
    {
        return false;
    }

    auto state = DriverHolder::driver().fullStateRequest(30);
    if (state.posMode == 8) // Открытый документ
    {
        // Отменяем документ
        if (!DriverHolder::driver().cancelCheck(30))
        {
            Error("Не удалось отменить открытый чек.");
            return false;
        }
    }
    else if (state.posMode == 2) // Открытая смена
    {
        // Закрываем смену
        if (!DriverHolder::driver().shiftCloseReport(30))
        {
            Error("Не удалось закрыть смену.");
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
    return m_tests;
}

std::vector<TriggerTestPtr> TestCore::getTriggers()
{
    return m_triggers;
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
    Py_Initialize();
    initfrdriver();
    Py_SetPythonHome("python2/");

    PyObject* sysModule = PyImport_ImportModule("sys");
    if (sysModule == nullptr)
    {
        Critical("Can't import sys.");
        return;
    }


    PyObject* pathObject = PyObject_GetAttrString(sysModule, "path");
    if (pathObject == nullptr)
    {
        Critical("Can't find path property in `sys` module.");
        return;
    }

    PyList_Append(pathObject, PyString_FromString(
            SystemTools::Path::join(Settings::instance().getValue(
                    SETTINGS_NAMES_TRIGGERSPATH,
                    "./triggers/"
            ), "python").c_str()
    ));

    PyList_Append(pathObject, PyString_FromString(
            SystemTools::Path::join(Settings::instance().getValue(
                    SETTINGS_NAMES_TESTSPATH,
                    "./tests/"
            ), "python").c_str()
    ));
}

void TestCore::deinit()
{
    Py_Finalize();
}
