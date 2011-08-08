from distutils.core import setup, Extension

setup(
    name = 'murmur3',
    version = '0.1',
    ext_modules = [
        Extension('murmur3', ['module.cpp', 'MurmurHash3.cpp'],),
        ],
)
