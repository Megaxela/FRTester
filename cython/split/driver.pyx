# coding=utf-8
cdef class PyConverters:

    def __init__(self):
        raise TypeError("You cant create this object.")

    @staticmethod
    def pos_mode_to_string(mode):
        return FRDriver.Converters.posModeToString(mode)

    @staticmethod
    def error_to_string(error):
        return FRDriver.Converters.errorToString(error)

    @staticmethod
    def pos_sub_mode_to_string(subMode):
        return FRDriver.Converters.posSubModeToString(subMode)

    @staticmethod
    def last_print_result_to_string(result):
        return FRDriver.Converters.lastPrintResultToString(result)

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

        def read_exchange_configuration(self, sys_adm_password, port_number):
            return self.driver.readExchangeConfiguration(sys_adm_password, port_number)

        def technological_reset(self):
            return self.driver.technologicalReset()

        def standard_string_print(self, password, flags, print_string):
            return self.driver.standardStringPrint(password, flags, print_string)

        def document_header_print(self, password, document, document_number):
            return self.driver.documentHeaderPrint(password, document, document_number)

        def currency_register_request(self, password, register_number):
            return self.driver.currencyRegisterRequest(password, register_number)

        def write_table(self, sys_password, table_number, row, field, value):
            return self.driver.writeTable(sys_password, table_number, row, field, value)

        def time_programming(self, sys_password, h, m, s):
            return self.driver.timeProgramming(sys_password, h, m, s)

        def date_programming(self, sys_password, day, month, year):
            return self.driver.dateProgramming(sys_password, day, month, year)

        def date_confirm(self, sys_password, day, month, year):
            return self.driver.dateConfirm(sys_password, day, month, year)

        def time_values_init(self, sys_password):
            return self.driver.tableValuesInit(sys_password)

        def cut_check(self, sys_password, cut_type):
            return self.driver.cutCheck(sys_password, cut_type)

        def read_font_configuration(self, sys_password, font_number):
            return self.driver.readFontConfiguration(sys_password, font_number)

        def total_extinction(self, sys_password):
            return self.driver.totalExtinction(sys_password)

        def scrolling(self, password, flags, numberOfLines):
            return self.driver.scrolling(password, flags, numberOfLines)

        def table_structure_request(self, sys_password, table_number):
            return self.driver.tableStructureRequest(sys_password, table_number)

        def field_structure_request(self, sys_password, table, field):
            return self.driver.fieldStructureRequest(sys_password, table, field)

        def font_string_print(self, password, flags, font_number, string):
            return self.driver.fontStringPrint(password, flags, font_number, string)

        def shift_report_without_extinction(self, password):
            return self.driver.shiftReportWithoutExtinction(password)

        def sections_report(self, password):
            return self.driver.sectionsReport(password)

        def taxes_report(self, password):
            return self.driver.taxesReport(password)

        def cashier_report(self, password):
            return self.driver.cashierReport(password)

        def payin(self, password, sum):
            return self.driver.payin(password, sum)

        def payout(self, password, sum):
            return self.driver.payout(password, sum)

        def print_cliches(self, password):
            return self.driver.printCliches(password)

        def print_document_end(self, password, print_ads):
            return self.driver.printDocumentEnd(password, print_ads)

        def print_ads(self, password):
            return self.driver.printAds(password)

        def enter_factory_number(self, factory_number, password=0):
            return self.driver.enterFactoryNumber(password, factory_number)

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
