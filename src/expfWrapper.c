#include "Python.h"
#include "math.h"
#include "numpy/ndarraytypes.h"
#include "numpy/ufuncobject.h"
#include "numpy/npy_3kcompat.h"

/*
 * 
 * This is the C code for creating your own
 * NumPy ufunc for a vdt_expfv function.
 *
 * In this code we only define the ufunc for
 * a single dtype. 
 *
 * Details explaining the Python-C API can be found under
 * 'Extending and Embedding' and 'Python/C API' at
 * docs.python.org .
 */


static PyMethodDef vdt_expfvMethods[] = {
        {NULL, NULL, 0, NULL}
};

/* The loop definition must precede the PyMODINIT_FUNC. */
void vdt_expfv(const float* __restrict__ iarray, float* __restrict__ oarray, long size);
void vdt_expv(const double* __restrict__ iarray, double* __restrict__ oarray, long size);

static void vdt_expfvf(char **args, npy_intp *dimensions,
                            npy_intp* steps, void* data)
{
    npy_intp n = dimensions[0];
    char *in = args[0], *out = args[1];
    /* npy_intp in_step = steps[0], out_step = steps[1]; */

    vdt_expfv((const float *)in, (float *)out,n); 
}

/*This a pointer to the above function*/
PyUFuncGenericFunction funcs[1] = {&vdt_expfvf};

/* These are the input and return dtypes of vdt_expfv.*/
static char types[2] = {NPY_FLOAT, NPY_FLOAT};

static void *data[1] = {NULL};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "npufunc",
    NULL,
    -1,
    vdt_expfvMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_npufunc(void)
{
    PyObject *m, *vdt_expfv, *d;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }

    import_array();
    import_umath();

    vdt_expfv = PyUFunc_FromFuncAndData(funcs, data, types, 1, 1, 1,
                                    PyUFunc_None, "vdt_expfv",
                                    "tiwce_docstring", 0);

    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "vdt_expfv", vdt_expfv);
    Py_DECREF(vdt_expfv);

    return m;
}
#else
PyMODINIT_FUNC initnpufunc(void)
{
    PyObject *m, *vdt_expfv, *d;


    m = Py_InitModule("npufunc", vdt_expfvMethods);
    if (m == NULL) {
        return;
    }

    import_array();
    import_umath();

    vdt_expfv = PyUFunc_FromFuncAndData(funcs, data, types, 1, 1, 1,
                                    PyUFunc_None, "vdt_expfv",
                                    "vdt_expfv_docstring", 0);

    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "vdt_expfv", vdt_expfv);
    Py_DECREF(vdt_expfv);
}
#endif

