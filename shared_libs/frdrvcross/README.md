0x00 ФН Успешное выполнение команды
0x01 ФН Неизвестная команда, неверный форматпосылки или неизвестные параметры.
0x02 ФН Неверное состояние ФН
0x03 ФН Ошибка ФН
0x04 ФН Ошибка КС
0x05 ФН Закончен срок эксплуатации ФН
0x06 ФН Архив ФН переполнен
0x07 ФН Неверные дата и/или время
0x08 ФН Нет запрошенных данных
0x09 ФН Некорректное значение параметров команды
0x10 ФН Превышение размеров TLV данных
0x11 ФН Нет транспортного соединения
0x12 ФН Исчерпан ресурс КС
0x14 ФН Исчерпан ресурс хранения
0x15 ФН Исчерпан ресурс Ожидания передачи сообщения
0x16 ФН Продолжительность смены более 24 часов
0x17 ФН Неверная разница во времени между 2 операциями.
0x20 ФН Сообщение от ОФД не может быть принято
0x26 ККТ Вносимая клиентом сумма меньше суммы чека
0x2B ККТ Невозможно отменить предыдущую команду
0x2C ККТ Обнулённая касса (повторное гашение невозможно)
0x2D ККТ Сумма чека по секции меньше суммы сторно
0x2E ККТ В ККТ нет денег для выплаты
0x30 ККТ ККТ заблокирован, ждет ввода пароля налогового инспектора
0x32 ККТ Требуется выполнение общего гашения
0x33 ККТ Некорректные параметры в команде
0x34 ККТ Нет данных
0x35 ККТ Некорректный параметр при данных настройках
0x36 ККТ Некорректные параметры в команде для данной реализации ККТ
0x37 ККТ Команда не поддерживается в данной реализации ККТ
0x38 ККТ Ошибка в ПЗУ
0x39 ККТ Внутренняя ошибка ПО ККТ
0x3A ККТ Переполнение накопления по надбавкам в смене
0x3B ККТ Переполнение накопления в смене
0x3C ККТ Смена открыта – операция невозможна
0x3D ККТ Смена не открыта – операция невозможна
0x3E ККТ Переполнение накопления по секциям всмене
0x3F ККТ Переполнение накопления по скидкам всмене
0x40 ККТ Переполнение диапазона скидок
0x41 ККТ Переполнение диапазона оплаты наличными
0x42 ККТ Переполнение диапазона оплаты типом 2
0x43 ККТ Переполнение диапазона оплаты типом 3
0x44 ККТ Переполнение диапазона оплаты типом 4
0x45 ККТ Cумма всех типов оплаты меньше итога чека
0x46 ККТ Не хватает наличности в кассе
0x47 ККТ Переполнение накопления по налогам в смене
0x48 ККТ Переполнение итога чека
0x49 ККТ Операция невозможна в открытом чеке данного типа
0x4A ККТ Открыт чек – операция невозможна
0x4B ККТ Буфер чека переполнен
0x4C ККТ Переполнение накопления по обороту налогов в смене
0x4D ККТ Вносимая безналичной оплатой сумма больше суммы чека
0x4E ККТ Смена превысила 24 часа
0x4F ККТ Неверный пароль
0x50 ККТ Идет печать результатов выполнения предыдущей команды
0x51 ККТ Переполнение накоплений наличными в смене
0x52 ККТ Переполнение накоплений по типу оплаты 2 в смене
0x53 ККТ Переполнение накоплений по типу оплаты 3 в смене
0x54 ККТ Переполнение накоплений по типу оплаты 4 в смене
0x55 ККТ Чек закрыт – операция невозможна
0x56 ККТ Нет документа для повтора
0x58 ККТ Ожидание команды продолжения печати
0x59 ККТ Документ открыт другим кассиром
0x5A ККТ Скидка превышает накопления в чеке
0x5B ККТ Переполнение диапазона надбавок
0x5C ККТ Понижено напряжение 24В
0x5D ККТ Таблица не определена
0x5E ККТ Неверная операция
0x5F ККТ Отрицательный итог чека
0x60 ККТ Переполнение при умножении
0x61 ККТ Переполнение диапазона цены
0x62 ККТ Переполнение диапазона количества
0x63 ККТ Переполнение диапазона отдела
0x65 ККТ Не хватает денег в секции
0x66 ККТ Переполнение денег в секции
0x68 ККТ Не хватает денег по обороту налогов
0x69 ККТ Переполнение денег по обороту налогов
0x6A ККТ Ошибка питания в момент ответа по I2C
0x6B ККТ Нет чековой ленты
0x6C ККТ Нет операционного журнала
0x6D ККТ Не хватает денег по налогу
0x6E ККТ Переполнение денег по налогу
0x6F ККТ Переполнение по выплате в смене
0x71 ККТ Ошибка отрезчика
0x72 ККТ Команда не поддерживается в данном подрежиме
0x73 ККТ Команда не поддерживается в данном режиме
0x74 ККТ Ошибка ОЗУ
0x75 ККТ Ошибка питания
0x76 ККТ Ошибка принтера: нет импульсов с тахогенератора
0x77 ККТ Ошибка принтера: нет сигнала с датчиков
0x78 ККТ Замена ПО
0x7A ККТ Поле не редактируется
0x7B ККТ Ошибка оборудования
0x7C ККТ Не совпадает дата
0x7D ККТ Неверный формат даты
0x7E ККТ Неверное значение в поле длины
0x7F ККТ Переполнение диапазона итога чека
0x84 ККТ Переполнение наличности
0x85 ККТ Переполнение по приходу в смене
0x86 ККТ Переполнение по расходу в смене
0x87 ККТ Переполнение по возвратам прихода в смене
0x88 ККТ Переполнение по возвратам расхода в смене
0x89 ККТ Переполнение по внесению в смене
0x8A ККТ Переполнение по надбавкам в чеке
0x8B ККТ Переполнение по скидкам в чеке
0x8C ККТ Отрицательный итог надбавки в чеке
0x8D ККТ Отрицательный итог скидки в чеке
0x8E ККТ Нулевой итог чека
0x8F ККТ Касса не зарегистрирована
0x90 ККТ Поле превышает размер, установленный в настройках
0x91 ККТ Выход за границу поля печати при данных настройках шрифта
0x92 ККТ Наложение полей
0x93 ККТ Восстановление ОЗУ прошло успешно
0x94 ККТ Исчерпан лимит операций в чеке
0x96 ККТ Выполните отчет о закрытии смены
0x9B ККТ Некорректное действие
0x9C ККТ Товар не найден по коду в базе товаров
0x9D ККТ Неверные данные в записе о товаре в базе товаров
0x9E ККТ Неверный размер файла базы или регистров товаров
0xC0 ККТ Контроль даты и времени (подтвердите дату и время)
0xC2 ККТ Превышение напряжения в блоке питания
0xC4 ККТ Несовпадение номеров смен
0xС5 ККТ Буфер подкладного документа пуст
0xC6 ККТ Подкладной документ отсутствует
0xC7 ККТ Поле не редактируется в данном режиме
0xС8 ККТ Нет связи с принтером или отсутствуют импульсы от таходатчика
0xC9 ККТ Перегрев печатающей головки
0xCA ККТ Температура вне условий эксплуатации
0xCB ККТ Неверный подытог чека
0xCE ККТ Лимит минимального свободного объема
0xОЗ ПЗУ на ККТ исчерпан
0xCF ККТ Неверная дата (Часы сброшены? Установите дату!)
0xD0 ККТ Отчет операционного журнала не распечатан!
0xD1 ККТ Нет данных в буфере
0xD5 ККТ Критическая ошибка при загрузке ERRxx
0xE0 ККТ Ошибка связи с купюроприемником
0xE1 ККТ Купюроприемник занят
0xE2 ККТ Итог чека не соответствует итогу купюроприемника
0xE3 ККТ Ошибка купюроприемника
0xE4 ККТ Итог купюроприемника не нулевой

{1, "Выдача данных."},
{2, "Открытая смена, 24 часа не кончились."},
{3, "Открытая смена, 24 часа кончились."},
{4, "Закрытая смена."},
{5, "Блокировка по неправильному паролю налогового инспектора."},
{6, "Ожидание подтверждения ввода даты."},
{7, "Разрешение изменения положения десятичной точки"},
{8, "Открытый документ"},
{9, "Режим разрешения технологического обнуления."},
{10, "Тестовый прогон."},
{11, "Печать полного фискального отчета."},
{12, "Зарезервировано."},
{13, "Работа с фискальным подкладным документом1"},
{14, "Печать подкладного документа1"},
{15, "Фискальный подкладной документ сформирован1"},