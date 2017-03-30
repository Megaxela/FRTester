from distutils.core import setup
from Cython.Build import cythonize

setup(
    name="FR Driver Test Suite",
    ext_modules=cythonize("cython/*.pyx"),
)
