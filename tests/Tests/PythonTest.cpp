//
// Created by megaxela on 28.11.16.
//

#include "PythonTest.h"
#include <Python.h>
#include "cython/frdriver.h"
#include <include/Tools/Logger.h>

PythonTest::PythonTest(TestEnvironment *environment) :
    AbstractTest(environment, std::string(), std::string()),
    m_pTest(nullptr),
    m_pModule(nullptr)
{

}

PythonTest::~PythonTest()
{
    if (m_pModule)
    {
        Py_DECREF(m_pModule);
    }
}

bool PythonTest::execute()
{
    if (m_pTest == nullptr)
    {
        Critical("Попытка вызвать незагруженный тест.");
        return false;
    }

    // Формируем аргументы для создания класса

    auto pEnvironment = createEnvironment(enviroment());

    auto pArguments = Py_BuildValue("(O)", pEnvironment);

    if (pArguments == nullptr)
    {
        Critical("Не удалось создать аргументы для создания класса.");
        throw std::runtime_error("Не удалось создать аргументы для создания класса.");
    }

    auto pCreatedObject = PyObject_CallObject(m_pTest, pArguments);

    if (pCreatedObject == nullptr)
    {
        std::string error = getPythonError();

        Critical("Не удалось создать объект теста. Ошибка: " + error);
        Py_DECREF(pArguments);
        Py_DECREF(pEnvironment);
        throw std::runtime_error("Не удалось создать объект теста. Ошибка: " + error);
    }

    auto pAnswerResult = PyObject_CallMethod(pCreatedObject, "execute", NULL);

    if (pAnswerResult == nullptr)
    {
        std::string error = getPythonError();
        Critical("Не удалось выполнить метод execute теста. Ошибка: " + error);

        Py_DECREF(pCreatedObject);
        Py_DECREF(pArguments);
        Py_DECREF(pEnvironment);

        throw std::runtime_error("Не удалось выполнить метод execute теста. Ошибка: " + error);
    }

    if (!PyBool_Check(pAnswerResult))
    {
        Critical("Результат выполнения теста должен быть bool.");

        Py_DECREF(pAnswerResult);
        Py_DECREF(pCreatedObject);
        Py_DECREF(pArguments);
        Py_DECREF(pEnvironment);
        throw std::runtime_error("Результат выполнения теста должен быть bool.");
    }

    bool result = PyObject_IsTrue(pAnswerResult) == 1;

    Py_DECREF(pAnswerResult);
    Py_DECREF(pCreatedObject);
    Py_DECREF(pArguments);
    Py_DECREF(pEnvironment);

    return result;
}

bool PythonTest::loadModule(const std::string &modulePath, const std::string &moduleName)
{
    auto pModuleName = PyString_FromString(moduleName.c_str());

    char* m = (char*) modulePath.c_str();
    m_testPath = modulePath;

    if (pModuleName == nullptr)
    {
        Critical("Не удалось создать Python строку из строки \"" + moduleName + "\"");
        return false;
    }

    auto pModule = PyImport_Import(pModuleName);

    if (pModule == nullptr)
    {
        Critical("Не удалось загрузить модуль " + moduleName + ", ошибка: " + getPythonError());

        Py_DECREF(pModuleName);
        return false;
    }

    auto pDict = PyModule_GetDict(pModule);

    if (pDict == nullptr)
    {
        Critical("Не удалось загрузить словарь из модуля " + moduleName + ", ошибка: " + getPythonError());

        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        return false;
    }

    m_pTest = PyDict_GetItemString(pDict, "Test");

    if (m_pTest == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Не удалось найти класс с названием \"Test\".");
        return false;
    }

    if (!PyCallable_Check(m_pTest))
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Объект в модуле " + moduleName + " под названием \"Test\" не является вызываемым.");
        return false;
    }

    auto pName = PyObject_GetAttrString(m_pTest, "name");

    if (pName == nullptr)
    {
        Critical("Класс не имеет названия.");
        return false;
    }

    auto pDescription = PyObject_GetAttrString(m_pTest, "description");

    if (pDescription == nullptr)
    {
        Critical("Класс не имеет описания.");
        return false;
    }

    setName(PyString_AsString(pName));
    setDescription(PyString_AsString(pDescription));

    m_pModule = pModule;
    Py_DECREF(pModuleName);

    return true;
}

TestPtr PythonTest::loadTest(TestEnvironment *environment, const std::string &moduleName, const std::string &modulePath)
{
    auto test = std::make_shared<PythonTest>(environment);

    if (!test->loadModule(modulePath, moduleName))
    {
        return nullptr;
    }

    return test;
}

std::string PythonTest::getPythonError()
{
    std::string result;

    PyObject* pExceptionType;
    PyObject* pValue;
    PyObject* pTraceback;

    PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

    if (pValue == nullptr && pExceptionType == nullptr && pTraceback == nullptr)
    {
        return "Unknown error";
    }

    if (pValue != nullptr)
    {
        char* pValueStr = PyString_AsString(pValue);

        if (pValueStr)
        {
            result += "Exception: " + std::string(pValueStr);
        }
    }

    return result;
}
