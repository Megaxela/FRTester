# coding=utf-8
import trigger_gear


class Trigger(trigger_gear.AbstractTrigger):
    name = "Пример триггера"
    tags = []

    def __init__(self, env, tag, arguments):
        self.environment = env
        self.tag = tag
        self.arguments = arguments

    def on_pre_execute(self):
        pass

    def on_post_execute(self):
        pass

    def succeed(self):
        return True
