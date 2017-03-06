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
                       {{"Password", (uint32_t) 30},
                        {"Document", std::string("")},
                        {"Document number", (uint16_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

DocumentHeaderPrintTestAction::~DocumentHeaderPrintTestAction()
{

}

bool DocumentHeaderPrintTestAction::execute()
{
    environment()->driver()->documentHeaderPrint(
            getValue("Password").toUInt32(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    QByteArray(
                            getValue("Document").toString().c_str()
                    )
            ).toStdString(),
            getValue("Document number").toUInt16()
    );

    return true;
}

TestActionPtr DocumentHeaderPrintTestAction::createAction() const
{
    return std::make_shared<DocumentHeaderPrintTestAction>();
}
