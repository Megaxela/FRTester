
# Python обертка вокруг тестового драйвера
cdef class PyTestFRDriver(PyFRDriver):
        cdef TestDriver* thisptr

        def __init__(self):

            raise ReferenceError("You can't create instance of this class from python code.")

        cdef __cinit(self):
            pass

        def pure_driver(self):
            return createDriver(self.thisptr.pureDriver())

# Python обертка вокрег тестировочного окружения
cdef class PyTestEnvironment:
        cdef TestEnvironment* thisptr

        def __init__(self):
            raise ReferenceError("You can't create instance of this class from python code.")

        cdef __cinit(self):
            pass

        cpdef driver(self):
            return createTestDriver(self.thisptr.driver())

 # todo: Поправить этот костыль, и формировать объекты при вызове методов
cdef public createTestDriver(TestDriver* test_drv):
    obj = PyTestFRDriver()
    obj.thisptr = test_drv
    obj.driver = test_drv
    return obj

cdef public createEnvironment(TestEnvironment* env):
    obj = PyTestEnvironment()
    return obj