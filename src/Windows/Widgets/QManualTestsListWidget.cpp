//
// Created by megaxela on 20.02.17.
//

#include <netinet/in.h>
#include <QtCore/QMetaObject>
#include <QtWidgets/QListWidget>
#include <QtCore/QObject>
#include <QtGui/QtGui>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractScrollArea>
#include "include/Windows/Widgets/QManualTestsListWidget.h"
#include <QListWidgetItem>
#include <include/Testing/ManualTests/Actions/BeepTestAction.h>
#include <include/Testing/ManualTests/Actions/LongStateRequestTestAction.h>
#include <include/Testing/ManualTests/Actions/WhileTestAction.h>
#include <include/Tools/Logger.h>
#include <include/Testing/SettingsSystem.h>
#include <include/Tools/StdExtend.h>
#include <include/Testing/ManualTests/ManualTestFabric.h>

QManualTestsListWidget::QManualTestsListWidget(QWidget *parent) :
        QListWidget(parent),
        m_manualTests()
{
    connect(this,
            &QManualTestsListWidget::itemSelectionChanged,
            this,
            &QManualTestsListWidget::onSelectionChanged);
}

QManualTestsListWidget::~QManualTestsListWidget()
{
    saveTests();
}

void QManualTestsListWidget::updateTests()
{
    m_manualTests.clear();

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
            Error("Тест \"" + file + "\" поврежден. Не удалось его распарсить. Ошибка: " + e.what());
            continue;
        }

        m_manualTests.push_back(std::make_pair(file, manualTest));
    }

    displayTests();
}

void QManualTestsListWidget::displayTests()
{
    clear();

    for (auto& element : m_manualTests)
    {
        QListWidgetItem* item = new QListWidgetItem(this);
        item->setText(QString::fromStdString(element.second->name()));
        item->setToolTip(QString::fromStdString(element.second->description()));
        addItem(item);
    }
}

void QManualTestsListWidget::onSelectionChanged()
{
    onTestSelected(m_manualTests[currentIndex().row()].second);
}

void QManualTestsListWidget::saveTests()
{
    auto path = SettingsSystem::instance().getValue(
            SettingsSystem::TestsManualTestsPath,
            "tests/manual"
    );

    for (auto& test : m_manualTests)
    {
        if (!test.first.empty() && test.first != test.second->name() + ".json")
        {
            std::remove(
                    SystemTools::Path::join(
                            path,
                            test.first
                    ).c_str()
            );
        }

        // Сериализуем тест
        auto serializedTest = ManualTestFabric::serializeTestToJSON(test.second);

        // Записываем его
        std::ofstream outputStream(SystemTools::Path::join(
                path,
                test.second->name() + ".json"
        ));

        test.first = test.second->name() + ".json";

        outputStream << serializedTest.dump(4);

        outputStream.close();
    }
}

void QManualTestsListWidget::addTest(ManualTestPtr test)
{
    uint32_t add = 0;
    uint32_t index = 0;
    while (true)
    {
        if (index == m_manualTests.size())
        {
            break;
        }

        std::string newName = test->name();
        if (add != 0)
        {
            newName += ' ' + std::to_string(add);
        }

        if (m_manualTests[index].second->name() == newName)
        {
            index = 0;
            ++add;
        }
        else
        {
            ++index;
        }

    }

    if (add != 0)
    {
        test->setName(test->name() + ' ' + std::to_string(add));
    }

    m_manualTests.push_back(std::make_pair(std::string(), test));
    displayTests();
}

void QManualTestsListWidget::removeTest(uint32_t index)
{
    // Удаляем файл с тестом
    auto realPath = SettingsSystem::instance().getValue(
            SettingsSystem::TestsManualTestsPath,
            "tests/manual"
    );

    if (m_manualTests[index].first.empty())
    {
        realPath = SystemTools::Path::join(
                realPath,
                m_manualTests[index].second->name()
        );
    }
    else
    {
        realPath = SystemTools::Path::join(
                realPath,
                m_manualTests[index].first
        );
    }

    if (SystemTools::Path::fileExists(realPath))
    {
        std::remove(realPath.c_str());
    }
    else
    {
        Error("Удаляемый тест не сохранен.");
    }

    // Удаляем сам тест из массива
    m_manualTests.erase(m_manualTests.begin() + index);
    displayTests();
}
