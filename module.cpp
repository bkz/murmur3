#include <Python.h>
#include "MurmurHash3.h"

const char *documentation =
    "Python wrapper around the MurmurHash3 library, written in C++. "
    "Functions included are: hash32, hash128, and hash128_64. All of "
    "these functions take the same parameters, which are: "
    "(<str/unicode/buffer>, [seed]), but may return different types.";

static PyObject* hash32(PyObject *self, PyObject *args)
{
    char *value = NULL;
    int len = 0;
    uint32_t seed = 0;
    uint32_t h = 0;
    PyObject *result;

    if (PyTuple_Size(args) == 2) {
        if (!PyArg_ParseTuple(args, "s#I", &value, &len, &seed)) {
            return NULL;
        }
    } else {
        if (!PyArg_ParseTuple(args, "s#", &value, &len)) {
            return NULL;
        }
    }

    MurmurHash3_x86_32(value, len, seed, &h);
    result = PyLong_FromUnsignedLong(h);
    if (!result) {
        PyErr_SetString(PyExc_MemoryError, "PyLong_FromUnsignedLong() returned NULL.");
    }

    return result;
}

static PyObject *hash128(PyObject *self, PyObject *args)
{
    char *value = NULL;
    int len = 0;
    int i;
    uint32_t seed = 0;
    uint32_t out[4] = {0, 0, 0, 0};
    PyObject *result;
    PyObject *tmp[4];

    if (PyTuple_Size(args) == 2) {
        if (!PyArg_ParseTuple(args, "s#I", &value, &len, &seed)) {
            return NULL;
        }
    } else {
        if (!PyArg_ParseTuple(args, "s#", &value, &len)) {
            return NULL;
        }
    }
    
    MurmurHash3_x86_128(value, len, seed, &out);
    
    result = PyTuple_New(4);
    if (!result) {
        PyErr_SetString(PyExc_MemoryError, "PyTuple_New() returned NULL.");
        return NULL;
    }
    
    /* TODO: Does tmp[] need to be Py_DECREF()'d on error? */
    for (i = 0; i < 4; i++) {
        tmp[i] = PyLong_FromUnsignedLong(out[i]);
        if (!tmp[i]) {
            Py_XDECREF(result);
            PyErr_SetString(PyExc_MemoryError, "PyLong_FromUnsignedLong() returned NULL.");
            return NULL;
        }
    
        PyTuple_SetItem(result, i, tmp[i]);
    }
    
    return result;
}

static PyObject *hash128_64(PyObject *self, PyObject *args)
{
    char *value = NULL;
    int len = 0;
    int i;
    uint32_t seed = 0;
    uint64_t out[2] = {0, 0};
    PyObject *result;
    PyObject *tmp[4];

    if (PyTuple_Size(args) == 2) {
        if (!PyArg_ParseTuple(args, "s#I", &value, &len, &seed)) {
            return NULL;
        }
    } else {
        if (!PyArg_ParseTuple(args, "s#", &value, &len)) {
            return NULL;
        }
    }
    
    MurmurHash3_x64_128(value, len, seed, &out);
    
    result = PyTuple_New(2);
    if (!result) {
        PyErr_SetString(PyExc_MemoryError, "PyTuple_New() returned NULL.");
        return NULL;
    }
    
    for (i = 0; i < 2; i++) {
        tmp[i] = PyLong_FromUnsignedLongLong(out[i]);
        if (!tmp[i]) {
            
            Py_XDECREF(result);
            PyErr_SetString(PyExc_MemoryError, "PyLong_FromUnsignedLong() returned NULL.");
            return NULL;
        }
    
        PyTuple_SetItem(result, i, tmp[i]);
    }
    
    return result;
}

static PyMethodDef methods[] = {
    {"hash32", hash32, METH_VARARGS,
        "Calculate Murmur3 32-bit unsigned hash value. "
        "Parameters: <str>, [seed]"},
        
    {"hash128", hash128, METH_VARARGS,
        "Calculate Murmur3 128-bit hash to four 32bit integers. "
        "Returns tuple (int, int, int, int). "
        "Parameters: <str>, [seed]"},
        
    {"hash128_64", hash128_64, METH_VARARGS,
        "Calculate Murmur3 128-bit hash to two 64bit integers. "
        "Returns tuple (int, int). "
        "Parameters: <str>, [seed]"},
        
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initmurmur3(void)
{
    //Py_Initialize();
    (void) Py_InitModule3("murmur3", methods, (char *)documentation);
}
