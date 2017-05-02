import imp
import timeit
import numpy as np

#vdtnpfun= imp.load_dynamic('vdtnpfun','vdtnpfun_directory/vdtnpfun.so')
#from vdtnpfun import *
#print dir(vdtnpfun)

import vdt
from vdt import *
print dir(vdt)

def norf(vi):
    requires = ['CONTIGUOUS', 'ALIGNED']
    vi = np.asanyarray(vi)
    vi = np.require(vi, np.single, requires)
    return vi

def nord(vi):
    requires = ['CONTIGUOUS', 'ALIGNED']
    vi = np.asanyarray(vi)
    vi = np.require(vi, np.double, requires)
    return vi


print vdt_arch()
print vdt_sin(nord([1,2,3,4]))
print vdt_sin(norf([1,2,3,4]))
print vdt_atan2(norf([1,2,-3,-4]),norf([1,-2,3,-4]))
print vdt_sincos(norf([1,2,3,4]))
print vdt_exp(norf([1,2,3,4]))


xx = nord(np.linspace(-np.pi, np.pi, 2001))
xf = norf(np.linspace(-np.pi, np.pi, 2001))
yf = norf(np.linspace(-np.pi, np.pi, 2001))
zf = norf(np.linspace(-np.pi, np.pi, 2001))


def nfma():
    global xf,yf,zf
    return  xf*yf+zf

def nsc() :
    global xx
    s = np.sin(xx)
    c = np.cos(xx)
    return (s,c)

def nscf() :
    global xf
    s = np.sin(xf)
    c = np.cos(xf)
    return (s,c)

def vfma():
    global xf,yf,zf
    return  vdt_fma(xf,yf,zf)


def vsc() :
    global xx
    return vdt_sincos(xx)

def vscf() :
    global xf
    return vdt_sincos(xf)


def nex() :
    global xx
    return np.exp(xx)

def nexf() :
    global xf
    return np.exp(xf)


def vex() :
    global xx
    return vdt_exp(xx)

def vexf() :
    global xf
    return vdt_exp(xf)


print "timing fma"
print(timeit.timeit("nfma()", setup="from __main__ import nfma",number=100000))
print(timeit.timeit("vfma()", setup="from __main__ import vfma",number=100000))


print "timing exp"
print(timeit.timeit("nex()", setup="from __main__ import nex",number=100000))
print(timeit.timeit("nexf()", setup="from __main__ import nexf",number=100000))
print(timeit.timeit("vex()", setup="from __main__ import vex",number=100000))
print(timeit.timeit("vexf()", setup="from __main__ import vexf",number=100000))



print "timing sincos"
print(timeit.timeit("nsc()", setup="from __main__ import nsc",number=100000))
print(timeit.timeit("nscf()", setup="from __main__ import nscf",number=100000))
print(timeit.timeit("vsc()", setup="from __main__ import vsc",number=100000))
print(timeit.timeit("vscf()", setup="from __main__ import vscf",number=100000))


