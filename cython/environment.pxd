

cimport driver
from driver cimport FRDriver

cdef extern from "../include/Testing/TestDriver.h":
    cdef cppclass TestDriver(FRDriver):
        FRDriver* pureDriver()


cdef extern from "../include/Testing/TestEnvironment.h":
    cdef cppclass TestEnvironment:
        TestDriver* driver()

