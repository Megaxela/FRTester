//
// Created by megaxela on 17.02.17.
//

#include "include/Windows/Controllers/TestControllers/TestsEditorController.h"
#include "Windows/Widgets/QManualTestsListWidget.h"
#include <ui_mainwindow.h>
#include <include/Windows/Widgets/QActionTreeWidgetItem.h>
#include <include/Tools/GUIHelper.h>
#include <include/Windows/Widgets/QNumberLineEdit.h>
#include <QtWidgets/QCheckBox>
#include <QModelIndex>
#include <include/Windows/Widgets/QHexLineEdit.h>

TestsEditorController::TestsEditorController(Ui::MainWindow *ptr, QWidget *parent):
    AbstractTabController(ptr, parent),
    m_currentTest(),
    m_editingAction(),
    m_editorDisconnected(false)
{

}

TestsEditorController::~TestsEditorController()
{

}

void TestsEditorController::setupConnections()
{
    connect(this,
            &TestsEditorController::actionChanged,
            ui()->testsEdit_testActionsLineEdit,
            &QTestDisplayTreeWidget::onActionChanged);
}

void TestsEditorController::tabSelected()
{
    ui()->testsEditor_testsListWidget->updateTests();
}

void TestsEditorController::configureWidgets()
{
    connect(ui()->testsEditor_testsListWidget,
            &QManualTestsListWidget::onTestSelected,
            this,
            &TestsEditorController::onTestSelectorTestSelected);

    connect(ui()->testsEdit_testNameLineEdit,
            &QLineEdit::textChanged,
            this,
            &TestsEditorController::onTestNameChanged);

    connect(ui()->testsEdit_testActionsSearchLineEdit,
            &QLineEdit::textChanged,
            this,
            &TestsEditorController::onTestActionsSearchChanged);

    connect(ui()->testsEdit_actionsSearchLineEdit,
            &QLineEdit::textChanged,
            this,
            &TestsEditorController::onAvailableActionsSearchChanged);

    connect(ui()->testsEdit_availableActionsTreeWidget,
            &QTestActionsTreeWidget::onActionSelected,
            this,
            &TestsEditorController::onAvailableActionElementSelected);

    connect(ui()->testsEdit_testActionsLineEdit,
            &QTestDisplayTreeWidget::onActionSelected,
            this,
            &TestsEditorController::onAvailableActionElementSelected);

    connect(ui()->testsEditor_addTestPushButton,
            &QPushButton::clicked,
            this,
            &TestsEditorController::onAddTestButtonPressed);

    connect(ui()->testsEditor_removeTestPushButton,
            &QPushButton::clicked,
            this,
            &TestsEditorController::onRemoveTestButtonPressed);
}

void TestsEditorController::onTestNameChanged(QString name)
{
    if (!m_currentTest || m_editorDisconnected)
    {
        return;
    }

    m_currentTest->setName(name.toStdString());

    ui()->testsEdit_testActionsLineEdit->applyChanges();

    ui()->testsEditor_testsListWidget->saveTests();

    ui()->testsEditor_testsListWidget->displayTests();
}

void TestsEditorController::onTestSelectorTestSelected(ManualTestPtr test)
{
    disconnectEditor();

    // Применяем изменения
    ui()->testsEdit_testActionsLineEdit->applyChanges();

    // Сохраняем сразу после изменения
    ui()->testsEditor_testsListWidget->saveTests();

    // Очищаем место для изменения
    clearChecker();

    // Устанавливаем текущий тест
    m_currentTest = test;

    // Отрисовываем его и передаем остальным виджетам
    displayCurrentTest();

    // Соединяем все обратно (нет)
    connectEditor();
}

void TestsEditorController::tabLeaved()
{
    ui()->testsEdit_testActionsLineEdit->applyChanges();
    ui()->testsEditor_testsListWidget->saveTests();
}

void TestsEditorController::clearChecker()
{
    ui()->testsEdit_testNameLineEdit->clear();

    GUIHelper::cleanFormLayout(ui()->testsEdit_actionSettingsFormLayout);

    ui()->testsEdit_testActionsLineEdit->setManualTest(nullptr);
}

void TestsEditorController::displayCurrentTest()
{
    if (!m_currentTest)
    {
        return;
    }

    ui()->testsEdit_testNameLineEdit->setText(
            QString::fromStdString(
                    m_currentTest->name()
            )
    );

    ui()->testsEdit_testActionsLineEdit->setManualTest(m_currentTest);
}

void TestsEditorController::onTestActionsSearchChanged(QString name)
{
    ui()->testsEdit_testActionsLineEdit->setSearchFilter(name);
}

void TestsEditorController::onAvailableActionsSearchChanged(QString name)
{
    ui()->testsEdit_availableActionsTreeWidget->setSearchFilter(name);
}

void TestsEditorController::disconnectEditor()
{
    m_editorDisconnected = true;
}

void TestsEditorController::connectEditor()
{
    m_editorDisconnected = false;
}

void TestsEditorController::onAvailableActionElementSelected(TestActionPtr item)
{
    m_editingAction = item;
    GUIHelper::cleanFormLayout(ui()->testsEdit_actionSettingsFormLayout);
    ui()->testsEdit_actionNameLabel->clear();

    if (m_editingAction == nullptr)
    {
        return;
    }

    auto variables = m_editingAction->getAllVariables();

    for (auto& variable : variables)
    {
        QWidget* widget = nullptr;

        // Выбор виджета для типа данных
        switch (variable.second.type)
        {
        case DataValue::Type::UInt8:
        {
            auto numberLineEdit = new QNumberLineEdit<uint8_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toUInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint8_t>::newUInt8ValueAccepted,
                    [=](uint8_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int8:
        {
            auto numberLineEdit = new QNumberLineEdit<int8_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<int8_t>::newInt8ValueAccepted,
                    [=](int8_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt16:
        {
            auto numberLineEdit = new QNumberLineEdit<uint16_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toUInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint16_t>::newUInt16ValueAccepted,
                    [=](uint16_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int16:
        {
            auto numberLineEdit = new QNumberLineEdit<int16_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<int16_t>::newInt8ValueAccepted,
                    [=](int16_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt32:
        {
            auto numberLineEdit = new QNumberLineEdit<uint32_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toUInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint32_t>::newUInt32ValueAccepted,
                    [=](uint32_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int32:
        {
            auto numberLineEdit = new QNumberLineEdit<int32_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<int32_t>::newInt32ValueAccepted,
                    [=](int32_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt64:
        {
            auto numberLineEdit = new QNumberLineEdit<uint64_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toUInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint64_t>::newUInt64ValueAccepted,
                    [=](uint64_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int64:
        {
            auto numberLineEdit = new QNumberLineEdit<int64_t>(parentWidget());
            numberLineEdit->setNumberValue(variable.second.toInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<int64_t>::newInt64ValueAccepted,
                    [=](int64_t value)
                    {
                        m_editingAction->setValue(variable.first, value);
                        emit actionChanged(m_editingAction);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Boolean:
        {
            auto checkBox = new QCheckBox(parentWidget());
            checkBox->setChecked(variable.second.toBoolean());
            connect(checkBox,
                    &QCheckBox::stateChanged,
                    [=](int state)
                    {
                        m_editingAction->setValue(variable.first, (bool) state);
                        emit actionChanged(m_editingAction);
                    });

            widget = checkBox;
            break;
        }
        case DataValue::Type::ByteArray:
        {
            auto hexLineEdit = new QHexLineEdit(parentWidget());
            hexLineEdit->setByteArray(variable.second.toByteArray());
            connect(hexLineEdit,
                    &QHexLineEdit::byteArrayChanged,
                    [=](ByteArray n)
                    {
                        m_editingAction->setValue(variable.first, n);
                        emit actionChanged(m_editingAction);
                    });
            widget = hexLineEdit;
            break;
        }
        case DataValue::Type::String:
        {
            auto stringEdit = new QLineEdit(parentWidget());
            stringEdit->setText(
                    QString::fromStdString(
                            variable.second.toString()
                    )
            );
            connect(stringEdit,
                    &QLineEdit::textChanged,
                    [=](QString s)
                    {
                        m_editingAction->setValue(variable.first, s.toStdString());
                        emit actionChanged(m_editingAction);
                    });
            widget = stringEdit;
            break;
        }
        default:
            Log("Unknown value type. What??");
        }

        if (widget != nullptr)
        {
            ui()->testsEdit_actionSettingsFormLayout->addRow(
                    QString::fromStdString(variable.first),
                    widget
            );
        }
    }

    // Вывод названия

    ui()->testsEdit_actionNameLabel->setText(
            QString::fromStdString(
                    m_editingAction->name()
            )
    );
}

void TestsEditorController::onAddTestButtonPressed()
{
    // Создаем пустой тест
    auto test = std::make_shared<ManualTest>("Новый тест", "");

    // Добавлем тест
    ui()->testsEditor_testsListWidget->addTest(test);

    // Применяем
    ui()->testsEdit_testActionsLineEdit->applyChanges();

    // Сохраняем его
    ui()->testsEditor_testsListWidget->saveTests();
}

void TestsEditorController::onRemoveTestButtonPressed()
{
    // Получаем индекс удаляемого теста
    auto row = ui()->testsEditor_testsListWidget->currentIndex().row();

    if (row == -1)
    {
        return;
    }

    // Удаляем тест. Совсем и навсегда.
    ui()->testsEditor_testsListWidget->removeTest((uint32_t) row);
}
