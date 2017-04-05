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

def invoke2to1(vi1,vi2,f,type) :
  requires = ['CONTIGUOUS', 'ALIGNED']
  vi1 = numpy.asanyarray(vi1)
  vi1 = numpy.require(vi1, type, requires)
  vi2 = numpy.asanyarray(vi2)
  vi2 = numpy.require(vi2, type, requires)
  vo = numpy.empty_like(vi1)
  f(vi1,vi2,vo,vi1.size)
  return vo

def invoke1to2(vi,f,type) :
  requires = ['CONTIGUOUS', 'ALIGNED']
  vi = numpy.asanyarray(vi)
  vi = numpy.require(vi, type, requires)
  vo1 = numpy.empty_like(vi)
  vo2 = numpy.empty_like(vi)
  f(vi,vo1,vo2,vi.size)
  return (vo1,vo2)



def load(fn,is_single,libname='libvdtFatLibWrapper',nin=1,nout=1) :
   if (is_single) :
     type = numpy.single
   else :
     type = numpy.float      

   tin = numpy.ctypeslib.ndpointer(type,flags='aligned, contiguous')
   tout = numpy.ctypeslib.ndpointer(type,flags='aligned, contiguous, writeable')

   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library(libname, _path)
   f = lib[fn]
   f.restype = None
   f.argtypes = [tin]*nin + [tout]*nout + [numpy.ctypeslib.c_intp]
   return (f,type)

def loadInvoke(vi,fn,is_single,libname='libvdtFatLibWrapper') :
   (f,type) = load(fn,is_single,libname)
   return invoke(vi,f,type)

