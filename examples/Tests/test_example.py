# -*- encoding: utf-8 -*-
import test_gear


class Test(test_gear.AbstractTest):
    name = "Пример обычного теста"
    description = "Описание примера"

    def __init__(self, env):
        self.environment = env

    def execute(self):
        print self.environment

        return True

