import imp
import numpy
npufunc= imp.load_dynamic('npufunc','npufunc_directory/npufunc.so')
import npufunc


vi=[1,2,3,4]
requires = ['CONTIGUOUS', 'ALIGNED']
vi = numpy.asanyarray(vi)
vi = numpy.require(vi, numpy.single, requires)
vo = numpy.empty_like(vi)
print npufunc.vdt_expfv(vi)
