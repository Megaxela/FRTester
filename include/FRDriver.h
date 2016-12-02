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
        static std::string errorToString(ErrorCode error);

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
     * @return Результат. Верет, только если
     * последняя ошибка равна ErrorCode::NoError.
     */
    ShortState shortStateRequest(uint32_t password);

    /**
     * @brief Полный запрос состояния.
     * @param password Пароль кассира.
     * @return Результат. Верен, только если
     * последняя ошибка равна ErrorCode::NoError
     */
    FullState fullStateRequest(uint32_t password);

    /**
     * @brief Метод для проверки наличия соединения.
     * @return
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
     * @return
     * @todo Разобраться, что должен этот метод возвращать.
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


protected:
    /**
     * @brief Перечисляемый тип с кодами команд.
     */
    enum class Command
    {
          Beep = 0x13                       //< Сигнал
        , Sell = 0x80                       //< Продажа
        , ShortStateRequest = 0x10          //< Короткий запрос состояния
        , FullStateRequest = 0x11           //< Полный запрос состояния
        , OpenShift = 0xE0                  //< Открытие смены
        , ShiftCloseReport = 0x41           //< Отчет о закрытии смены
        , ContinuePrint = 0xB0              //< Продолжить печать
        , NullifyCheck = 0x88               //< Аннулирование чека
        , CloseCheck = 0x85                 //< Обычное закрытие чека
        , OperatingRegisterRequest = 0x1B   //< Запрос операционного регистра
        , CancelCheck = 0x88                //< Аннулирование чека
        , ReadTable = 0x1F                  //< Чтение таблицы
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
