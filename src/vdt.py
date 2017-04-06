import os
import imp
vdtnpfun= imp.load_dynamic('vdtnpfun','vdtnpfun_directory/vdtnpfun.so')
from vdtnpfun import *
import numpy
from ctypes import c_char_p
def vdt_arch() :
   _path = os.path.dirname('__file__')
   lib = numpy.ctypeslib.load_library('libvdtFatLibWrapper', _path)
   f = lib['vdt_arch']
   f.restype = c_char_p
   return f()

