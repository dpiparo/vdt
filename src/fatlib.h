#ifndef  __VDT_FATLIB__
#define __VDT_FATLIB__
#include<iostream>
#include<string>

typedef float __attribute__( ( vector_size( 16 ) ) ) float32x4_t;
typedef float __attribute__( ( vector_size( 32 ) ) ) float32x8_t;
typedef float __attribute__( ( vector_size( 64 ) ) ) float32x16_t;

namespace {
static std::string fathi;
}
#define FATHALLO(...) char const * __attribute__ ((__target__ (__VA_ARGS__))) \
  fathelloCPP() { fathi = std::string("targer is ")+#__VA_ARGS__; return fathi.c_str();}

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

#define FATLIB(RET,FUN) \
RET __attribute__ ((__target__ ("default"))) FUN \
RET __attribute__ ((__target__ ("sse3"))) FUN \
RET __attribute__ ((__target__ ("arch=corei7"))) FUN \
RET __attribute__ ((__target__ ("arch=bdver1"))) FUN \
RET __attribute__ ((__target__ ("avx2","fma"))) FUN \
RET __attribute__ ((__target__ ("avx"))) FUN \
RET __attribute__ ((__target__ ("avx512f"))) FUN


inline
float theFMA (float x, float y, float z) { return x+y*z;}


#define FATFMA myfmaCPP(float x, float y, float z) { return theFMA(x,y,z);} 
#define FATFMARETURN float

namespace {
FATLIB(FATFMARETURN,FATFMA)
}

extern "C" {
  float myfma(float x, float y, float z) { return myfmaCPP(x,y,z);} 
}

#endif
