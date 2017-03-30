# distutils: language = c++
# distutils: include_dirs = include
# distutils: sources = src/Tools/TTY.cpp src/Tools/Logger.cpp src/Tools/ByteArray.cpp src/Abstract/PhysicalInterface.cpp src/Abstract/Protocol.cpp src/Implementation/COMInterface.cpp src/Implementation/DefaultProtocol.cpp src/Implementation/TestProtocol.cpp src/FRDriver.cpp

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


cdef class PyFRDriver:

        cdef FRDriver *driver;

        cpdef send_raw(self, data):
            if not isinstance(data, list):
                print "Data to write is not list"

            if len(data) == 0:
                print "Trying to write 0 bytes."
                return 0

            cdef ByteArray arr
            for i in data:
                if i / 256 > 0:
                    print "List has byte", i, " that is > 255. Stopping."
                    return 0

                arr.append[uint8_t](i, ByteOrder_BigEndian)

            cdef ByteArray result = self.driver.sendRaw(arr)

            list_obj = []
            for index in xrange(result.length()):
                list_obj.append(result[index])

            return list_obj

        cpdef set_interface(self, interface):
            print "Didn't implemented yet"

        def physical_interface(self):
            print "Didn't implemented yet"
            return None

        def get_last_error(self):
            return self.driver.getLastError()

        def get_last_received_cashier_number(self):
            return self.driver.getLastReceivedCashierNumber()

        def beep(self, password):
            return self.driver.beep(password)

        def open_shift(self, password):
            return self.driver.openShift(password)

        def shift_close_report(self, password):
            return self.driver.shiftCloseReport(password)

        def sell(self, password, count, price, department, firstTax, secondTax, thirdTax, fourthTax, good):
            return self.driver.sell(password, count, price, department, firstTax, secondTax, thirdTax, fourthTax, good)

        def short_state_report(self, password):
            return self.driver.shortStateRequest(password)

        def full_state_report(self, password):
            return self.driver.fullStateRequest(password)

        def check_connection(self):
            return self.driver.checkConnection()

        def close_check(self, password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint):
            return self.driver.closeCheck(password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint)

        def resumt_printing(self, password):
            return self.driver.resumePrinting(password)

        def operating_register_request(self, password, registerNumber):
            return self.driver.operatingRegisterRequest(password, registerNumber)

        def readTable(self, password, table, row, field):
            return self.driver.readTable(password, table, row, field)

        def cancel_check(self, password):
            return self.driver.cancelCheck(password)


cdef class PyCOMInterface:
        cdef COMInterface *thisptr

        def open_connection(self):
            return self.thisptr.openConnection()

        def close_connection(self):
            return self.thisptr.closeConnection()

        cpdef int write(self, data):
            if not isinstance(data, list):
                print "Data to write is not list"

            if len(data) == 0:
                print "Trying to write 0 bytes."
                return 0

            cdef ByteArray arr
            for i in data:
                if i / 256 > 0:
                    print "List has byte", i, " that is > 255. Stopping."
                    return 0

                arr.append[uint8_t](i, ByteOrder_BigEndian)

            self.thisptr.write(arr)

        cpdef read(self, size, timeout):
            cdef ByteArray data = self.thisptr.read(size, timeout)

            result = []
            for index in xrange(data.length()):
                result.append(data[index])

            return result

        def set_device(self, name):
            self.thisptr.setDevice(name)

        def device(self):
            return self.thisptr.device()

        def set_baud_rate(self, rate):
            self.thisptr.setBaudRate(rate)

        def baud_rate(self):
            self.thisptr.baudRate()

        def set_byte_send_time(self, time):
            self.thisptr.setByteSendTime(time)

cdef public createDriver(FRDriver* s):
    v = PyFRDriver()
    v.driver = s
    return v

cdef public createPhysicalInterface(PhysicalInterface* interface):
    t = interface.type()
    if t == 0: # Abstract
        print "Can't inst. Abstract type."
        return None
    elif t == 1:
        v = PyCOMInterface()
        v.thisptr = <COMInterface*> interface
        return v
    else:
        print "Unknown interface type."
        return None
