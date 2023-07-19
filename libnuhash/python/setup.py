import sys
from setuptools import setup

try:
    from Cython.Build import cythonize
except ImportError:
    sys.exit("Cython is required!")

setup(
    name='NuHash',
    ext_modules=cythonize("nuhash.pyx"),
    zip_safe=False
)
