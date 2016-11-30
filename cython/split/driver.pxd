from libcpp.string cimport string

ctypedef unsigned char uint8_t
ctypedef unsigned short uint16_t
ctypedef unsigned int uint32_t
ctypedef unsigned long long uint64_t

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
    cdef cppclass FRDriver:
        FRDriver()

        ByteArray sendRaw(const ByteArray &data)
        void setInterface(PhysicalInterface* interface)
        PhysicalInterface* physicalInterface()
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

cdef extern from "../include/FRDriver.h" namespace "FRDriver":
    cdef enum ErrorCode:
        pass

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


