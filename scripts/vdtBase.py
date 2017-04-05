import numpy
import os
from ctypes import c_char_p
import invokeVDT
def vdt_arch() :
   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('libvdtFatLibWrapper', _path)
   f = lib['vdt_arch']
   f.restype = c_char_p
   return f()

VDTFunMap = {}

def loadit(fn,nin=1,nout=1) :
    if not fn in VDTFunMap :
        is_single = fn.endswith('f')
        VDTFunMap[fn] = invokeVDT.load(fn+'v',is_single,'libvdtFatLibWrapper',nin,nout)

def vdt_invoke(fn,vi) :
    loadit(fn)
    (f,type) =  VDTFunMap[fn]
    return invokeVDT.invoke(vi,f,type)    

def vdt_invoke2to1(fn,vi1,vi2) :
    loadit(fn,2,1)
    (f,type) =  VDTFunMap[fn]
    return invokeVDT.invoke2to1(vi1,vi2,f,type)    

def vdt_invoke1to2(fn,vi) :
    loadit(fn,1,2)
    (f,type) =  VDTFunMap[fn]
    return invokeVDT.invoke1to2(vi,f,type)    
