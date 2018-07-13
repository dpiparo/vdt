![VDT Logo](https://svnweb.cern.ch/trac/vdt/raw-attachment/ticket/3/VDTlogo.png)
# The **vdt** mathematical library
**v**ectorise**d** ma**t**h
* A collection of **fast** and **inline** implementations of mathematical functions.
* The functions can be used in autovectorised loops.
* **Double and single precision** implementations are available.
* No overhead present, no intrinsics used.
* A scalar (`T(T)`) and array signature (`void(const unsigned int,T*,T*)`) are provided.

Born and developed at [CERN](www.cern.ch), it is used, among the others, by LHC experiments and the [Geant4](http://geant4.cern.ch/) simulation toolkit.


Much of the VDT code is inspired by the well known [Cephes](http://www.netlib.org/cephes) mathematical library.

## How to get, compile and install it
The **vdt** functions are **inline** and contained in header files: they are ready to be used without compilation of an external library. In any case, there is the possibility to compile a shared library containing the array signatures `void(const unsigned int,T*,T*)`.
The makesystem chosen for **vdt** is  [CMake](http://www.cmake.org).
```bash
export INSTALLDIR=/path/to/mydir
git clone https://github.com/dpiparo/vdt.git
cd vdt
cmake -DCMAKE_INSTALL_PREFIX=$INSTALLDIR .
make
make install
```

## How to use it
Good examples of vdt functions usage are located in the **progs** and **progs/units** directories.

### The vdt functions
All **vdt** functions live in the `vdt` namespace. Their names are structured as follows:
```cpp
vdt::fast_<function_name>[f][v]
```
Where:
* The function name is one of the list in the table below.
* The `f` letter stands for the single precision function (`float`).
* The `v` letter identifies the array function.
You may wonder, why prepending `fast_`? This is done to allow the user to decide where a fast and approximate implementation of the function is to be used. It is not always obvious where the accuracy can be reduced: all the flexibility is needed.

These are the available functions:

| Function | Scalar double precision | Scalar single precision | Array double precision | Array single precision |
| -------- | ----------------------- | ----------------------- | ---------------------- | ---------------------- |
| exponential | fast_exp | fast_expf | fast_expv | fast_expfv |
| sine | fast_sin | fast_sinf | fast_sinv | fast_sinfv |
| cosine | fast_cos | fast_cosf | fast_cosv | fast_cosfv |
| tangent | fast_tan | fast_tanf | fast_tanv | fast_tanfv |
| hyperbolic tangent | fast_tanh | fast_tanhf | fast_tanhv | fast_tanhfv |
| logarithm | fast_log | fast_logf | fast_logv | fast_logfv |
| arcsine | fast_asin | fast_asinf | fast_asinv | fast_asinfv |
| arccosine | fast_acos | fast_acosf | fast_acosv | fast_acosfv |
| arctangent | fast_atan | fast_atanf | fast_atanv | fast_atanfv |
| inverse square root (1/sqrt) | fast_isqrt | fast_isqrtf | fast_isqrtv | fast_isqrtfv |


## Other Cmake options
Other options for Cmake are available to steer the creation of the makefile:
* Enable AVX extensions `-D AVX=1` 
* Enable NEON extensions on ARM `-D NEON=1`
* Benchmarking tools and unit tests (requires C++11 support by the compiler) `-D DIAG=1`
* Build static library `-D BUILD_SHARED_LIBS=0`
* Prepare the library to be pre-loaded in order to replace the calls to the default math lib at runtime `-D PRELOAD=1`

## Supported Compilers
The **vdt** functions can be used with every compiler (icc and gcc were tested).
To compile the benchmarking tools **gcc4.7** (icc12) is at least needed because of the support of c++11.
To vectorise the functions **gcc4.7** (icc12) is at least needed.

## Benchmarks
This section is for experts who want to study the details of the functions provided and/or to compare them to other implementations.
**vdt** comes with a complete benchmark suite both for accuracy and speed measurements.
To measure the speed of the functions, you should use the ''vdtPerfBenchmark''. To dump on disk the ascii files summarising the accuracy of the functions, you should use ''vdtArithmBenchmark'', while the tool to compare them is ''vdtArithmComparison''.
In order to produce the plots of the different bits as a function of input, the script to be used is ''diffhisto.py'' (which depends on [ROOT](http://root.cern.ch) to produce plots).

## Example of Performance

Double precision, Intel® Core™ i7-3930K CPU @ 3.20GHz running Scientific Linux 6. Operative interval of the input: [-5000,5000] ((0,5000] for isqrt and [-1,1] for Asin and Acos). Time in ns per call.

|Function| libm | VDT | VDT SSE | VDT AVX |
|--------| ---- | --- | ------- | ------- |
|Exp|16.7|6.1|3.8|2.9|
|Log|34.9|12.5|5.7|4.2|
|Sin|33.7|16.2|6|5.7|
|Cos|34.4|13.4|5.4|5.1|
|Tan|46.6|12.5|6.3|5.6|
|Asin|23|10.3|8.6|8.1|
|Acos|23.7|11|8.2|8.1|
|Atan|19.7|11|8.3|8.3|
|Isqrt| 9.3|6.7|3|2.1|

### Accuracy
Accuracy measured in terms of least significant bit. Average difference with respect to libm.

|Function|AVG vdt|
|--------| ------|
|Acos|0.39|
|Asin|0.32|
|Atan|0.33|
|Cos|0.25|
|Exp|0.14|
|Isqrt|0.45|
|Log|0.42|
|Sin|0.25|
|Tan|0.35|

## Related Documents:
   * A full characterisation of the accuracies please refer to this [presentation](http://indico.cern.ch/contributionDisplay.py?contribId=4&sessionId=9&confId=202688).

## Reference
If you want to cite vdt, please use your reference:
[D. Piparo, V.Innocente and T.Hauth 2014 J. Phys.: Conf. Ser. 513 052027 "Speeding up HEP experiment software with a library of fast and auto-vectorisable mathematical functions"](http://iopscience.iop.org/1742-6596/513/5/052027)

## Mailing List
The VDT projects has a mailing list: VDTlibrary-talk at cern ch, linked to an e-group with the same name.
The Infrastructure used is the one provided by CERN IT. If you don't have a CERN account, you can obtain an external one [https://simba3.web.cern.ch/simba3/SelfSubscription.aspx?groupName=your-e-group-name here].
Alternatively, feel free to contact Danilo Piparo (danilo_dot_piparo_at_cern_dot_ch).

## Licence
The VDT mathematical library is licenced under the LGPL3 licence

![LGPL3](https://svnweb.cern.ch/trac/vdt/raw-attachment/ticket/2/LGPL-3-Logo-mini.png)
