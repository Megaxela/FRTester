//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include <QtCore/QByteArray>
#include <include/Tools/Codecs.h>
#include "include/Testing/ManualTests/Actions/DocumentHeaderPrintTestAction.h"

REGISTER_ACTION(DocumentHeaderPrintTestAction);

DocumentHeaderPrintTestAction::DocumentHeaderPrintTestAction() :
    AbstractTestAction("Печать заголовка",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Документ", ""},
                        {"Номер документа", (uint16_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

DocumentHeaderPrintTestAction::~DocumentHeaderPrintTestAction()
{

}

bool DocumentHeaderPrintTestAction::execute()
{
    environment()->driver()->documentHeaderPrint(
            getValue("Пароль").toUInt32(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    QByteArray(
                            getValue("Строка для печати").toString().c_str()
                    )
            ).toStdString(),
            getValue("Номер документа").toUInt16()
    );

    return true;
}

TestActionPtr DocumentHeaderPrintTestAction::createAction() const
{
    return std::make_shared<DocumentHeaderPrintTestAction>();
}
