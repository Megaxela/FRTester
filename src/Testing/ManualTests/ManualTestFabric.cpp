//
// Created by megaxela on 17.02.17.
//

#include <include/Testing/ManualTests/Actions/BeepTestAction.h>
#include <include/Tools/Logger.h>
#include <include/Testing/ManualTests/Actions/LongStateRequestTestAction.h>
#include <include/Testing/ManualTests/Actions/WhileTestAction.h>
#include "include/Testing/ManualTests/ManualTestFabric.h"

#define ADD_ACTION(NAME)\
else if(actionName == ""#NAME"")\
{\
    testAction = std::make_shared<NAME>();\
}

#define BEG_ACTION(NAME)\
if(actionName == ""#NAME"")\
{\
    testAction = std::make_shared<NAME>();\
}

#define END_ACTION()\
else\
{\
    throw std::invalid_argument("Неизвестное значение действия \"" + actionName + "\"");\
}

ManualTestPtr ManualTestFabric::createTest(json tst)
{
    ManualTestPtr manualTest = std::make_shared<ManualTest>(
            tst["name"].get<std::string>(),
            tst["description"].get<std::string>()
    );

    for (auto& element : tst["actions"])
    {
        manualTest->addTestAction(createAction(element));
    }

    return manualTest;
}

json ManualTestFabric::serializeTestToJSON(ManualTestPtr tst)
{
    json j;
    j["name"] = tst->name();
    j["description"] = tst->description();
    j["actions"] = std::vector<json>();

    for (uint32_t i = 0; i < tst->numberOfActions(); ++i)
    {
        j["actions"][i] = serializeActionToJSON(tst->getTestAction(i));
    }

    return j;
}

TestActionPtr ManualTestFabric::createAction(json action)
{
    std::string actionName = action["action"];

    TestActionPtr testAction;

    BEG_ACTION(BeepTestAction)
    ADD_ACTION(LongStateRequestTestAction)
    ADD_ACTION(WhileTestAction)
    END_ACTION()

    // Загрузка значений переменных
    for (auto& element : action["variables"])
    for (json::iterator element = action["variables"].begin();
         element != action["variables"].end();
         ++element)
    {
        DataValue value;
        DataValue::from_json(element.value(), value);
        testAction->setValue(element.key(), value);
    }

    // Сериализация дочерних объектов
    for (auto& element : action["children"])
    {
        testAction->addChild(
                createAction(element)
        );
    }

    return testAction;
}

json ManualTestFabric::serializeActionToJSON(TestActionPtr action)
{
    json j;
    j["action"] = SystemTools::getTypeName(*action.get());
    j["variables"] = std::map<std::string, json>();

    auto variables = action->getAllVariables();

    // Переменные
    for (auto& pair : variables)
    {
        json value;
        DataValue::to_json(value, pair.second);
        j["variables"][pair.first] = value;
    }

    // Дети
    auto children = action->children();
    j["children"] = std::vector<json>();
    for (auto& child : children)
    {
        j["children"].push_back(serializeActionToJSON(child));
    }

    return j;
}
