import numpy
import os
from ctypes import c_char_p


def invoke(vi,f,type) :
  requires = ['CONTIGUOUS', 'ALIGNED']
  vi = numpy.asanyarray(vi)
  vi = numpy.require(vi, type, requires)
  vo = numpy.empty_like(vi)
  f(vi,vo,vi.size)
  return vo

def load(fn,is_single,libname='vdtdiag_numpyWrapper') :
   if (is_single) :
     type = numpy.single
   else :
     type = numpy.float      

   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library(libname, _path)
   f = lib[fn]
   f.restype = None
   f.argtypes = [numpy.ctypeslib.ndpointer(type,
                      flags='aligned, contiguous'),
                 numpy.ctypeslib.ndpointer(type,
                      flags='aligned, contiguous,writeable'),
                 numpy.ctypeslib.c_intp]
   return (f,type)

def loadInvoke(vi,fn,is_single,libname='vdtdiag_numpyWrapper') :
   (f,type) = load(fn,is_single,libname)
   return invoke(vi,f,type)

