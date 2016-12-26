# coding=utf-8

cdef class converters:

    def __init__(self):
        raise TypeError("You cant create this object.")

    @staticmethod
    def pos_mode_to_string(mode):
        """
        @brief Функция для преобразования кода режима в строку.
        @param mode Режим
        @return Строковое представление режима.
        """
        return FRDriver.Converters.posModeToString(mode)

    @staticmethod
    def error_to_string(error):
        """
        @brief Функция для преобразования кода ошибки в строку.
        @param error Код ошибки.
        @return Строковое представление ошибки.
        """
        return FRDriver.Converters.errorToString(error)

    @staticmethod
    def pos_sub_mode_to_string(subMode):
        """
        @brief Функция для преобразования подрежима в строку.
        @param subMode Подрежим.
        @return Строковое представление подрежима.
        """
        return FRDriver.Converters.posSubModeToString(subMode)

    @staticmethod
    def last_print_result_to_string(result):
        """
        @brief Функция для преобразования результата последней
        печати в строку.
        @param result Результат.
        @return Строковое представление результата.
        """
        return FRDriver.Converters.lastPrintResultToString(result)

cdef class PyFRDriver:
        """
        @brief Класс, описывающий драйвер для работы с
        Фискальным Регистратором напрямую через указанный
        интерфейс с использованием указанного протокола.
        Если команда возвращает некие данные (не успешность
        своего выполнения) - тогда данные считаются валидными
        только в случае, если последняя ошибка == 0x00.
        """

        cdef FRDriver *driver;

        def __init__(self):
            pass

        def send_raw(self, data):
            """
            @brief Отправка чистых наборов байт. Эти байты будут обернуты
            в протокол и отправлены по интерфейсу.
            Результат не обрабатывается драйвером и последняя ошибка,
            получаемая через getLastError не устанавливается.
            @param data Передаваемые данные.
            @return Не очищенный от служебных данных ответ от ФР.
            """
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
            """
            @brief Геттер для интерфейса драйвера.
            @return Указатель на текущий интерфейс.
            """
            return createPhysicalInterface(self.driver.physicalInterface().get())

        def get_last_error(self):
            """
            @brief Метод для получения ошибки выполнения последней
            команды.
            @return Код ошибки.
            """
            return self.driver.getLastError()

        def get_last_received_cashier_number(self):
            """
            @brief Метод для получения последнего полученного
            порядкового номера кассира.
            @return Порядковый номер кассира.
            """
            return self.driver.getLastReceivedCashierNumber()

        def beep(self, password):
            """
            @brief Сигнал.
            @param password Пароль кассира.
            @return Успех выполнения.
            """
            return self.driver.beep(password)

        def open_shift(self, password):
            """
            @brief Открыть смену.
            @param password Пароль кассира.
            """
            return self.driver.openShift(password)

        def shift_close_report(self, password):
            """
            @brief Отчет от закрытии смены.
            @param password Пароль администратора или системного администратора.
            @return Успешность выполнения команды.
            """
            return self.driver.shiftCloseReport(password)

        def sell(self, password, count, price, department, firstTax, secondTax, thirdTax, fourthTax, good):
            """
            @brief Продажа (приход)
            @param password Пароль кассира.
            @param count Количество (будет обрезано до 5 байт)
            @param price Цена (будет обрезана до 5 байт)
            @param department Номер отдела. 0..16 режим свободной продажи.
            255 - режим продажи по коду товара.
            @param firstTax Первая налоговая группа.
            @param secondTax Вторая налоговая группа.
            @param thirdTax Третья налоговая группа.
            @param fourthTax Четвертая налоговая группа.
            @param good Название товара.
            @return Удалось ли совершить продажу.
            """
            return self.driver.sell(password, count, price, department, firstTax, secondTax, thirdTax, fourthTax, good)

        def short_state_request(self, password):
            """
            @brief Короткий запрос состояния.
            @param password Пароль кассира.
            @return Результат. Верет, только если
            последняя ошибка равна 0.
            """
            return self.driver.shortStateRequest(password)

        def full_state_request(self, password):
            """
            @brief Полный запрос состояния.
            @param password Пароль кассира.
            @return Результат. Верен, только если
            последняя ошибка равна 0
            """
            return self.driver.fullStateRequest(password)

        def check_connection(self):
            """
            @brief Метод для проверки наличия соединения.
            @return Есть ли соединение.
            """
            return self.driver.checkConnection()

        def close_check(self, password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint):
            """
            @brief Метод для закрытия чека.
            @param password Пароль кассира.
            @param cashPaySum Сумма, оплаченная наличными.
            @param type2PaySum Сумма, оплаченная безналом второго типа.
            @param type3PaySum Сумма, оплаченная безналом третьего типа.
            @param type4PaySum Сумма, оплаченная безналом четвертного типа.
            @param discount Скидка (>0) или надбавка (<0)
            @param tax1 Первый налог.
            @param tax2 Второй налог.
            @param tax3 Третий налог.
            @param tax4 Четвертый налог.
            @param textToPrint Текст для печати на чеке.
            @return Результат закрытия чека. URL и сдача.
            """
            return self.driver.closeCheck(password, cashPaySum, type2PaySum, type3PaySum, type4PaySum, discount, tax1, tax2, tax3, tax4, textToPrint)

        def resume_printing(self, password):
            """
            @brief Метод для продолжения печати.
            @param password Пароль кассира.
            @return Результат выполнения команды.
            """
            return self.driver.resumePrinting(password)

        def operating_register_request(self, password, registerNumber):
            """
            @brief Метод для получения операционного регистра.
            @param password Пароль кассира.
            @param registerNumber Номер регистра.
            @return Содержимое регистра
            """
            return self.driver.operatingRegisterRequest(password, registerNumber)

        def read_table_str(self, password, table, row, field):
            """
            @brief Метод для считывания значения
            @param password Пароль системного администратора
            @param table Таблица
            @param row Ряд (строка)
            @param field Поле (столбец)
            @return Строковое значение таблицы
            """
            return self.driver.readTableStr(password, table, row, field)

        def read_table_bin(self, password, table, row, field):
            return self.driver.readTableBin(password, table, row, field)

        def cancel_check(self, password):
            """
            @brief Метод для аннулирования чека.
            @param password Пароль кассира.
            @return Успешность аннулирования.
            """
            return self.driver.cancelCheck(password)

        def read_exchange_configuration(self, sysAdmPassword, portNumber):
            """
            @brief Метод для чтения параметров обмена
            @param sysAdmPassword Пароль системного администратора.
            @param portNumber Номер порта.
            @return Структура с параметрами обмена
            """
            return self.driver.readExchangeConfiguration(sysAdmPassword, portNumber)

        def technological_reset(self):
            """
            @brief Метод для произведения технологического обнуления.
            @return Успешность выполнения команды.
            """
            return self.driver.technologicalReset()

        def standard_string_print(self, password, flags, printString):
            """
            @brief Метод для печати стандартной строки.
            @param password Пароль кассира.
            @param flags Флаги.<br>
            0 бит - операционный журнал.<br>
            1 бит - чековая лента.<br>
            2 бит - подкладной документ.<br>
            3 бит - слип-чек.<br>
            4 бит - ничего.
            5 бит - ничего.
            6 бит - перенос строк.<br>
            7 бит - отложенная печать.
            @param print Строка, длиной от 0 до 200 байт.
            @return Успешность выполнения команды.
            """
            return self.driver.standardStringPrint(password, flags, printString)

        def document_header_print(self, password, document, documentNumber):
            """
            @brief Метод для печати заголовка документа.
            @param password Пароль кассира.
            @param document Наименование документа.
            @param documentNumber Номер документа.
            @return Сквозной номер документа.
            """
            return self.driver.documentHeaderPrint(password, document, documentNumber)

        def currency_register_request(self, password, registerNumber):
            """
            @brief Метод для запроса денежного Ф регистра.
            @param password Пароль кассира.
            @param registerNumber Номер регистра.
            @return 6 байт с содержимым регистра.
            """
            return self.driver.currencyRegisterRequest(password, registerNumber)

        def write_table_str(self, sysPassword, tableNumber, row, field, value):
            """
            @brief Метод для записи таблицы.
            @param sysPassword Пароль системного администратора.
            @param tableNumber Таблица
            @param row Ряд
            @param field Поле
            @param value Значение до 40 или до 246 байт
            @return Успешность выполнения команды.
            """
            return self.driver.writeTable(sysPassword, tableNumber, row, field, value)

        def write_table_bin(self, sysPassword, tableNumber, row, field, value, valSize):
            return self.driver.writeTable(sysPassword, tableNumber, row, field, value, valSize)

        def time_programming(self, sysPassword, h, m, s):
            """
            @brief Метод для программирования времени.
            @param sysPassword Пароль системного администратора.
            @param h Часы.
            @param m Минуты.
            @param s Секунды.
            @return Успешность выполнения команды.
            """
            return self.driver.timeProgramming(sysPassword, h, m, s)

        def date_programming(self, sysPassword, day, month, year):
            """
            @brief Метод для программироавния даты.
            @param sysPassword Пароль системного администратора.
            @param day День.
            @param month Месяц.
            @param year Год.
            @return Успешность выполнения команды.
            """
            return self.driver.dateProgramming(sysPassword, day, month, year)

        def date_confirm(self, sysPassword, day, month, year):
            """
            @brief Метод для подтверждения даты.
            @param sysPassword Пароль системного администратора.
            @param day День.
            @param month Месяц.
            @param year Год.
            @return Успешность выполнения команды.
            """
            return self.driver.dateConfirm(sysPassword, day, month, year)

        def time_values_init(self, sysPassword):
            """
            @brief Метод для инициализации таблиц начальными
            значениями.
            @param sysPassword Пароль системного администратора.
            @return Успешность выполнения команды.
            """
            return self.driver.tableValuesInit(sysPassword)

        def cut_check(self, sysPassword, cutType):
            """
            @brief Метод для отрезки чека.
            @param sysPassword Пароль кассира.
            @param cutType Тип отрезки. <br>
            0 - полная.<br>
            1 - неполная.<br>
            @return Успешность выполнения команды.
            """
            return self.driver.cutCheck(sysPassword, cutType)

        def read_font_configuration(self, sysPassword, fontNumber):
            """
            @brief Метод для прочтения параметров шрифта.
            @param sysPassword Пароль системного администратора.
            @param fontNumber Номер шрифта.
            @return Конфигурация шрифта.
            """
            return self.driver.readFontConfiguration(sysPassword, fontNumber)

        def total_extinction(self, sysPassword):
            """
            @brief Метод для выполнения общего гашения.
            @param sysPassword Пароль системного администратора.
            @return Успешность выполнения команды.
            """
            return self.driver.totalExtinction(sysPassword)

        def scrolling(self, password, flags, numberOfLines):
            """
            @brief Метод для выполнения протяжки (прокрутки)
            @param password Пароль кассира.
            @param flags Флаги.<br>
            0 бит - операционный журнал.<br>
            1 бит - чековая лента.<br>
            2 бит - прикладной документ.
            @param numberOfLines
            @return Успешность выполнения команды.
            """
            return self.driver.scrolling(password, flags, numberOfLines)

        def table_structure_request(self, sysPassword, tableNumber):
            """
            @brief Метод для запрашивания структуры таблицы.
            @param sysPassword Проль системного администратора.
            @param tableNumber Номер таблицы.
            @return Структура таблицы.
            """
            return self.driver.tableStructureRequest(sysPassword, tableNumber)

        def field_structure_request(self, sysPassword, table, field):
            """
            @brief Метод для запрашивания структуры поля.
            @param sysPassword Пароль системного администратора.
            @param table Номер таблицы.
            @param field Номер поля.
            @return Структура поля.
            """
            return self.driver.fieldStructureRequest(sysPassword, table, field)

        def font_string_print(self, password, flags, fontNumber, string):
            """
            @brief Метод для печати страки с заданными шрифтом.
            @param password Пароль кассира.
            @param flags Флаги.<br>
            0 бит - операционный журнал.<br>
            1 бит - чековая лента.<br>
            2 бит - подкладной документ.<br>
            3 бит - слип-чек.<br>
            4 бит - ничего.
            5 бит - ничего.
            6 бит - перенос строк.<br>
            7 бит - отложенная печать.
            @param fontNumber Номер шрифта.
            @param string Печатаемые символы.
            @return Успешность выполнения команды.
            """
            return self.driver.fontStringPrint(password, flags, fontNumber, string)

        def shift_report_without_extinction(self, password):
            """
            @brief Метод для суточного отчета без гашения.
            @param password Пароль администратора, системного
            администратора или старшего кассира.
            @return Успешность выполнения команды.
            """
            return self.driver.shiftReportWithoutExtinction(password)

        def sections_report(self, password):
            """
            @brief Метод для отчета по секциям.
            @param password Пароль администратора, системного
            администратора или старшего кассира.
            @return Успешность выполнения команды.
            """
            return self.driver.sectionsReport(password)

        def taxes_report(self, password):
            """
            @brief Метод для отчета по налогами.
            @param password Пароль администратора, системного
            администратора или старшего кассира.
            @return Успешность выполнения команды.
            """
            return self.driver.taxesReport(password)

        def cashier_report(self, password):
            """
            @brief Метод для отчета по кассирам.
            @param password Пароль администратора, системного
            администратора или старшего кассира.
            @return Успешность выполнения команды.
            """
            return self.driver.cashierReport(password)

        def payin(self, password, sum):
            """
            @brief Метод для внесения.
            @param password Пароль кассира.
            @param sum Сумма (5 байт).
            @return Сквозной номер документа.
            """
            return self.driver.payin(password, sum)

        def payout(self, password, sum):
            """
            @brief Метод для выплаты.
            @param password Пароль кассира.
            @param sum Сумма (5 байт).
            @return Сквозной номер документа.
            """
            return self.driver.payout(password, sum)

        def print_cliches(self, password):
            """
            @brief Метод для печати клише.
            @param password Пароль кассира.
            @return Успешность выполнения команды.
            """
            return self.driver.printCliches(password)

        def print_document_end(self, password, print_ads):
            """
            @brief Метод для печати конца документа.
            @param password Пароль кассира.
            @param printAds Печатать ли рекламный текст. (0 - нет, 1 - да).
            @return Успешность выполнения операции.
            """
            return self.driver.printDocumentEnd(password, print_ads)

        def print_ads(self, password):
            """
            @brief Метод для печати рекламного текста.
            @param password Пароль кассира.
            @return Успешность выполнения операции.
            """
            return self.driver.printAds(password)

        def enter_factory_number(self, factory_number, password=0):
            """
            @brief Метод для ввода заводского номера с любым паролем.
            @param password Пароль. Предполагается использование пароля 0.
            @param factoryNumber Заводской номер.
            @return Успешность выполнения операции.
            """
            return self.driver.enterFactoryNumber(password, factory_number)

        def get_non_zero_sums(self):
            """
            @brief Метод для получения необнуляемых сумм.
            @return Словарь с необнуляемыми суммами.
            """
            return self.driver.getNonZeroSums()

cdef class AbstractInterface:
        cdef PhysicalInterface* abstractptr;

        def __init__(self):
            pass

        def open_connection(self):
            return self.thisptr.openConnection()

        def close_connection(self):
            return self.thisptr.closeConnection()

        def write(self, data):
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

            return self.abstractptr.write(arr)

        def read(self, size, timeout):
            cdef ByteArray data = self.abstractptr.read(size, timeout)

            result = []
            for index in xrange(data.length()):
                result.append(data[index])

            return result

cdef class PyTCPInterface(AbstractInterface):
        cdef TCPInterface* thisptr;

        def __init__(self):
            pass

        def set_address(self, addr, port):
            """
            @brief Метод для установки адреса для соединения
            с устройством.
            @param address Строка с IPv4 адресом.
            @param port Порт.
            """
            # cdef IPv4Address address
            # address.setFromString(addr)
            #
            # return self.thisptr.setAddress(address, port)
            return None

        cpdef address(self):
            return self.thisptr.address().toString()

        def port(self):
            return self.thisptr.port()

cdef class PyCOMInterface(AbstractInterface):
        cdef COMInterface* thisptr

        def __init__(self):
            pass

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

cdef createCOMInterface(COMInterface* interface):
    v = PyCOMInterface()
    v.thisptr = interface
    v.abstractptr = <PhysicalInterface*> interface
    return v

cdef createTCPInterface(TCPInterface* interface):
    v = PyTCPInterface()
    v.thisptr = interface
    v.abstractptr = <PhysicalInterface*> interface
    return v


cdef public createPhysicalInterface(PhysicalInterface* interface):
    t = interface.type()
    if t == 0: # Abstract
        print "Can't inst. Abstract type."
        return None
    elif t == 1:
        return createCOMInterface(<COMInterface*> interface)
    elif t == 3:
        return createTCPInterface(<TCPInterface*> interface)
    else:
        print "Unknown interface type."
        return None
