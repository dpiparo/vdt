import numpy
import os
from ctypes import c_char_p

def vdt_arch() :
   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('vdtdiag_numpyWrapper', _path)
   f = lib['vdt_arch']
   f.restype = c_char_p
   return f()

def invoke(vi,f,type) :
  requires = ['CONTIGUOUS', 'ALIGNED']
  vi = numpy.asanyarray(vi)
  vi = numpy.require(vi, type, requires)
  vo = numpy.empty_like(vi)
  f(vi,vo,vi.size)
  return vo

def loadInvoke(vi,fn,is_single) :
   if (is_single) :
     fn+='fv'
     type = numpy.single
   else :
     fn+='v'
     type = numpy.float      

   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('vdtdiag_numpyWrapper', _path)
   f = lib[fn]
   f.restype = None
   f.argtypes = [numpy.ctypeslib.ndpointer(type,
                      flags='aligned, contiguous'),
                 numpy.ctypeslib.ndpointer(type,
                      flags='aligned, contiguous,writeable'),
                 numpy.ctypeslib.c_intp]
   return invoke(vi,f,type)

