# -*- encoding: utf-8 -*-
import test_gear
import frdriver
import random
import sys
import time


class Test(test_gear.AbstractTest):
    name = "Пример обычного теста"
    description = "Описание примера"

    def __init__(self, env):
        """
        :type env: frdriver.PyTestEnvironment
        """
        self.environment = env
        self.random_values = {}
        self.password = 30
        self.functions_to_check = [
            (
                self.environment.driver().check_connection,
                None
            ),
            (
                self.environment.driver().beep,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().open_shift,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().sell,
                (
                    self.password, # Пароль
                    1,   # Количество
                    16,  # Цена
                    0,   # Номер отдела
                    0,   # 1 налоговая группа
                    0,   # 2 налоговая группа
                    0,   # 3 налоговая группа
                    0,   # 4 налоговая группа
                    "Test goods name"
                )
            ),
            (
                self.environment.driver().cancel_check,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().sell,
                (
                    self.password, # Пароль
                    1000,   # Количество
                    1600,  # Цена
                    0,   # Номер отдела
                    0,   # 1 налоговая группа
                    0,   # 2 налоговая группа
                    0,   # 3 налоговая группа
                    0,   # 4 налоговая группа
                    "Test goods name"
                )
            ),
            (
                self.environment.driver().close_check,
                (
                    self.password,  # Пароль
                    400,  # Нал
                    400,  # Карта
                    400,  # Карта
                    400,  # Карта,
                    0,  # Скидка
                    0,  # 1 налоговая группа
                    0,  # 2 налоговая группа
                    0,  # 3 налоговая группа
                    0,  # 4 налоговая группа
                    "Check text"
                )
            ),
            (
                self.environment.driver().shift_report_without_extinction,
                (
                    self.password,  # Пароль
                )
            ),
            (
                self.environment.driver().shift_close_report,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().short_state_request,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().full_state_request,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().operating_register_request,
                (
                    self.password, # Пароль
                    120  # Номер регистра
                )
            ),
            (
                self.environment.driver().read_table,
                (
                    self.password,  # Пароль
                    1,  # Таблица
                    0,  # Строка
                    0   # Поле
                )
            ),
            (
                self.environment.driver().read_exchange_configuration,
                (
                    self.password, # Пароль
                    0
                )
            ),
            (
                self.environment.driver().standard_string_print,
                (
                    self.password,  # Пароль
                    0b00000000,     # Флаги
                    "Manual string #" + str(self.get_random_number(0, 100))
                )
            ),
            (
                self.environment.driver().document_header_print,
                (
                    self.password,
                    "Some custom name",
                    332
                )
            ),
            (
                self.environment.driver().currency_register_request,
                (
                    self.password,
                    128
                )
            ),
            (
                self.environment.driver().cut_check,
                (
                    self.password,
                    0  # FULL CUT
                )
            ),
            (
                self.environment.driver().read_font_configuration,
                (
                    self.password,
                    2  # Номер шрифта
                )
            ),
            (
                self.environment.driver().scrolling,
                (
                    self.password,
                    10  # Количество строк
                )
            ),
            (
                self.environment.driver().font_string_print,
                (
                    self.password,
                    0b00000000,
                    3,
                    "Test string #" + str(self.get_random_number(0, 100))
                )
            ),
            (
                self.environment.driver().sections_report,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().taxes_report,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().cashier_report,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().print_cliches,
                (
                    self.password,
                )
            ),
            (
                self.environment.driver().print_document_end,
                (
                    self.password,
                    1  # Print ads
                )
            ),
            (
                self.environment.driver().print_ads,
                (
                    self.password,
                )
            )
        ]

    def get_random_number(self, min_value, max_value):
        if (min_value, max_value) not in self.random_values:
            self.random_values[(min_value, max_value)] = []

        val = random.randint(min_value, max_value)

        self.random_values[(min_value, max_value)].append(val)

        return val

    def execute(self):
        driver = self.environment.driver()
        logger = self.environment.logger()

        # Проверка наличия соединения С ФР
        if not driver.check_connection():
            logger.log("Соединение с ФР отсутствует или метод checkConnection не работает.")
            return False

        for test in self.functions_to_check:
            done = False

            meth_name = str(test[0]).split(' ')[2]
            logger.log("Тестируем \"" + meth_name + "\"")

            while not done:
                try:
                    if test[1] is None:
                        result = test[0]()
                    else:
                        result = test[0](*test[1])
                except:
                    logger.log("Команда \"" + meth_name + "\"  вызвала исключение: " + str(sys.exc_info()))
                    return False

                if driver.get_last_error() != 0:
                    if driver.get_last_error() == 0x50: # Идет печать результатов выполнения предыдущей команды
                        logger.log("Идет печать прошлой команды. Ждем 500ms и пробуем еще раз.")
                        time.sleep(0.5)
                        continue
                    else:
                        logger.log("Команда \"" +
                                   meth_name +
                                   "\" вернула ошибку: #" +
                                   hex(driver.get_last_error()) + " " +
                                   frdriver.converters.error_to_string(driver.get_last_error()))
                        return False
                done = True

            logger.log("Команда \"" + meth_name + "\" выполнена успешно. Ответ: " + str(result))

        return True
