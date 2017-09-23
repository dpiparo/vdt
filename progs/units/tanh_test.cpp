/*
 * tanh_test.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */

#ifndef TANH_TEST_CPP_
#define TANH_TEST_CPP_

#include "tanh.h"
#include "vdtdiag_helper.h"
using namespace vdt;
using namespace vdth;

int main(){

	constexpr uint32_t size=10;
	
	// dp
	double dpvals[size]={-1e200,-1e50,-300.,-20.,0.,13.,230.,1e20,1e303};
	printFuncDiff("tanh", (dpdpfunction)tanh,(dpdpfunction)fast_tanh,dpvals,size);
	printFuncDiff ("tanhv", (dpdpfunctionv) tanhv, (dpdpfunctionv) fast_tanhv, dpvals, size );

	//sp 
        float spvals[size]={-1e30f,-1e19f,-300.f,-20.f,0.f,13.f,230.f,1e20f,1e30f};
        printFuncDiff("tanhf", (spspfunction)tanhf,(spspfunction)fast_tanhf,spvals,size);
        printFuncDiff ("tanhfv", (spspfunctionv) tanhfv, (spspfunctionv) fast_tanhfv, spvals, size );
	
}

#endif /* TANH_TEST_CPP_ */
