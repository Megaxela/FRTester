

cdef class PyFRDriver:

        cdef FRDriver *driver;

        def __init__(self):
            pass

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

        def physical_interface(self):
            return createPhysicalInterface(self.driver.physicalInterface().get())

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

        def short_state_request(self, password):
            return self.driver.shortStateRequest(password)

        def full_state_request(self, password):
            return self.driver.fullStateRequest(password)

        def check_connection(self):
            return self.driver.checkConnection()

        def close_check(self, password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint):
            return self.driver.closeCheck(password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint)

        def resume_printing(self, password):
            return self.driver.resumePrinting(password)

        def operating_register_request(self, password, registerNumber):
            return self.driver.operatingRegisterRequest(password, registerNumber)

        def read_table(self, password, table, row, field):
            return self.driver.readTable(password, table, row, field)

        def cancel_check(self, password):
            return self.driver.cancelCheck(password)


cdef class PyCOMInterface:

        cdef COMInterface *thisptr

        def __init__(self):
            pass

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
