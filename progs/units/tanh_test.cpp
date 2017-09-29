/*
 * tanh_test.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: Paul Seyfert
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
	double dpvals[size]={-1e200,-1e50,-10.,-0.1,0.,0.3,2.3,1e20,1e303};
	printFuncDiff("tanh", (dpdpfunction)tanh,(dpdpfunction)fast_tanh,dpvals,size);
	printFuncDiff ("tanhv", (dpdpfunctionv) tanhv, (dpdpfunctionv) fast_tanhv, dpvals, size );

	//sp 
        float spvals[size]={-1e30f,-1e1f,-3.f,-0.7f,0.f,.3f,2.3f,1e20f,1e30f};
        printFuncDiff("tanhf", (spspfunction)tanhf,(spspfunction)fast_tanhf,spvals,size);
        printFuncDiff ("tanhfv", (spspfunctionv) tanhfv, (spspfunctionv) fast_tanhfv, spvals, size );
	
}

#endif /* TANH_TEST_CPP_ */
