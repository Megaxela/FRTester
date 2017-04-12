//
// Created by megaxela on 03.02.17.
//

#ifndef FRTESTER_TESTINGTOOLS_H
#define FRTESTER_TESTINGTOOLS_H

#include <FRDriver.h>
#include "TestDriver.h"
#include <Testing/TestLogger.h>

class TestingExecutor;
class QWidget;
class UnitTestsController;

/**
 * @brief Класс, описывающий тестовые инструменты для
 * быстрого использования часто используемых функций.
 */
class TestingTools
{
    friend class TestCore;

public:
    enum class Printing
    {
        Unknown,
        Enabled,
        DisabledForOneCheck,
        Disabled
    };

    /**
     * @brief Конструктор.
     */
    TestingTools(TestDriver* testDriver, TestLogger* environment);

    /**
     * @brief Деструктор.
     */
    ~TestingTools();

    /**
     * @brief Метод для ожидания окончания печати.
     * @param timeout Таймаут, после которого ожидание
     * прекращается и возвращается false. Если timeout = 0
     * то таймаута нет.
     * @return Результат ожидания.
     */
    bool waitForPrintingFinished(uint32_t password, uint32_t timeout=0);

    /**
     * @brief Метод для переключения используемого
     * драйвера с тестировочного на обычный.
     * @param enabled Использовать ли тестировочный драйвер.
     */
    void enableTesting(bool enabled);

    /**
     * @brief Метод для смены режима печати.
     * @param password Пароль для чтения и записи таблиц.
     * @param mode Новый режим печати.
     * @return Старый режим печати.
     */
    Printing enablePrinting(uint32_t password, Printing mode);

    /**
     * @brief Метод для проверки работает ли еще тестирование.
     * @return Работает ли еще тестирование.
     */
    bool testingStoped();

    /**
     * @brief Метод для задачи вопроса пользователю.
     * @param question Вопрос пользователю.
     * @param acceptMessage Строка кнопки, возвращающая true
     * @param declineMessage Строка кнопки, возвращающая false
     * @return
     */
    bool messageQuestion(
            const std::string &question,
            const std::string &acceptMessage,
            const std::string &declineMessage
    );

    /**
     * @brief Метод для уведомления пользователя.
     * @param question Текст уведомления.
     */
    void messageNotify(
            const std::string &text
    );

    /**
     * @brief Метод для передачи родительского виджета.
     * @param parent Родительский виджет.
     */
    void setParentWidget(QWidget* parent);

    void setUnitTestsController(UnitTestsController* controller);

    /**
     * @brief Метод для безопасного выполнения определенной команды
     * драйвера. В этом методе контроллируется переподключение после
     * потери соединения.
     * @param methodToBeSafe Функция, которая будет использоваться в
     * качестве метода для выполнения.
     * @return True - если команда точно была выполнена.
     * False - если по какой-то причине команда выполниться не
     * смогла.
     */
    bool safeOperation(std::function<void(/*_Args&&...*/)> methodToBeSafe);

    /**
     * @brief Метод для произведения подключения при разрыве соединения.
     * @return True - если получилось подключиться к ККТ.
     * False - если по какой-то причине подключения не удалось.
     */
    bool performConnection();

    /**
     * @brief Метод для получения строки с ошибкой.
     * @return Строка с ошибкой вида "#0 Ошибок нет"
     */
    std::string getErrorString();

private:
    TestingTools(const TestingTools&) = delete;
    TestingTools& operator=(const TestingTools&) = delete;

    TestingExecutor* m_executor;

    FRDriver* m_currentDriver;
    TestDriver* m_testDriver;
    TestLogger* m_logger;
    QWidget* m_parentWidget;

    UnitTestsController* m_controller;
};


#endif //FRTESTER_TESTINGTOOLS_H
