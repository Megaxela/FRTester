//
// Created by megaxela on 21.02.17.
//

#include "Tools/GUIHelper.h"
#include <QWidget>

void GUIHelper::cleanFormLayout(QFormLayout *formLayout)
{
    QLayoutItem* child;
    while ((child = formLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }
}
