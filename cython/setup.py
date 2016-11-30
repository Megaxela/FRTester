from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os

def merge_files():
    full_file = ""

    # fl = open(os.path.join("split", "header.txt"))
    # full_file = fl.read()
    # fl.close()
    #
    # full_file += "\n"

    for root, dirs, files in os.walk("split"):
        for file in files:
            if file.endswith(".pxd"):
                full_path = os.path.join(root, file)
                print "Found " + full_path

                fl = open(full_path, 'r')

                full_file += fl.read()

                fl.close()
                full_file += "\n"

    for root, dirs, files in os.walk("split"):
        for file in files:
            if file.endswith(".pyx"):
                full_path = os.path.join(root, file)
                print "Found " + full_path

                fl = open(full_path, 'r')

                full_file += fl.read()

                fl.close()
                full_file += "\n"

    fl = open("frdriver.pyx", "w")
    fl.write(full_file)
    fl.close()

if __name__ == "__main__":
    merge_files()

    # setup(
    #     name="FR Testing Suite",
    #     ext_modules=cythonize(
    #         "frdriver.pyx",
    #         extra_compile_args="-std=c++11"
    #     )
    # )

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
                    '../tests/Tests/CycleTest.cpp',
                    '../tests/Tests/PythonTest.cpp',
                    '../src/Tools/TTY.cpp',
                    '../src/Tools/Logger.cpp',
                    '../src/Tools/ByteArray.cpp',
                    '../src/Abstract/PhysicalInterface.cpp',
                    '../src/Abstract/Protocol.cpp',
                    '../src/Implementation/COMInterface.cpp',
                    '../src/Implementation/DefaultProtocol.cpp',
                    '../src/Implementation/TestProtocol.cpp',
                    '../src/FRDriver.cpp'
                ],
                extra_compile_args=['-std=c++11', '-D_hypot=hypot'],
                language='c++',
                include_dirs=['..', '../include']
            )
        ],
        cmdclass={'build_ext': build_ext}
    )
