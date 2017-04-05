import imp
import numpy
vdtnpfun= imp.load_dynamic('vdtnpfun','vdtnpfun_directory/vdtnpfun.so')
import vdtnpfun
print dir(vdtnpfun)

vi=[1,2,3,4]
requires = ['CONTIGUOUS', 'ALIGNED']
vi = numpy.asanyarray(vi)
vi = numpy.require(vi, numpy.single, requires)
vo = numpy.empty_like(vi)
print vdtnpfun.vdt_exp(vi)
vd=[1,2,3,4]
requires = ['CONTIGUOUS', 'ALIGNED']
vd = numpy.asanyarray(vi)
vd = numpy.require(vi, numpy.double, requires)
vo = numpy.empty_like(vi)
print vdtnpfun.vdt_exp(vd)
