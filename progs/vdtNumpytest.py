from vdt import *
import numpy as np
import timeit

print vdt_arch()
print vdt_sin([1,2,3,4])
print vdt_sinf([1,2,3,4])
print vdt_atan2([1,2,-3,-4],[1,-2,3,-4])
print vdt_sincosf([1,2,3,4])

x = np.linspace(-np.pi, np.pi, 2001)

def nsc() :
    global x
    s = np.sin(x)
    c = np.cos(x)
    return (s,c)

def vsc() :
    global x
    return vdt_sincos(x)

def vscf() :
    global x
    return vdt_sincosf(x)


print(timeit.timeit("nsc()", setup="from __main__ import nsc",number=100000))
print(timeit.timeit("vsc()", setup="from __main__ import vsc",number=100000))
print(timeit.timeit("vscf()", setup="from __main__ import vscf",number=100000))

