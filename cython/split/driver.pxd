from libcpp.string cimport string

ctypedef unsigned char uint8_t
ctypedef unsigned short uint16_t
ctypedef unsigned int uint32_t
ctypedef unsigned long long uint64_t
ctypedef bint bool

cdef extern from "../include/Abstract/PhysicalInterface.h":
    cdef cppclass PhysicalInterface:
        int type()

cdef extern from "../include/Implementation/COMInterface.h":
    cdef cppclass COMInterface:
        bint openConnection()
        bint closeConnection()
        unsigned int write(const ByteArray& data)
        ByteArray read(const unsigned int &n, unsigned int timeout)
        void setDevice(const string &name)
        string device()
        void setBaudRate(int rate)
        int baudRate()
        void setByteSendTime(long long time);

cdef extern from "../include/Implementation/DefaultProtocol.h":
    cdef cppclass DefaultProtocol:
        ByteArray proceedData(const ByteArray &b)
        ByteArray proceedReceivedData(const ByteArray &raw)
        bint needResponseAfterReceivedData()
        ByteArray responseAfterReceivedData()
        ByteArray receiveDataFromInterface(PhysicalInterface* physicalInterface, unsigned int bytesExpected)
        void prepareDeviceToWrite(PhysicalInterface* physicalInterface)
        bint checkConnection(PhysicalInterface* physicalInterface)

cdef extern from "../include/Tools/ByteArray.h":
    cdef cppclass ByteArray:
        ByteArray()
        void append[T](T number, ByteOrder order)
        unsigned char operator[](unsigned int i);
        int length()

cdef extern from "../include/Tools/ByteArray.h" namespace "ByteArray":
    cdef enum ByteOrder:
        ByteOrder_BigEndian
        ByteOrder_LittleEndian

cdef extern from "../include/FRDriver.h":
    cdef cppclass InterfacePtr:
        PhysicalInterface* get()

cdef extern from "../include/FRDriver.h" namespace "FRDriver":
    cdef cppclass Converters:
        @staticmethod
        string posModeToString(uint8_t mode)

        @staticmethod
        string errorToString(int error)

        @staticmethod
        string posSubModeToString(uint8_t subMode)

        @staticmethod
        string lastPrintResultToString(uint8_t result)

cdef extern from "../include/FRDriver.h":
    cdef cppclass FRDriver:
        FRDriver()

        ByteArray sendRaw(const ByteArray &data)
        void setInterface(PhysicalInterface* interface)
        InterfacePtr physicalInterface()
        ErrorCode getLastError()
        uint8_t getLastReceivedCashierNumber()
        bint beep(uint32_t password)
        bint openShift(uint32_t password)
        bint shiftCloseReport(uint32_t password)
        bint sell(uint32_t password, uint64_t count, uint64_t price, uint8_t department, uint8_t firstTax, uint8_t secondTax, uint8_t thirdTax, uint8_t fourthTax, const string &good)
        ShortState shortStateRequest(uint32_t password)
        FullState fullStateRequest(uint32_t password)
        bint checkConnection()
        CheckResult closeCheck(uint32_t password, uint64_t cashPaySum, uint64_t type2PaySum, uint64_t type3PaySum, uint64_t type4PaySum, uint16_t discount, uint8_t tax1, uint8_t tax2, uint8_t tax3, uint8_t tax4, const string &textToPrint);
        bint resumePrinting(uint32_t password)
        uint16_t operatingRegisterRequest(uint32_t password, uint8_t registerNumber)
        string readTable(uint32_t password, uint8_t table, uint16_t row, uint8_t field)
        bint cancelCheck(uint32_t password)
        ExchangeConfiguration readExchangeConfiguration(uint32_t sysAdmPassword, uint8_t portNumber)
        bool technologicalReset()
        bool standardStringPrint(uint32_t password, uint8_t flags, const string &p)
        uint16_t documentHeaderPrint(uint32_t password, const string &document, uint16_t documentNumber)
        uint64_t currencyRegisterRequest(uint32_t password, uint8_t registerNumber)
        bool writeTable(uint32_t sysPassword, uint8_t tableNumber, uint16_t row, uint8_t field, const string &value);
        bool timeProgramming(uint32_t sysPassword, uint8_t h, uint8_t m, uint8_t s);
        bool dateProgramming(uint32_t sysPassword, uint8_t day, uint8_t month, uint8_t year);
        bool dateConfirm(uint32_t sysPassword, uint8_t day, uint8_t month, uint8_t year);
        bool tableValuesInit(uint32_t sysPassword);
        bool cutCheck(uint32_t sysPassword, uint8_t cutType);
        FontConfiguration readFontConfiguration(uint32_t sysPassword, uint8_t fontNumber);
        bool totalExtinction(uint32_t sysPassword);
        bool scrolling(uint32_t password, uint8_t flags, uint8_t numberOfLines);
        TableStructure tableStructureRequest(uint32_t sysPassword, uint8_t tableNumber);
        FieldStructure fieldStructureRequest(uint32_t sysPassword, uint8_t table, uint8_t field);
        bool fontStringPrint(uint32_t password, uint8_t flags, uint8_t fontNumber, const string &string);
        bool shiftReportWithoutExtinction(uint32_t password);
        bool sectionsReport(uint32_t password);
        bool taxesReport(uint32_t password);
        bool cashierReport(uint32_t password);
        uint16_t payin(uint32_t password, uint64_t sum);
        uint16_t payout(uint32_t password, uint64_t sum);
        bool printCliches(uint32_t password);
        bool printDocumentEnd(uint32_t password, uint8_t printAds);
        bool printAds(uint32_t password);
        bool enterFactoryNumber(uint32_t password, uint32_t factoryNumber);
        bool enterFactoryNumber(uint32_t factoryNumber);

cdef extern from "../include/FRDriver.h" namespace "FRDriver":
    cdef enum ErrorCode:
        NoError = 0x00
        FiscalAccumulatorUnknownCommand = 0x01
        FiscalAccumulatorWrongFiscalAccumulatorState = 0x02
        FiscalAccumulatorError = 0x03
        FiscalAccumulatorCryptographCoprocessorError = 0x04
        FiscalAccumulatorLifetimeExpired = 0x05
        FiscalAccumulatorArchiveIsFull = 0x06
        FiscalAccumulatorWrongDateAndTime = 0x07
        FiscalAccumulatorNoRequestedData = 0x08
        FiscalAccumulatorWrongArgumentsInCommand = 0x09
        FiscalAccumulatorTLVDataExcessSize = 0x10
        FiscalAccumulatorNoTransportConnection = 0x11
        FiscalAccumulatorCryptographCoprocessorResourceExcess = 0x12
        FiscalAccumulatorHolderResourceExcess = 0x14
        FiscalAccumulatorWaitingForMessage = 0x15
        FiscalAccumulatorDurationOfChangeIsMoreThan24Hours = 0x16
        FiscalAccumulatorWrongTimeDifferenceBetweenOperations = 0x17
        FiscalAccumulatorMessageFromFiscalDataOperatorCantBeRecieved = 0x20
        PointOfSalesClientSummIsLowerThanCheckSumm = 0x26
        PointOfSalesCantCancelLastCommand = 0x2B
        PointOfSalesNilledCashbox = 0x2C
        PointOfSalesCheckSummIsLowerThanReversal = 0x2D
        PointOfSalesHasNoCashToPay = 0x2E
        PointOfSalesIsLockedWaitingForTaxInspector = 0x30
        PointOfSalesTotalExtintionIsRequired = 0x32
        PointOfSalesWrongArgumnetsInCommand = 0x33
        PointOfSalesNoData = 0x34
        PointOfSalesWrongArgumentOnSetup = 0x35
        PointOfSalesWrongArgumentInCommandForThisPointOfSalesImplementation = 0x36
        PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation = 0x37
        PointOfSalesROMError = 0x38
        PointOfSalesInternalSoftwareError = 0x39
        PointOfSalesAllowanceStorageOverflowForChange  = 0x3A
        PointOfSalesStorageOverflowForChange = 0x3B
        PointOfSalesCommandCantBeExecutedChangeIsOpened = 0x3C
        PointOfSalesCommandCantBeExecutedChangeIsClosed = 0x3D
        PointOfSalesAllowanceStorageBySectionForChange = 0x3E
        PointOfSalesAllowanceStorageByDiscountForChange = 0x3F
        PointOfSaleRangeOfDiscountsOverflow = 0x40
        PointOfSaleRangeOfCashPayOverflow = 0x41
        PointOfSaleRangeOfEx2PayOverfloat = 0x42
        PointOfSaleRangeOfEx3PayOverfloat = 0x43
        PointOfSaleRangeOfEx4PayOverfloat = 0x44
        PointOfSalePaySummLesserThanResultOfCheck = 0x45
        PointOfSaleNotEnoughCashInPointOfSales = 0x46
        PointOfSaleAllowanceStorageByTaxForChange = 0x47
        PointOfSaleResultOfCheckOverflow = 0x48
        PointOfSaleCommandCantBeExecutedInOpenedCheckOfThisType = 0x49
        PointOfSaleCantBeExecutedCheckIsOpened = 0x4A
        PointOfSaleCheckBufferOverflow = 0x4B
        PointOfSaleAccumulatorOverflowOfTaxTurnoverOfChange = 0x4C
        PointOfSaleInsertionOfCashlessPaymentSumIsGreaterThanSumOfCheck = 0x4D
        ShiftIsMoreThan24Hour = 0x4E
        WrongPassword = 0x4F
        PreviousCommandPrintingIsActive = 0x50
        CashAccumulationOverflowOfChange = 0x51
        PayType1AccumulationOverflowOfChange = 0x52
        PayType2AccumulationOverflowOfChange = 0x53
        PayType3AccumulationOverflowOfChange = 0x54
        PayType4AccumulationOverflowOfChange = 0x55
        CheckIsClosed = 0x56
        ThereIsNoDocumentToRepeat = 0x57
        WaitingForCommandToContinuePrinting = 0x58

    cdef struct ShortState:
        uint16_t posFlags
        uint8_t posMode
        uint8_t posSubMode
        uint8_t checkNumberOfActions
        uint8_t batteryVoltage
        uint8_t powerSupplyVoltage
        uint8_t checkNumberOfActionsHigher
        uint8_t lastPrintResult

    cdef struct FullState:
        char firmwareVersion[2]
        uint16_t firmwareBuild
        uint8_t firmwareDate[3]
        uint8_t numberInHall
        uint16_t currentDocumentPassthrougNumber
        uint16_t posFlags
        uint8_t posMode
        uint8_t posSubMode
        uint8_t posPort
        uint8_t date[3]
        uint8_t time[3]
        uint32_t factoryNumberLower
        uint16_t lastClosedShiftNumber
        uint8_t numberOfReRegistration
        uint8_t inn[6]
        uint16_t factoryNumberUpper

    cdef struct CheckResult:
        uint64_t change
        string url

    cdef struct ExchangeConfiguration:
        uint8_t baudRateCode;
        uint8_t tcpPortNumber;
        uint8_t byteTimeout;

    cdef struct FontConfiguration:
        uint16_t printAreaWidthPixels
        uint8_t symbolWidthWithInterval
        uint8_t symbolHeightWithInterval
        uint8_t numberOfFonts

    cdef struct TableStructure:
        string name
        uint16_t numberOfRows
        uint8_t numberOfCols

    cdef struct FieldStructure:
        string name
        uint8_t fieldType
        uint8_t numberOfBytes
        string maxValue
        string minValue


