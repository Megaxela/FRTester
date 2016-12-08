//
// Created by megaxela on 11.11.16.
//

#ifndef FRDRVCROSS_FRDRIVER_H
#define FRDRVCROSS_FRDRIVER_H


#include <mutex>
#include <memory>
#include "Abstract/Protocol.h"
#include "Abstract/PhysicalInterface.h"

/**
 * @brief Класс, описывающий драйвер для работы с
 * Фискальным Регистратором напрямую через указанный
 * интерфейс с использованием указанного протокола.
 * Если команда возвращает некие данные (не успешность
 * своего выполнения) - тогда данные считаются валидными
 * только в случае, если последняя ошибка == 0x00.
 */
class FRDriver
{
public:
    /**
     * @brief Коды ошибок. Для получения строкового представления
     * ошибок требуется воспользоваться FRDriver::Converters::errorToString
     */
    enum class ErrorCode
    {
          NoError = 0x00
        , FiscalAccumulatorUnknownCommand = 0x01
        , FiscalAccumulatorWrongFiscalAccumulatorState = 0x02
        , FiscalAccumulatorError = 0x03
        , FiscalAccumulatorCryptographCoprocessorError = 0x04
        , FiscalAccumulatorLifetimeExpired = 0x05
        , FiscalAccumulatorArchiveIsFull = 0x06
        , FiscalAccumulatorWrongDateAndTime = 0x07
        , FiscalAccumulatorNoRequestedData = 0x08
        , FiscalAccumulatorWrongArgumentsInCommand = 0x09
        , FiscalAccumulatorTLVDataExcessSize = 0x10
        , FiscalAccumulatorNoTransportConnection = 0x11
        , FiscalAccumulatorCryptographCoprocessorResourceExcess = 0x12
        , FiscalAccumulatorHolderResourceExcess = 0x14
        , FiscalAccumulatorWaitingForMessage = 0x15
        , FiscalAccumulatorDurationOfChangeIsMoreThan24Hours = 0x16
        , FiscalAccumulatorWrongTimeDifferenceBetweenOperations = 0x17
        , FiscalAccumulatorMessageFromFiscalDataOperatorCantBeRecieved = 0x20
        , PointOfSalesClientSummIsLowerThanCheckSumm = 0x26
        , PointOfSalesCantCancelLastCommand = 0x2B
        , PointOfSalesNilledCashbox = 0x2C
        , PointOfSalesCheckSummIsLowerThanReversal = 0x2D
        , PointOfSalesHasNoCashToPay = 0x2E
        , PointOfSalesIsLockedWaitingForTaxInspector = 0x30
        , PointOfSalesTotalExtintionIsRequired = 0x32
        , PointOfSalesWrongArgumnetsInCommand = 0x33
        , PointOfSalesNoData = 0x34
        , PointOfSalesWrongArgumentOnSetup = 0x35
        , PointOfSalesWrongArgumentInCommandForThisPointOfSalesImplementation = 0x36
        , PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation = 0x37
        , PointOfSalesROMError = 0x38
        , PointOfSalesInternalSoftwareError = 0x39
        , PointOfSalesAllowanceStorageOverflowForChange  = 0x3A
        , PointOfSalesStorageOverflowForChange = 0x3B
        , PointOfSalesCommandCantBeExecutedChangeIsOpened = 0x3C
        , PointOfSalesCommandCantBeExecutedChangeIsClosed = 0x3D
        , PointOfSalesAllowanceStorageBySectionForChange = 0x3E
        , PointOfSalesAllowanceStorageByDiscountForChange = 0x3F
        , PointOfSaleRangeOfDiscountsOverflow = 0x40
        , PointOfSaleRangeOfCashPayOverflow = 0x41
        , PointOfSaleRangeOfEx2PayOverfloat = 0x42
        , PointOfSaleRangeOfEx3PayOverfloat = 0x43
        , PointOfSaleRangeOfEx4PayOverfloat = 0x44
        , PointOfSalePaySummLesserThanResultOfCheck = 0x45
        , PointOfSaleNotEnoughCashInPointOfSales = 0x46
        , PointOfSaleAllowanceStorageByTaxForChange = 0x47
        , PointOfSaleResultOfCheckOverflow = 0x48
        , PointOfSaleCommandCantBeExecutedInOpenedCheckOfThisType = 0x49
        , PointOfSaleCantBeExecutedCheckIsOpened = 0x4A
        , PointOfSaleCheckBufferOverflow = 0x4B
        , PointOfSaleAccumulatorOverflowOfTaxTurnoverOfChange = 0x4C
        , PointOfSaleInsertionOfCashlessPaymentSumIsGreaterThanSumOfCheck = 0x4D
        , ShiftIsMoreThan24Hour = 0x4E
        , WrongPassword = 0x4F
        , PreviousCommandPrintingIsActive = 0x50
        , CashAccumulationOverflowOfChange = 0x51
        , PayType1AccumulationOverflowOfChange = 0x52
        , PayType2AccumulationOverflowOfChange = 0x53
        , PayType3AccumulationOverflowOfChange = 0x54
        , PayType4AccumulationOverflowOfChange = 0x55
        , CheckIsClosed = 0x56
        , ThereIsNoDocumentToRepeat = 0x57
        , WaitingForCommandToContinuePrinting = 0x58

        /* ... todo: Добавить остальные коды ошибок */
        , Unknown
    };

    class Converters
    {
    public:
        /**
         * @brief Функция для преобразования кода режима в строку.
         * @param mode Режим
         * @return Строковое представление режима.
         */
        static std::string posModeToString(uint8_t mode);

        /**
         * @brief Функция для преобразования кода ошибки в строку.
         * @param error Код ошибки.
         * @return Строковое представление ошибки.
         */
        static std::string errorToString(int error);

        /**
         * @brief Функция для преобразования подрежима в строку.
         * @param subMode Подрежим.
         * @return Строковое представление подрежима.
         */
        static std::string posSubModeToString(uint8_t subMode);

        /**
         * @brief Функция для преобразования результата последней
         * печати в строку.
         * @param result Результат.
         * @return Строковое представление результата.
         */
        static std::string lastPrintResultToString(uint8_t result);

    private:
        Converters() = delete;
        ~Converters() = delete;
    };

    /**
     * @brief Результат короткого запроса состояния.
     */
    struct ShortState
    {
        uint16_t posFlags;                      //< Флаги ККТ
        uint8_t  posMode;                       //< Режим ККТ
        uint8_t  posSubMode;                    //< Подрежим ККТ
        uint8_t  checkNumberOfActions;          //< Количество операций в чеке (младший байт)
        uint8_t  batteryVoltage;                //< Напряжение на резервной батареи
        uint8_t  powerSupplyVoltage;            //< Напряжение источника питания
        uint8_t  checkNumberOfActionsHigher;    //< Количество операций в чеке (старний байт)
        uint8_t  lastPrintResult;               //< Результат последней печати
    };

    /**
     * @brief Результат полного запроса состояния.
     */
    struct FullState
    {
        char firmwareVersion[2];                   //< Версия ПО ККТ
        uint16_t firmwareBuild;                     //< Сборка ПО ККТ
        uint8_t firmwareDate[3];                    //< Дата ПО ККТ [ДД-ММ-ГГ]
        uint8_t numberInHall;                       //< Номер в зале
        uint16_t currentDocumentPassthrougNumber;   //< Сквозной номер текущего документа
        uint16_t posFlags;                          //< Флаги ККТ
        uint8_t posMode;                            //< Режим ККТ
        uint8_t posSubMode;                         //< Подрежим ККТ
        uint8_t posPort;                            //< Порт ККТ
        uint8_t date[3];                            //< Дата [ДД-ММ-ГГ]
        uint8_t time[3];                            //< Время [ЧЧ-ММ-СС]
        uint32_t factoryNumberLower;                //< Младшее слово заводского номера
        uint16_t lastClosedShiftNumber;             //< Номер последней закрытой смены
        uint8_t numberOfReRegistration;             //< Количество перерегистраций
        uint8_t inn[6];                             //< ИНН
        uint16_t factoryNumberUpper;                //< Старшее слово заводского номера
    };

    /**
     * @brief Результат запроса параметров обмена.
     */
    struct ExchangeConfiguration
    {
        union{
            uint8_t baudRateCode;  //< Код скорости обмена
            uint8_t tcpPortNumber; //< Номер TCP порта
        };

        uint8_t byteTimeout;       //< Таймаут приема байта
    };

    /**
     * @brief Результат запроса параметров шрифта.
     */
    struct FontConfiguration
    {
        uint16_t printAreaWidthPixels;      //< Ширина области печати в точках
        uint8_t symbolWidthWithInterval;    //< Ширина символа с учетом межсимвольного интервала в точках
        uint8_t symbolHeightWithInterval;   //< Высота символа с учетом межстрочного интервала в точках
        uint8_t numberOfFonts;              //< Количество шрифтов в ККТ
    };

    /**
     * @brief Результат запроса структуры таблицы.
     */
    struct TableStructure
    {
        std::string name;       //< Название таблицы
        uint16_t numberOfRows;  //< Количество рядов
        uint8_t numberOfCols;   //< Количество полей
    };

    /**
     * @brief Результат запроса структуры поля.
     */
    struct FieldStructure
    {
        std::string name;
        uint8_t fieldType;
        uint8_t numberOfBytes;
        std::string maxValue;
        std::string minValue;
    };

    /**
     * @brief Результат закрытия чека
     */
    struct CheckResult
    {
        uint64_t change; //< Сдача
        std::string url; //< Веб ссылка
    };

    /**
     * @brief Конструктор
     */
    FRDriver();

    /**
     * @brief Деструктор.
     */
    virtual ~FRDriver();

    /**
     * @brief Отправка чистых наборов байт. Эти байты будут обернуты
     * в протокол и отправлены по интерфейсу.
     * Результат не обрабатывается драйвером и последняя ошибка,
     * получаемая через getLastError не устанавливается.
     * @param data Передаваемые данные.
     * @return Не очищенный от служебных данных ответ от ФР.
     */
    ByteArray sendRaw(const ByteArray &data);

    /**
     * @brief Сеттер для интерфейса драйвера.
     * @param inter Указатель на интерфейс.
     */
    void setInterface(InterfacePtr inter);

    /**
     * @brief Геттер для интерфейса драйвера.
     * @return Указатель на текущий интерфейс.
     */
    InterfacePtr physicalInterface() const;

    /**
     * @brief Сеттер для протокола драйвера.
     * @param protocol Указатель на протокол.
     */
    void setProtocol(ProtocolPtr protocol);

    /**
     * @brief Геттер для текущего протокола драйвера.
     * @return Указатель на протокол.
     */
    ProtocolPtr protocol() const;

    /**
     * @brief Метод для получения ошибки выполнения последней
     * команды.
     * @return Код ошибки.
     */
    ErrorCode getLastError() const;

    /**
     * @brief Метод для получения последнего полученного
     * порядкового номера кассира.
     * @return Порядковый номер кассира.
     */
    uint8_t getLastReceivedCashierNumber() const;

    /**
     * @brief Сигнал.
     * @param password Пароль кассира.
     * @return Успех выполнения.
     */
    bool beep(uint32_t password);

    /**
     * @brief Открыть смену.
     * @param password Пароль кассира.
     */
    bool openShift(uint32_t password);

    /**
     * @brief Отчет от закрытии смены.
     * @param password Пароль администратора или системного администратора.
     * @return Успешность выполнения команды.
     */
    bool shiftCloseReport(uint32_t password);

    /**
     * @brief Продажа (приход)
     * @param password Пароль кассира.
     * @param count Количество (будет обрезано до 5 байт)
     * @param price Цена (будет обрезана до 5 байт)
     * @param department Номер отдела. 0..16 режим свободной продажи.
     * 255 - режим продажи по коду товара.
     * @param firstTax Первая налоговая группа.
     * @param secondTax Вторая налоговая группа.
     * @param thirdTax Третья налоговая группа.
     * @param fourthTax Четвертая налоговая группа.
     * @param good Название товара.
     * @return Удалось ли совершить продажу.
     */
    bool sell(uint32_t password,
              uint64_t count,
              uint64_t price,
              uint8_t department,
              uint8_t firstTax,
              uint8_t secondTax,
              uint8_t thirdTax,
              uint8_t fourthTax,
              const std::string &good);

    /**
     * @brief Короткий запрос состояния.
     * @param password Пароль кассира.
     * @return Результат.
     */
    ShortState shortStateRequest(uint32_t password);

    /**
     * @brief Полный запрос состояния.
     * @param password Пароль кассира.
     * @return Результат.
     */
    FullState fullStateRequest(uint32_t password);

    /**
     * @brief Метод для проверки наличия соединения.
     * @return Есть ли соединение.
     */
    bool checkConnection();

    /**
     * @brief Метод для закрытия чека.
     * @param password Пароль кассира.
     * @param cashPaySum Сумма, оплаченная наличными.
     * @param type2PaySum Сумма, оплаченная безналом второго типа.
     * @param type3PaySum Сумма, оплаченная безналом третьего типа.
     * @param type4PaySum Сумма, оплаченная безналом четвертного типа.
     * @param discount Скидка (>0) или надбавка (<0)
     * @param tax1 Первый налог.
     * @param tax2 Второй налог.
     * @param tax3 Третий налог.
     * @param tax4 Четвертый налог.
     * @param textToPrint Текст для печати на чеке.
     * @return Результат закрытия чека. URL и сдача.
     */
    CheckResult closeCheck(uint32_t password,
                           uint64_t cashPaySum,
                           uint64_t type2PaySum,
                           uint64_t type3PaySum,
                           uint64_t type4PaySum,
                           uint16_t discount,
                           uint8_t tax1,
                           uint8_t tax2,
                           uint8_t tax3,
                           uint8_t tax4,
                           const std::string &textToPrint);

    /**
     * @brief Метод для продолжения печати.
     * @param password Пароль кассира.
     * @return Результат выполнения команды.
     */
    bool resumePrinting(uint32_t password);

    /**
     * @brief Метод для получения операционного регистра.
     * @param password Пароль кассира.
     * @param registerNumber Номер регистра.
     * @return Содержимое регистра
     */
    uint16_t operatingRegisterRequest(uint32_t password,
                                      uint8_t registerNumber);

    /**
     * @brief Метод для считывания значения
     * @param password Пароль системного администратора
     * @param table Таблица
     * @param row Ряд (строка)
     * @param field Поле (столбец)
     * @return Строковое значение таблицы
     */
    std::string readTable(uint32_t password,
                          uint8_t table,
                          uint16_t row,
                          uint8_t field);

    /**
     * @brief Метод для аннулирования чека.
     * @param password Пароль кассира.
     * @return Успешность аннулирования.
     */
    bool cancelCheck(uint32_t password);

    /**
     * @brief Метод для чтения параметров обмена
     * @param sysAdmPassword Пароль системного администратора.
     * @param portNumber Номер порта.
     * @return Структура с параметрами обмена
     */
    FRDriver::ExchangeConfiguration readExchangeConfiguration(uint32_t sysAdmPassword,
                                                              uint8_t portNumber);

    /**
     * @brief Метод для произведения технологического обнуления.
     * @return Успешность выполнения команды.
     */
    bool technologicalReset();

    /**
     * @brief Метод для печати стандартной строки.
     * @param password Пароль кассира.
     * @param flags Флаги.<br>
     * 0 бит - операционный журнал.<br>
     * 1 бит - чековая лента.<br>
     * 2 бит - подкладной документ.<br>
     * 3 бит - слип-чек.<br>
     * 4 бит - ничего.
     * 5 бит - ничего.
     * 6 бит - перенос строк.<br>
     * 7 бит - отложенная печать.
     * @param print Строка, длиной от 0 до 200 байт.
     * @return Успешность выполнения команды.
     */
    bool standardStringPrint(uint32_t password,
                             uint8_t flags,
                             const std::string &print);

    /**
     * @brief Метод для печати заголовка документа.
     * @param password Пароль кассира.
     * @param document Наименование документа.
     * @param documentNumber Номер документа.
     * @return Сквозной номер документа.
     */
    uint16_t documentHeaderPrint(uint32_t password,
                                 const std::string &document,
                                 uint16_t documentNumber);

    /**
     * @brief Метод для запроса денежного Ф регистра.
     * @param password Пароль кассира.
     * @param registerNumber Номер регистра.
     * @return 6 байт с содержимым регистра.
     */
    uint64_t currencyRegisterRequest(uint32_t password, uint8_t registerNumber);

    /**
     * @brief Метод для записи таблицы.
     * @param sysPassword Пароль системного администратора.
     * @param tableNumber Таблица
     * @param row Ряд
     * @param field Поле
     * @param value Значение до 40 или до 246 байт
     * @return Успешность выполнения команды.
     */
    bool writeTable(uint32_t sysPassword,
                    uint8_t tableNumber,
                    uint16_t row,
                    uint8_t field,
                    const std::string &value);

    /**
     * @brief Метод для программирования времени.
     * @param sysPassword Пароль системного администратора.
     * @param h Часы.
     * @param m Минуты.
     * @param s Секунды.
     * @return Успешность выполнения команды.
     */
    bool timeProgramming(uint32_t sysPassword,
                         uint8_t h,
                         uint8_t m,
                         uint8_t s);

    /**
     * @brief Метод для программироавния даты.
     * @param sysPassword Пароль системного администратора.
     * @param day День.
     * @param month Месяц.
     * @param year Год.
     * @return Успешность выполнения команды.
     */
    bool dateProgramming(uint32_t sysPassword,
                         uint8_t day,
                         uint8_t month,
                         uint8_t year);

    /**
     * @brief Метод для подтверждения даты.
     * @param sysPassword Пароль системного администратора.
     * @param day День.
     * @param month Месяц.
     * @param year Год.
     * @return Успешность выполнения команды.
     */
    bool dateConfirm(uint32_t sysPassword,
                     uint8_t day,
                     uint8_t month,
                     uint8_t year);

    /**
     * @brief Метод для инициализации таблиц начальными
     * значениями.
     * @param sysPassword Пароль системного администратора.
     * @return Успешность выполнения команды.
     */
    bool tableValuesInit(uint32_t sysPassword);

    /**
     * @brief Метод для отрезки чека.
     * @param sysPassword Пароль кассира.
     * @param cutType Тип отрезки. <br>
     * 0 - полная.<br>
     * 1 - неполная.<br>
     * @return Успешность выполнения команды.
     */
    bool cutCheck(uint32_t sysPassword, uint8_t cutType);

    /**
     * @brief Метод для прочтения параметров шрифта.
     * @param sysPassword Пароль системного администратора.
     * @param fontNumber Номер шрифта.
     * @return Конфигурация шрифта.
     */
    FontConfiguration readFontConfiguration(uint32_t sysPassword,
                                            uint8_t fontNumber);

    /**
     * @brief Метод для выполнения общего гашения.
     * @param sysPassword Пароль системного администратора.
     * @return Успешность выполнения команды.
     */
    bool totalExtinction(uint32_t sysPassword);

    /**
     * @brief Метод для выполнения протяжки (прокрутки)
     * @param password Пароль кассира.
     * @param flags Флаги.<br>
     * 0 бит - операционный журнал.<br>
     * 1 бит - чековая лента.<br>
     * 2 бит - прикладной документ.
     * @param numberOfLines
     * @return Успешность выполнения команды.
     */
    bool scrolling(uint32_t password,
                   uint8_t flags,
                   uint8_t numberOfLines);

    /**
     * @brief Метод для запрашивания структуры таблицы.
     * @param sysPassword Проль системного администратора.
     * @param tableNumber Номер таблицы.
     * @return Структура таблицы.
     */
    TableStructure tableStructureRequest(uint32_t sysPassword,
                                         uint8_t tableNumber);

    /**
     * @brief Метод для запрашивания структуры поля.
     * @param sysPassword Пароль системного администратора.
     * @param table Номер таблицы.
     * @param field Номер поля.
     * @return Структура поля.
     */
    FieldStructure fieldStructureRequest(uint32_t sysPassword,
                                         uint8_t table,
                                         uint8_t field);

    /**
     * @brief Метод для печати страки с заданными шрифтом.
     * @param password Пароль кассира.
     * @param flags Флаги.<br>
     * 0 бит - операционный журнал.<br>
     * 1 бит - чековая лента.<br>
     * 2 бит - подкладной документ.<br>
     * 3 бит - слип-чек.<br>
     * 4 бит - ничего.
     * 5 бит - ничего.
     * 6 бит - перенос строк.<br>
     * 7 бит - отложенная печать.
     * @param fontNumber Номер шрифта.
     * @param string Печатаемые символы.
     * @return Успешность выполнения команды.
     */
    bool fontStringPrint(uint32_t password,
                         uint8_t flags,
                         uint8_t fontNumber,
                         const std::string &string);

    /**
     * @brief Метод для суточного отчета без гашения.
     * @param password Пароль администратора, системного
     * администратора или старшего кассира.
     * @return Успешность выполнения команды.
     */
    bool shiftReportWithoutExtinction(uint32_t password);

    /**
     * @brief Метод для отчета по секциям.
     * @param password Пароль администратора, системного
     * администратора или старшего кассира.
     * @return Успешность выполнения команды.
     */
    bool sectionsReport(uint32_t password);

    /**
     * @brief Метод для отчета по налогами.
     * @param password Пароль администратора, системного
     * администратора или старшего кассира.
     * @return Успешность выполнения команды.
     */
    bool taxesReport(uint32_t password);

    /**
     * @brief Метод для отчета по кассирам.
     * @param password Пароль администратора, системного
     * администратора или старшего кассира.
     * @return Успешность выполнения команды.
     */
    bool cashierReport(uint32_t password);

    /**
     * @brief Метод для внесения.
     * @param password Пароль кассира.
     * @param sum Сумма (5 байт).
     * @return Сквозной номер документа.
     */
    uint16_t payin(uint32_t password, uint64_t sum);

    /**
     * @brief Метод для выплаты.
     * @param password Пароль кассира.
     * @param sum Сумма (5 байт).
     * @return Сквозной номер документа.
     */
    uint16_t payout(uint32_t password, uint64_t sum);

    /**
     * @brief Метод для печати клише.
     * @param password Пароль кассира.
     * @return Успешность выполнения команды.
     */
    bool printCliches(uint32_t password);

    /**
     * @brief Метод для печати конца документа.
     * @param password Пароль кассира.
     * @param printAds Печатать ли рекламный текст. (0 - нет, 1 - да).
     * @return Успешность выполнения операции.
     */
    bool printDocumentEnd(uint32_t password, uint8_t printAds);

    /**
     * @brief Метод для печати рекламного текста.
     * @param password Пароль кассира.
     * @return Успешность выполнения операции.
     */
    bool printAds(uint32_t password);

    /**
     * @brief Метод для ввода заводского номера с любым паролем.
     * @param password Пароль. Предполагается использование пароля 0.
     * @param factoryNumber Заводской номер.
     * @return Успешность выполнения операции.
     */
    bool enterFactoryNumber(uint32_t password, uint32_t factoryNumber);

    /**
     * @brief Метод для ввода заводского номера с паролем 0.
     * @param factoryNumber Заводской номер.
     * @return Успешность выполнения операции.
     */
    bool enterFactoryNumber(uint32_t factoryNumber);


protected:
    /**
     * @brief Перечисляемый тип с кодами команд.
     */
    enum class Command
    {
        ShortStateRequest = 0x10          //< Короткий запрос состояния
        , FullStateRequest = 0x11           //< Полный запрос состояния
        , Beep = 0x13                       //< Сигнал
        , ReadExchangeConfiguration = 0x15  //< Чтение параметров обмена
        , TechReset = 0x16                  //< Технологическое обнуление
        , StandardStringPrint = 0x17        //< Печать стандартной строки
        , DocumentHeaderPrint = 0x18        //< Печать заголовка документа
        , CurrencyRegisterRequest = 0x1A    //< Запрос денежного регистра
        , OperatingRegisterRequest = 0x1B   //< Запрос операционного регистра
        , WriteTable = 0x1E                 //< Запись таблицы
        , ReadTable = 0x1F                  //< Чтение таблицы
        , TimeProgramming = 0x21            //< Программирование времени
        , DateProgramming = 0x22            //< Программирование даты
        , DateConfirm = 0x23                //< Подтверждение программирования даты
        , TableValuesInit = 0x24            //< Инициализация таблиц начальными значениями
        , CutCheck = 0x25                   //< Отрезка чека
        , ReadFontConfiguration = 0x26      //< Чтение параметров шрифта
        , TotalExtinction = 0x27            //< Полное гашение
        , Scrolling = 0x28                  //< Протяжка
        , TableStructureRequest = 0x2D      //< Запрос структуры таблицы
        , ShiftReportNoExtinction = 0x40    //< Суточный отчет без гашения
        , ShiftCloseReport = 0x41           //< Отчет о закрытии смены
        , Sell = 0x80                       //< Продажа
        , CloseCheck = 0x85                 //< Обычное закрытие чека
        , CancelCheck = 0x88                //< Аннулирование чека
        , ContinuePrint = 0xB0              //< Продолжить печать
        , OpenShift = 0xE0                  //< Открытие смены
        , FieldStructureRequest = 0x2E      //< Запрос структуры поля
        , FontStringPrint = 0x40            //< Печать строки данным шрифтом
        , SectionsReport = 0x42             //< Отчет по секциям
        , TaxesReport = 0x43                //< Отчет по налогам
        , CashierReport = 0x44              //< Отчет по кассирам
        , PayIn = 0x50                      //< Внесение
        , PayOut = 0x51                     //< Выплата
        , PrintCliches = 0x52               //< Печать клише
        , DocumentEndPrint = 0x53           //< Конец Документа (печать)
        , PrintAds = 0x54                   //< Печать рекламного текста
        , EnterFactoryNumber = 0x55         //< Ввод заводского номера
    };

    /**
     * @brief Метод для отправки команд на ФР.
     * @param c Код команды.
     * @param arguments Аргументы для команды в виде набора байт.
     * @return Очищенный от служебных данных ответ от ФР.
     */
    virtual ByteArray sendCommand(const Command& c, const ByteArray& arguments=ByteArray());

    /**
     * @brief Метод для обработки ответа от ФР.
     * @param data Чистый ответ на команду.
     */
    void proceedResponse(const ByteArray &data, bool cashier=false);

private:
    /**
     * @brief Удаленный конструктор копирования.
     */
    FRDriver(const FRDriver&) = delete;

    /**
     * @brief Удаленный оператор присваивания.
     */
    FRDriver& operator=(const FRDriver&) = delete;

    /**
     * @brief Метод для проверки наличия адаптеров для работы.
     * @return Готовность к работе.
     */
    bool isAdaptorsReady() const;

    /**
     * Мьютекс для предотвращения одновременной отправки.
     */
    std::mutex m_sendMutex;

    /**
     * Протокол для связи с ФР.
     */
    ProtocolPtr m_protocol;

    /**
     * Интерфейс соединения с ФР.
     */
    InterfacePtr m_interface;

    /**
     * Промежуток между командами.
     */
    int m_commandsDelay;

    /**
     * Последний код ошибки.
     */
    ErrorCode m_lastErrorCode;

    /**
     * Последний полученный номер кассира.
     */
    uint8_t m_lastReceivedCahsierNumber;
};

/**
 * @brief Исключение драйвера.
 */
struct DriverException : public std::exception
{
    /**
     * @brief Конструктор.
     * @param __arg Ошибка.
     */
    DriverException(const std::string &__arg) :
            m_str(__arg)
    {}

    /**
     * @brief Виртуальный метод для получения ошибки.
     * @return Ошибкаю
     */
    virtual const char* what() const noexcept override
    {
        return m_str.c_str();
    }

    /**
     * Сама ошибка.
     */
    std::string m_str;
};

#endif //FRDRVCROSS_FRDRIVER_H
