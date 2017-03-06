//
// Created by megaxela on 20.02.17.
//

#include <netinet/in.h>
#include <unordered_map>
#include <QtCore/QMetaObject>
#include <QtWidgets/QTreeWidget>
#include <QtCore/QObject>
#include <QtGui/QtGui>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeView>
#include <include/Tools/Logger.h>
#include <include/Windows/Widgets/QTestActionsTreeWidget.h>
#include <include/Windows/Widgets/QActionTreeWidgetItem.h>
#include "include/Windows/Widgets/QTestDisplayTreeWidget.h"

QTestDisplayTreeWidget::QTestDisplayTreeWidget(QWidget *parent) :
        QTreeWidget(parent),
        m_test()
{
    connect(this,
            &QTestDisplayTreeWidget::currentItemChanged,
            this,
            &QTestDisplayTreeWidget::onElementSelected);



    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

QTestDisplayTreeWidget::~QTestDisplayTreeWidget()
{

}

void QTestDisplayTreeWidget::setManualTest(ManualTestPtr test)
{
    m_test = test;
    displayActions();
}

ManualTestPtr QTestDisplayTreeWidget::manualTest() const
{
    return m_test;
}

void QTestDisplayTreeWidget::dropEvent(QDropEvent *event)
{
    if (event->source() == this)
    {
        QTreeWidget::dropEvent(event);
        return;
    }

    auto item = static_cast<QActionTreeWidgetItem*>(
            static_cast<QTestActionsTreeWidget*>(
                    event->source()
            )->currentItem()
    );

    m_test->addTestAction(item->getAction()->copy());

    auto testAction = new QActionTreeWidgetItem(currentItem(), item->getAction()->copy());

    item->getAction()->populateTreeWidgetItem(testAction);

    if (!currentItem())
    {
        addTopLevelItem(testAction);
    }
}

void QTestDisplayTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // Внутренее перемещение
    if (event->source() == this)
    {
        event->accept();
        return;
    }

    // Добавление нового элемента
    auto testActionsTreeWidget = dynamic_cast<QTestActionsTreeWidget*>(event->source());
    if (!testActionsTreeWidget)
    {
        QAbstractItemView::dragEnterEvent(event);
        return;
    }

    if (m_test == nullptr)
    {
        event->ignore();
        return;
    }

    if (event->mimeData()->text() == "testing/action")
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void QTestDisplayTreeWidget::setSearchFilter(QString string)
{
    if (string.isEmpty())
    {
        m_searchFilter = QRegExp();
    }
    else
    {
        m_searchFilter = QRegExp(string);
    }
    displayActions();
}

void QTestDisplayTreeWidget::displayActions()
{
    clear();

    if (!m_test)
    {
        return;
    }

    for (uint32_t actionIndex = 0;
         actionIndex < m_test->numberOfActions();
         ++actionIndex)
    {
        // Получаем действие
        auto testAction = m_test->getTestAction(actionIndex);

        // Создаем объект
        auto item = new QActionTreeWidgetItem(this, testAction);

        //
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

        // Изменяем tree widget если требуется
        testAction->populateTreeWidgetItem(item);
    }
}

void QTestDisplayTreeWidget::onElementSelected(QTreeWidgetItem *item)
{
    if (item && item->type() == QActionTreeWidgetItem::ActionType)
    {
        emit onActionSelected(
                static_cast<QActionTreeWidgetItem*>(
                        item
                )->getAction()
        );
    }
    else
    {
        emit onActionSelected(nullptr);
    }
}

void QTestDisplayTreeWidget::applyChanges()
{
    if (m_test == nullptr)
    {
        return;
    }

    m_test->clear();

    for (uint32_t i = 0; i < topLevelItemCount(); ++i)
    {
        applyItem(static_cast<QActionTreeWidgetItem*>(topLevelItem(i)));
    }
}

void QTestDisplayTreeWidget::applyItem(QActionTreeWidgetItem *item, bool root)
{
    if (!item)
    {
        return;
    }

    if (root)
    {
        m_test->addTestAction(item->getAction());
    }

    if (item->getAction()->allowChildren())
    {
        item->getAction()->clearChildren();
        for (uint32_t childIndex = 0;
             childIndex < item->childCount();
             ++childIndex)
        {
            auto action = static_cast<QActionTreeWidgetItem*>(item->child(childIndex))->getAction();

            applyItem(static_cast<QActionTreeWidgetItem*>(item->child(childIndex)), false);

            item->getAction()->addChild(
                    action
            );
        }
    }
}

void QTestDisplayTreeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        if (currentItem() != nullptr)
        {
            delete currentItem();
        }
    }


    QTreeView::keyPressEvent(event);
}

void QTestDisplayTreeWidget::onActionChanged(TestActionPtr action)
{
    for (uint32_t i = 0; i < topLevelItemCount(); ++i)
    {
        auto item = static_cast<QActionTreeWidgetItem*>(topLevelItem(i));

        if (item->getAction() == action)
        {
            item->setText(0, QString::fromStdString(action->getFullName()));
        }
    }
}
