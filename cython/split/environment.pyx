
# Python обертка вокруг тестового драйвера
cdef class PyTestFRDriver(PyFRDriver):
        cdef TestDriver* thisptr

        def __init__(self):
            pass

        def pure_driver(self):
            return createDriver(self.thisptr.pureDriver())

# Python обертка вокрег тестировочного окружения
cdef class PyTestEnvironment:
        cdef TestEnvironment* thisptr

        def __init__(self):
            pass

        cpdef driver(self):
            return createTestDriver(self.thisptr.driver())

        cpdef logger(self):
            return createTestLogger(self.thisptr.logger())

cdef class PyTestLogger:
        cdef TestLogger* thisptr

        def __init__(self):
            pass

        def log(self, text):
            self.thisptr.log(text)

cdef public createTestDriver(TestDriver* test_drv):
    obj = PyTestFRDriver()
    obj.thisptr = test_drv
    obj.driver = test_drv # Здесь нет ошибки
    return obj

cdef public createEnvironment(TestEnvironment* env):
    obj = PyTestEnvironment()
    obj.thisptr = env
    return obj

cdef public createTestLogger(TestLogger* logger):
    obj = PyTestLogger()
    obj.thisptr = logger
    return obj