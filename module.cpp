#include <Python.h>
#include "MurmurHash3.h"

static PyObject* hash32(PyObject *self, PyObject *args)
{
    char *value = NULL;
    int len = 0;
    uint32_t seed = 0;

    if (!PyArg_ParseTuple(args, "s#I", &value, &len, &seed))
        return NULL;

    uint32_t h = 0;
    MurmurHash3_x86_32(value, len, seed, &h);

    return PyInt_FromLong(h);
}

static PyMethodDef methods[] = {
    {"hash32", hash32, METH_VARARGS, "Calculate Murmur3 32-bit unsigned hash value."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initmurmur3(void)
{
    (void) Py_InitModule("murmur3", methods);
}
