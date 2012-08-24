VDT is a library of mathematical functions, implemented in 
double and single precision. The implementation is fast and 
with the aid of modern compilers (e.g. gcc 4.7) autovectorisable.
For more information visit:
https://svnweb.cern.ch/trac/vdt
 
The basic idea is to exploit pade polinomials. A lot of ideas were inspired by 
the cephes math library (by Stephen L. Moshier, moshier@na-net.ornl.gov) as 
well as portions of actual code. The Cephes library can be found here: 
http://www.netlib.org/cephes

Implemented functions
 - log
 - exp
 - sin
 - cos
 - tan
 - asin
 - acos
 - atan
 - inverse sqrt
 - inverse (faster than division, based on isqrt)


To compile it:
cmake .
make

If you would like to compile the executables necessary for the diagnostics
(cpu and arithmetic performance measurements) type
cmake -D DIAG=1 .
make

The executables will be put in the /bin directory.

Copyright Danilo Piparo, Vincenzo Innocente, Thomas Hauth (CERN) 2012

