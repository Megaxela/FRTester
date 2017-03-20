//
// Created by megaxela on 20.03.17.
//

#include "Bug0017515Test.h"

Bug0017515Test::Bug0017515Test() :
    AbstractTest(nullptr,
                 "Тест бага №0017515",
                 "В отчетах за смену с (без) гашением счетчик фискальных документов за смену (ФД ЗА СМЕНУ) уже учтены ФД открытия и закрытия смены, даже если смена не открыта или еще не закрыта. \n"
                 "Например:\n"
                 "Смена закрыта, бью отчет без гашения и вижу, что:\n"
                 "чеков за смену ФД: 0 \n"
                 "ФД за смену: 2",
                 {{"Password", (uint32_t) 30}})
{

}

bool Bug0017515Test::execute()
{
    auto password = getValue("Password").toUInt32();

    environment()->logger()->log("Проверка, учтен ли ФД закрытия и открытия смены.");

    if (!environment()->driver()->closeNonFiscalDocument(password))
    {
        environment()->logger()->log("Не удалось снять отчет без гашения.");
        return false;
    }

    if (!environment()->tools()->messageQuestion(
            "Проверьте, ФД за смену.",
            "0",
            "Не 0"
    ))
    {
        environment()->logger()->log("Должен быть 0. Баг 0017515 повторился.");
        return false;
    }

    return true;
}
