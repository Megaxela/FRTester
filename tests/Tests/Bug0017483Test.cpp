//
// Created by megaxela on 20.03.17.
//

#include "Bug0017483Test.h"

Bug0017483Test::Bug0017483Test() :
    AbstractTest(nullptr,
                 "Тест бага №0017483",
                 "Если в таблице №18 \"Fiscal storage\" установить значение поля №7 \"User\" длиной в 128 символов - то при значении поля №12 \"Rus печать реквезитов пользователя\" (таблица №17 \"Региональные настройки\") равным 1 будет печататься в одну строку поле №7 и поле №8 из таблицы №18.",
                 {{"Password", (uint32_t) 30}})
{

}

bool Bug0017483Test::execute()
{
    auto password = getValue("Password").toUInt32();

    // Сохраняем значение "Печать реквезитов пользователя"
    environment()->driver()->readTableBin(
            password,
            17,
            1,
            0
    );

    return true;
}
