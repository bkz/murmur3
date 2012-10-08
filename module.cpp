#include <Python.h>
#include "MurmurHash3.h"

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
            /*for (i--; i > -1; i--) {
                Py_DECREF(tmp[i]);
            }*/
            
            Py_DECREF(result);
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
        "Calculate Murmur3 128-bit hash. Returns tuple. "
        "Parameters: <str>, [seed]"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initmurmur3(void)
{
    Py_Initialize();
    (void) Py_InitModule("murmur3", methods);
}
