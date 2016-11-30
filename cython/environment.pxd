# distutils: language = C++
# distutils: include_dirs = include
# distutils: sources = src/Testing/TestEnvironment.cpp src/FRDriver.cpp

cimport driver

cdef extern from "../include/Testing/TestDriver.h":
    cdef cppclass TestDriver(FRDriver):
        FRDriver* pureDriver()


cdef extern from "../include/Testing/TestEnvironment.h":
    cdef cppclass TestEnvironment:
        TestDriver* driver()

cdef class TestEnvironment:
        cdef TestEnvironment* thisptr;

        def driver(self):
            return createDriver(self.thisptr.driver())
