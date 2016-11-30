from distutils.core import setup
from Cython.Build import cythonize

setup(
    name="FR Testing Suite",
    ext_modules=cythonize("*.pyx")
)