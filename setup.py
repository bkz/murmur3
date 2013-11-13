from distutils.core import setup, Extension
import version

setup(
    name = 'murmur3',
    version = version.MODULE_VERSION,
    ext_modules = [
        Extension('murmur3', ['module.cpp', 'MurmurHash3.cpp'],),
        ],
)
