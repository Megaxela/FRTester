# distutils: language = c++
# distutils: include_dirs = ../include ../
# distutils: sources = ../src/Testing/TestEnvironment.cpp ../src/FRDriver.cpp ../src/Tools/ByteArray.cpp ../src/Tools/Logger.cpp

cimport environment
cimport driver

# Python обертка вокруг тестового драйвера
cdef class PyTestFRDriver:
        cdef TestDriver* thisptr

        def pure_driver(self):
            cdef PyFRDriver d = PyFRDriver()
            d.driver = self.thisptr.pureDriver()
            return d

# Python обертка вокрег тестировочного окружения
cdef class PyTestEnvironment:
        cdef TestEnvironment* thisptr

        cpdef driver(self):
            pass
            #return createTestDriver(self.thisptr.driver())