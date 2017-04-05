import imp
vdtnpfun= imp.load_dynamic('vdtnpfun','vdtnpfun_directory/vdtnpfun.so')
import vdtnpfun
print dir(vdtnpfun)


from vdt_ctypes import *
import numpy as np
import timeit

#print vdt_arch()
#print vdt_expf([1,2,3,4])

loadit('vdt_expf')

N=2001
xx = np.linspace(-np.pi, np.pi, N)
xf = np.linspace(-np.pi, np.pi, N)
x = np.linspace(-np.pi, np.pi, N)


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


def vsc() :
    global x
    return vdt_sincos(x)

def vscf() :
    global x
    return vdt_sincosf(x)


def nex() :
    global xx
    return np.exp(xx)

def nexf() :
    global xf
    return np.exp(xf)


def vex() :
    global x
    return vdt_exp(x)

def vexf() :
    global x
    return vdt_expf(x)



# (fscf, ftype) = VDTFunMap['vdt_sincosf']
(fexf, ftype) = VDTFunMap['vdt_expf']


requires = ['CONTIGUOUS', 'ALIGNED']
x = numpy.asanyarray(x)
x = numpy.require(x, ftype, requires)
vo1 = numpy.empty_like(x)
vo2 = numpy.empty_like(x)

xf = numpy.asanyarray(xf)
xf = numpy.require(xf, ftype, requires)


def vscff() :
    global x
    global fscf
    global vo1
    global vo2
    fscf(x,vo1,vo2,x.size)

def vexff() :
    global x
    global fexf
    global vo1
    fexf(x,vo1,x.size)

def vexfm() :
 global x
 return vdtnpfun.vdt_exp(x)

def vexm() :
 global xx
 return vdtnpfun.vdt_exp(xx)


print "timing exp"
print timeit.timeit("nex()", setup="from __main__ import nex",number=100000), 'np exp'
print timeit.timeit("nexf()", setup="from __main__ import nexf",number=100000), 'np expf'
#print timeit.timeit("vexf()", setup="from __main__ import vexf",number=100000), 'vdt expf ctypes wrapped'
print timeit.timeit("vexff()", setup="from __main__ import vexff",number=100000), 'vdt expf ctypes'
print timeit.timeit("vexfm()", setup="from __main__ import vexfm",number=100000), 'vdt expf module'
print timeit.timeit("vexm()", setup="from __main__ import vexm",number=100000), 'vdt exp module'

