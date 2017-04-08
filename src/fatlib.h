#ifndef __VDT_FATLIB__
#define __VDT_FATLIB__
#include<iostream>
#include<string>
#include<cmath>

namespace {
static std::string fathi;
}


#ifdef __linux__

#define FATHALLO(...) char const * __attribute__ ((__target__ (__VA_ARGS__))) \
  fathelloCPP() { fathi = std::string("target is ")+#__VA_ARGS__; return fathi.c_str();}

namespace {
FATHALLO("default")
FATHALLO("sse3")
FATHALLO("arch=corei7")
FATHALLO("arch=bdver1")
FATHALLO("avx2","fma")
FATHALLO("avx")
FATHALLO("avx512f")
// FATHALLO()
}

extern "C" {
  char const * vdt_arch() { return fathelloCPP();}
}
#else
extern "C" {
  char const * vdt_arch() { fathi = std::string("unknown target"); return fathi.c_str();;}
}

#endif

#ifdef __linux__

#define FATLIB(RET,FUN) \
RET __attribute__ ((__target__ ("default"))) FUN \
RET __attribute__ ((__target__ ("sse3"))) FUN \
RET __attribute__ ((__target__ ("arch=corei7"))) FUN \
RET __attribute__ ((__target__ ("arch=bdver1"))) FUN \
RET __attribute__ ((__target__ ("avx2","fma"))) FUN \
RET __attribute__ ((__target__ ("avx"))) FUN \
RET __attribute__ ((__target__ ("avx512f"))) FUN
#else
#define FATLIB(RET,FUN) RET FUN
#endif



template<typename T>
inline T theFMA (T x, T y, T z) { return x*y+z;}


#define FATFMA(T) myfmaCPP(T x, T y, T z) { return theFMA(x,y,z);} 

namespace {
  FATLIB(float,FATFMA(float))
  FATLIB(double,FATFMA(double))
}

extern "C" {
  float myfmaF(float x, float y, float z) { return myfmaCPP(x,y,z);} 
  float myfmaD(double x, double y, double z) { return myfmaCPP(x,y,z);} 
}

#endif
