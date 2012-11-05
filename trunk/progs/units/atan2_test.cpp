#ifndef ATAN2_TEST_CPP_
#define ATAN2_TEST_CPP_

#include "atan2.h"
#include <cmath>
#include "vdtdiag_helper.h"
using namespace vdt;
using namespace vdth;

int main(){

	constexpr uint32_t size=10;
	
	//sp 
    float spvals1[size]={-1e30f,-1e19f,-300.f,-20.f,-0.23f,0.f,13.f,230.f,1e20f,1e30f};
    float spvals2[size]={1e2f,1e1f,400.f,20.f,0.934f,0.f,-21.2f,-532.f,-1e17f,-1e29f};
    printFuncDiff("atan2f", (spsp2function)atan2,(spsp2function)fast_atan2f,spvals1,spvals2,size);
}

#endif /* ATAN2_TEST_CPP_ */
