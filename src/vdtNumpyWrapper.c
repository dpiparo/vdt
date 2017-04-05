#include "Python.h"
#include "math.h"
#include "numpy/ndarraytypes.h"
#include "numpy/ufuncobject.h"
#include "numpy/npy_3kcompat.h"
#include "vdtFatLibWrapper.h"
/*
 * 
 * This is the C code for creating VDT
 * NumPy ufunc for a vdt_v functions.
 *
 * In this code we only define the ufunc for
 * a single dtype. 
 *
 * Details explaining the Python-C API can be found under
 * 'Extending and Embedding' and 'Python/C API' at
 * docs.python.org .
 */


#define NVDTFUN 2
static void *data[2*NVDTFUN] =
  {
   &vdt_logfv,&vdt_logv
   ,&vdt_expfv,&vdt_expv
  };

static char * fname[NVDTFUN] =
{
  "vdt_log"
  ,"vdt_exp"
};

static char * fdoc[NVDTFUN] =
{
  "vdt_log"
  ,"vdt_exp"
};


static PyMethodDef vdt_Methods[] = {
        {NULL, NULL, 0, NULL}
};

/* The loop definition must precede the PyMODINIT_FUNC. */

static void vdt_vf(char **args, npy_intp *dimensions,
		   npy_intp* steps, void* data)
{
  void (*functionPtr)(const float* __restrict__, float* __restrict__, long);

  functionPtr = data;
  
  npy_intp n = dimensions[0];
    char *in = args[0], *out = args[1];
    /* npy_intp in_step = steps[0], out_step = steps[1]; */

    // vdt_expfv((const float *)in, (float *)out,n); 
    (*functionPtr)((const float *)in, (float *)out,n); 
}

static void vdt_vd(char **args, npy_intp *dimensions,
		  npy_intp* steps, void* data)
{
  void (*functionPtr)(const double* __restrict__, double* __restrict__, long);

  functionPtr = data;
  
  npy_intp n = dimensions[0];
    char *in = args[0], *out = args[1];
    /* npy_intp in_step = steps[0], out_step = steps[1]; */

    (*functionPtr)((const double *)in, (double *)out,n); 
}


/*This a pointer to the above function*/
PyUFuncGenericFunction funcs[2] = {&vdt_vf,&vdt_vd};

/* These are the input and return dtypes of vdt_expfv.*/
static char types[4] = {NPY_FLOAT, NPY_FLOAT, NPY_DOUBLE, NPY_DOUBLE};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "vdtnpfun",
    NULL,
    -1,
    vdt_Methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_vdtnpfun(void)
{
    PyObject *m, *vdt_v, *d;
    
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }

    d = PyModule_GetDict(m);

	
    import_array();
    import_umath();

    for (int i=0;i< NVDTFUN;++i) {   
      vdt_v = PyUFunc_FromFuncAndData(funcs, data+2*i, types, 2, 1, 1,
                                    PyUFunc_None, fname[i],
                                    fdoc[i], 0);
      PyDict_SetItemString(d, fname+i, vdt_v);
      Py_DECREF(vdt_v);
    }
    return m;
}
#else
PyMODINIT_FUNC initvdtnpfun(void)
{
    PyObject *m, *vdt_v, *d;
    
    m = Py_InitModule("vdtnpfun", vdt_Methods);
    if (m == NULL) {
        return;
    }

    d = PyModule_GetDict(m);

    import_array();
    import_umath();

    for (int i=0;i< NVDTFUN; ++i) {   
      vdt_v = PyUFunc_FromFuncAndData(funcs, data+2*i, types, 2, 1, 1,
				      PyUFunc_None, fname[i],
				      fdoc[i], 0);
      PyDict_SetItemString(d, fname[i], vdt_v);
      Py_DECREF(vdt_v);
    } 
}
#endif

