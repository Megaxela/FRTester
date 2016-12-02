from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os


def read_file(path):
    f = open(path, 'r')
    t = f.read()
    f.close()
    return t


def merge_files():
    full_file = ""

    full_file += read_file(os.path.join('split', 'driver.pxd'))
    full_file += read_file(os.path.join('split', 'environment.pxd'))
    full_file += read_file(os.path.join('split', 'driver.pyx'))
    full_file += read_file(os.path.join('split', 'environment.pyx'))

    fl = open("frdriver.pyx", "w")
    fl.write(full_file)
    fl.close()

if __name__ == "__main__":
    merge_files()

    setup(
        name='FR Testing Suite',
        ext_modules=[
            Extension(
                'frdriver',
                sources=[
                    'frdriver.pyx',
                    '../src/Testing/TestEnvironment.cpp',
                    '../src/Testing/TestDriver.cpp',
                    '../src/Testing/TestCore.cpp',
                    '../src/DriverHolder.cpp',
                    '../src/Testing/AbstractTest.cpp',
                    '../src/Testing/AbstractTriggerTest.cpp',
                    '../src/Testing/TestLogger.cpp',
                    '../tests/Tests/CycleTest.cpp',
                    '../tests/Tests/PythonTest.cpp',
                    '../tests/Triggers/PythonTrigger.cpp',
                    '../src/Tools/Settings.cpp',
                    '../src/Tools/SystemTools.cpp',
                    '../src/Tools/TTY.cpp',
                    '../src/Tools/Logger.cpp',
                    '../src/Tools/ByteArray.cpp',
                    '../src/Abstract/PhysicalInterface.cpp',
                    '../src/Abstract/Protocol.cpp',
                    '../src/Implementation/COMInterface.cpp',
                    '../src/Implementation/DefaultProtocol.cpp',
                    '../src/Implementation/TestProtocol.cpp',
                    '../src/FRDriver.cpp',
                ],
                extra_compile_args=['-std=c++11', '-D_hypot=hypot'],
                language='c++',
                include_dirs=['..', '../include']
            )
        ],
        cmdclass={'build_ext': build_ext}
    )
