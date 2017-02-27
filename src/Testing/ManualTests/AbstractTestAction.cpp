//
// Created by megaxela on 17.02.17.
//

#include "include/Testing/ManualTests/AbstractTestAction.h"
#include <Testing/ManualTests/ManualTest.h>
#include <QString>
#include <QTreeWidgetItem>
#include <include/Tools/Logger.h>
#include <include/Windows/Widgets/QActionTreeWidgetItem.h>

AbstractTestAction::AbstractTestAction(const std::string &name,
                                       const std::string &description,
                                       const std::vector<std::pair<std::string, DataValue>> &fields) :
    m_manualTest(nullptr),
    m_name(name),
    m_description(description),
    m_dynamicValues(fields),
    m_allowChildren(false)
{

}

AbstractTestAction::~AbstractTestAction()
{

}

std::string AbstractTestAction::name() const
{
    return m_name;
}

std::string AbstractTestAction::description() const
{
    return m_description;
}

std::vector<std::pair<std::string, DataValue::Type>> AbstractTestAction::getVariables() const
{
    std::vector<std::pair<std::string, DataValue::Type>> result;

    for (auto& data : m_dynamicValues)
    {
        result.push_back(std::make_pair(data.first, data.second.type));
    }

    return result;
}

void AbstractTestAction::setValue(const std::string &name, const DataValue &value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == value.type)
        {
            element.second = value;
            return;
        }
    }

    throw std::invalid_argument("Не удалось найти значение \"" + name + "\".");
}

DataValue AbstractTestAction::getValue(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name)
        {
            return value.second;
        }
    }

    throw std::invalid_argument("Не удалось найти значение \"" + name + "\".");
}

TestEnvironment *AbstractTestAction::environment() const
{
    if (m_manualTest == nullptr)
    {
        Error("Can't get environment for test action, because there is no test.");
        return nullptr;
    }

    return m_manualTest->environment();
}

std::string AbstractTestAction::getFullName() const
{
    return name();
}

void AbstractTestAction::setManualTest(ManualTest *test)
{
    m_manualTest = test;

    auto childrenVector = children();
    for (auto &child : childrenVector)
    {
        applyTest(child, test);
    }
}

std::vector<std::pair<std::string, DataValue>> AbstractTestAction::getAllVariables() const
{
    return m_dynamicValues;
}

void AbstractTestAction::populateTreeWidgetItem(QTreeWidgetItem *item) const
{
    item->setText(0, QString::fromStdString(getFullName()));

    if (m_allowChildren)
    {
        item->setFlags(item->flags() | Qt::ItemIsDropEnabled);
    }

    auto childVector = children();
    for (auto& child : childVector )
    {
        auto treeWidgetItem = new QActionTreeWidgetItem(item, child);

        treeWidgetItem->setFlags(treeWidgetItem->flags() ^ Qt::ItemIsDropEnabled);

        child->populateTreeWidgetItem(treeWidgetItem);
    }
}

TestActionPtr AbstractTestAction::copy() const
{
    auto action = createAction();

    action->m_manualTest = m_manualTest;
    action->m_dynamicValues = m_dynamicValues;
    action->m_name = m_name;
    action->m_description = m_description;

    return action;
}

std::vector<TestActionPtr> AbstractTestAction::children() const
{
    return m_childrenActions;
}

void AbstractTestAction::addChild(TestActionPtr action)
{
    applyTest(action, m_manualTest);

    m_childrenActions.push_back(action);
}

void AbstractTestAction::insertChild(TestActionPtr action, uint32_t index)
{
    applyTest(action, m_manualTest);

    m_childrenActions.insert(
            m_childrenActions.begin() + index,
            action
    );
}

void AbstractTestAction::removeChild(uint32_t index)
{
    applyTest(m_childrenActions[index], nullptr);

    m_childrenActions.erase(
            m_childrenActions.begin() + index
    );
}

void AbstractTestAction::clearChildren()
{
    m_childrenActions.clear();
}

bool AbstractTestAction::allowChildren() const
{
    return m_allowChildren;
}

void AbstractTestAction::setAllowChildren(bool allow)
{
    m_allowChildren = allow;
}

void AbstractTestAction::applyTest(TestActionPtr action, ManualTest *test)
{
    action->setManualTest(test);
    auto children = action->children();
    for (auto &child : children)
    {
        applyTest(child, test);
    }
}
