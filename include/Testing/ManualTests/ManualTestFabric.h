//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_MANUALTESTFABRIC_H
#define FRTESTER_MANUALTESTFABRIC_H

#include <libraries/json.hpp>
#include <include/Testing/ManualTests/ManualTest.h>
#include <include/Testing/ManualTests/AbstractTestAction.h>

using json = nlohmann::json;

/**
 * @brief Область имен фабрики для работы
 * с ручными тестами.
 */
namespace ManualTestFabric
{
    /**
     * @brief Функция для создания теста из JSON. При
     * неправильном значении будет выкинуто исключение
     * std::invalid_argument.
     * @param tst JSON значение теста. Формат:
     * {
     *     "name" : "Test #1",
     *     "description": "Some cool description",
     *     "actions": {
     *          ... Формат действий см. ManualTestFabric::createAction
     *      },
     * }
     * @return Объект теста.
     */
    ManualTestPtr createTest(json tst);

    /**
     * @brief Функция для преобразования теста в JSON значение.
     * @param tst Тест для преобразования.
     * @return JSON значение.
     */
    json serializeTestToJSON(ManualTestPtr tst);

    /**
     * @brief Функция для создания действия из JSON.
     * При неправильном формате будет выкинуто исключение
     * std::invalid_argument.
     * @param action JSON значение действия. Формат:
     * {
     *     "action": "BeepTestAction",
     *     "variables": [
     *          { ... см. формат сериализации DataValue }
     *     ]
     * }
     * @return Объект теста.
     */
    TestActionPtr createAction(json action);

    /**
     * @brief Функция для преобразования действия в JSON значение.
     * @param action Действие для преобразования.
     * @return JSON значение.
     */
    json serializeActionToJSON(TestActionPtr action);
};


#endif //FRTESTER_MANUALTESTFABRIC_H
