import numpy
import os
from ctypes import c_char_p
import invokeVDT
def vdt_arch() :
   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('vdtdiag_numpyWrapper', _path)
   f = lib['vdt_arch']
   f.restype = c_char_p
   return f()

VDTFunMap = {}

def loadit(fn) :
    if not fn in VDTFunMap :
        is_single = fn.endswith('f')
        VDTFunMap[fn] = invokeVDT.load(fn+'v',is_single,libname='vdtdiag_numpyWrapper')

def vdt_invoke(fn,vi) :
    loadit(fn)
    (f,type) =  VDTFunMap[fn]
    return invokeVDT.invoke(vi,f,type)    
