from vdt import *
import numpy as np
import timeit

print vdt_arch()
print vdt_sin([1,2,3,4])
print vdt_sinf([1,2,3,4])
print vdt_atan2([1,2,-3,-4],[1,-2,3,-4])
print vdt_sincosf([1,2,3,4])
print vdt_expf([1,2,3,4])


xx = np.linspace(-np.pi, np.pi, 2001)
xf = np.linspace(-np.pi, np.pi, 2001)
x = np.linspace(-np.pi, np.pi, 2001)


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
    return np.sin(xf)


def vex() :
    global x
    return vdt_exp(x)

def vexf() :
    global x
    return vdt_expf(x)



(fscf, ftype) = VDTFunMap['vdt_sincosf']
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
    global fscf
    global vo1
    fexf(x,vo1,x.size)


print "timing exp"
print(timeit.timeit("nex()", setup="from __main__ import nex",number=100000))
print(timeit.timeit("nexf()", setup="from __main__ import nexf",number=100000))
print(timeit.timeit("vex()", setup="from __main__ import vex",number=100000))
print(timeit.timeit("vexf()", setup="from __main__ import vexf",number=100000))
print(timeit.timeit("vexff()", setup="from __main__ import vexff",number=100000))


print "timing sincos"
print(timeit.timeit("nsc()", setup="from __main__ import nsc",number=100000))
print(timeit.timeit("nscf()", setup="from __main__ import nscf",number=100000))
print(timeit.timeit("vsc()", setup="from __main__ import vsc",number=100000))
print(timeit.timeit("vscf()", setup="from __main__ import vscf",number=100000))
print(timeit.timeit("vscff()", setup="from __main__ import vscff",number=100000))

