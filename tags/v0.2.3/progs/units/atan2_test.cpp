#ifndef ATAN2_TEST_CPP_
#define ATAN2_TEST_CPP_

#include "atan2.h"
#include <cmath>
#include "vdtdiag_helper.h"
using namespace vdt;
using namespace vdth;

// Temporary to "flatten" the atan2
inline float flat_fast_atan2f(float x){
  return fast_atan2f(x,x/2+1);
}

inline float flat_atan2(float x){
  return atan2(x,x/2+1);
}



int main(){

	constexpr uint32_t size=10;
	
	//sp 
    float spvals[size]={-1e30f,-1e19f,-300.f,-20.f,0.f,13.f,230.f,1e20f,1e30f};
    printFuncDiff("atanf", (spspfunction)flat_atan2,(spspfunction)flat_fast_atan2f,spvals,size);
}

#endif /* ATAN2_TEST_CPP_ */
