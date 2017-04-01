import numpy
import os

def invokef(vi,f) :
  requires = ['CONTIGUOUS', 'ALIGNED']
  vi = numpy.asanyarray(vi)
  vi = numpy.require(vi, numpy.single, requires)
  vo = numpy.empty_like(vi)
  f(vi,vo,vi.size)
  return vo

def loadInvokef(vi,fn) :
   fn+='fv'
   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('vdtdiag_numpyWrapper', _path)
   f = lib[fn]
   f.restype = None
   f.argtypes = [numpy.ctypeslib.ndpointer(numpy.single,
                      flags='aligned, contiguous'),
                 numpy.ctypeslib.ndpointer(numpy.single,
                      flags='aligned, contiguous,writeable'),
                 numpy.ctypeslib.c_intp]
   return invoke(vi,f)

