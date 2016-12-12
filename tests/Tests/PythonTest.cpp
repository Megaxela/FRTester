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
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(PyObject_Str(pValue));

        Critical("Не удалось создать объект теста. Ошибка: " + std::string(pStrErrorMessage));
        Py_DECREF(pArguments);
        Py_DECREF(pEnvironment);
        throw std::runtime_error("Не удалось создать объект теста. Ошибка: " + std::string(pStrErrorMessage));
    }

    auto pAnswerResult = PyObject_CallMethod(pCreatedObject, "execute", NULL);

    if (pAnswerResult == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(PyObject_Str(pValue));

        Critical("Не удалось выполнить метод execute теста. Ошибка: " + std::string(pStrErrorMessage));

        Py_DECREF(pCreatedObject);
        Py_DECREF(pArguments);
        Py_DECREF(pEnvironment);

        throw std::runtime_error("Не удалось выполнить метод execute теста. Ошибка: " + std::string(pStrErrorMessage));
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
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        std::string str = "None";
        if (pStrErrorMessage)
        {
            str = std::string(pStrErrorMessage);
        }

        char* pStrExceptionType = PyString_AsString(PyObject_Repr(pExceptionType));
        if (pStrExceptionType)
        {
            str += " " + std::string(pStrExceptionType);
        }

        char* pStrStack = PyString_AsString(PyObject_Repr(pTraceback));
        if (pStrStack)
        {
            str += " " + std::string(pStrStack);
        }

        Critical("Не удалось загрузить модуль " + moduleName + ", ошибка: " + str);

        Py_DECREF(pModuleName);
        return false;
    }

    auto pDict = PyModule_GetDict(pModule);

    if (pDict == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        Critical("Не удалось загрузить словарь из модуля " + moduleName + ", ошибка: " + std::string(pStrErrorMessage));

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
