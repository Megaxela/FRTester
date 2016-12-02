# -*- encoding: utf-8 -*-
import test_gear

print '2 test'

class Test(test_gear.AbstractTest):
    name = "Пример обычного теста"
    description = "Описание примера"

    def __init__(self, env):
        self.environment = env

    def execute(self):
        self.driver = self.environment.driver()

        request_result = self.driver.short_state_reqeust()

        print request_result
        print self.driver.get_last_error()

        return True

