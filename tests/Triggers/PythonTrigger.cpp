//
// Created by megaxela on 01.12.16.
//

#include "PythonTrigger.h"
#include <include/Tools/Logger.h>
#include <cython/frdriver.h>
#include <algorithm>

PythonTrigger::PythonTrigger(TestEnvironment *environment,
                             const std::string &name,
                             const std::string &description,
                             bool critical,
                             const std::vector<std::string> &tags)
        : AbstractTriggerTest(environment,
                              name,
                              description,
                              critical),
          m_pEnvironment(nullptr),
          m_pTriggerName(nullptr),
          m_pModule(nullptr),
          m_object(nullptr),
          m_tags(tags)
{}

PythonTrigger::~PythonTrigger()
{
    if (m_object)
    {
        Py_DECREF(m_object);
    }

    if (m_pModule)
    {
        Py_DECREF(m_pModule);
    }

    if (m_pEnvironment)
    {
        Py_DECREF(m_pEnvironment);
    }
}

void PythonTrigger::onPreExecute(const std::string &realTag)
{
    if (m_pTriggerName == nullptr)
    {
        Critical("Попытка вызвать незагруженный тест.");
        return;
    }

    m_pEnvironment = createEnvironment(environment());

    auto pTagString = PyString_FromString(realTag.c_str());

    auto pArguments = Py_BuildValue("(O,s)", m_pEnvironment, pTagString);

    if (pArguments == nullptr)
    {
        Critical("Не удалось создать аргументы для создания класса.");
        throw std::runtime_error("Не удалось создать аргументы для создания класса.");
    }

    m_object = PyObject_CallObject(m_pTriggerName, pArguments);

    Py_DECREF(pTagString);
    Py_DECREF(m_pEnvironment);
    Py_DECREF(pArguments);

    if (m_object == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        m_pEnvironment = nullptr;
        Critical("Не удалось создать объект теста. Ошибка: " + std::string(pStrErrorMessage));
        throw std::runtime_error("Не удалось создать объект теста. Ошибка: " + std::string(pStrErrorMessage));
    }

    auto pPreResult = PyObject_CallMethod(m_object, "on_pre_execute", NULL);

    if (pPreResult == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        Py_DECREF(m_object);
        m_object = nullptr;
        m_pEnvironment = nullptr;
        Critical("Не удалось выполнить метод on_post_execute теста. Ошибка: " + std::string(pStrErrorMessage));

        throw std::runtime_error("Не удалось выполнить метод on_pre_execute теста. Ошибка: " + std::string(pStrErrorMessage));
    }
}

void PythonTrigger::onPostExecute()
{
    if (m_object == nullptr)
    {
        Critical("Попытка вызвать onPostExecute перед onPreExecute.");
        return;
    }

    auto pPostResult = PyObject_CallMethod(m_object, "on_post_execute", NULL);

    if (pPostResult == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        Py_DECREF(m_object);
        m_object = nullptr;
        m_pEnvironment = nullptr;
        Critical("Не удалось выполнить метод on_post_execute теста. Ошибка: " + std::string(pStrErrorMessage));

        throw std::runtime_error("Не удалось выполнить метод on_post_execute теста. Ошибка: " + std::string(pStrErrorMessage));
    }
}

bool PythonTrigger::succeed()
{
    if (m_object == nullptr)
    {
        Critical("Попытка вызвать onPostExecute перед onPreExecute.");
        return false;
    }

    auto pPostResult = PyObject_CallMethod(m_object, "succeed", NULL);

    if (pPostResult == nullptr)
    {
        PyObject* pExceptionType;
        PyObject* pValue;
        PyObject* pTraceback;

        PyErr_Fetch(&pExceptionType, &pValue, &pTraceback);

        char* pStrErrorMessage = PyString_AsString(pValue);

        Py_DECREF(m_object);
        m_object = nullptr;
        m_pEnvironment = nullptr;
        Critical("Не удалось выполнить метод succeed теста. Ошибка: " + std::string(pStrErrorMessage));

        throw std::runtime_error("Не удалось выполнить метод succeed теста. Ошибка: " + std::string(pStrErrorMessage));
    }

    if (!PyBool_Check(pPostResult))
    {
        Critical("Результат вызова success возвращает не bool.");
        return false;
    }

    return PyObject_IsTrue(pPostResult) == 1;
}

TriggerTestPtr PythonTrigger::loadTrigger(TestEnvironment *environment, const std::string &moduleName, const std::string &modulePath)
{
    auto pModuleName = PyString_FromString(moduleName.c_str());

    PySys_SetPath((char*) modulePath.c_str());

    if (pModuleName == nullptr)
    {
        Critical("Не удалось создать Python строку из строки \"" + moduleName + "\".");
        return nullptr;
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

        Critical("Не удалось загрузить модуль " + moduleName + ", ошибка: " + str);

        Py_DECREF(pModuleName);
        return nullptr;
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

        return nullptr;
    }

    auto pTriggerName = PyDict_GetItemString(pDict, "Trigger");

    if (pTriggerName == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Не удалось найти класс с названием \"Trigger\".");
        return nullptr;
    }

    if (!PyCallable_Check(pTriggerName))
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Объект в модуле " + moduleName + " под названием \"Trigger\" не является вызываемым.");
        return nullptr;
    }

    auto pName = PyObject_GetAttrString(pTriggerName, "name");

    if (pName == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Класс не имеет названия или оно не является строкой.");
        return nullptr;
    }

    auto pDescription = PyObject_GetAttrString(pTriggerName, "description");

    if (pDescription == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Класс не имеет описания или оно не является строкой.");
        return nullptr;
    }

    auto pCritical = PyObject_GetAttrString(pTriggerName, "critical");

    if (pCritical == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Не установлен уровень критичности триггера или он не bool.");
        return nullptr;
    }

    auto pTags = PyObject_GetAttrString(pTriggerName, "tags");

    if (pTags == nullptr)
    {
        Py_DECREF(pModuleName);
        Py_DECREF(pModule);
        Critical("Класс не имеет тэгов или это не массив.");
        return nullptr;
    }

    auto size = PyList_GET_SIZE(pTags);

    std::vector<std::string> tagsVector;
    for (int i = 0; i < size; ++i)
    {
        auto element = PyList_GetItem(pTags, i);
        if (!PyString_Check(element))
        {
            Py_DECREF(pModuleName);
            Py_DECREF(pModule);
            Critical("Один из тэгов не является строкой.");
            return nullptr;
        }

        tagsVector.push_back(std::string(PyString_AsString(element)));
    }

    auto trigger = std::make_shared<PythonTrigger>(
            environment,
            PyString_AsString(pName),
            PyString_AsString(pDescription),
            PyObject_IsTrue(pCritical) == 1,
            tagsVector
    );

    trigger->m_pTriggerName = pTriggerName;
    trigger->m_pModule = pModule;

    return trigger;
}

bool PythonTrigger::containsTag(const std::string &tag)
{
    return std::find(
            m_tags.begin(),
            m_tags.end(),
            tag
    ) != m_tags.end();
}
