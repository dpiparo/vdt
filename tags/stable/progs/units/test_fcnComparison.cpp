/*
 * test_fcnResponse.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */
#include "vdtdiag_random.h"
#include "vdtdiag_fcnResponse.h"
#include "vdtdiag_fcnComparison.h"
#include "vdtdiag_helper.h"
#include <cmath>
#include "log.h"

#include <iostream>

int main(){
	
	constexpr uint32_t size = 100000;
	
	// Test the FcnResponse in double precision
	randomPool<double> dpRandomPool(100,1000,size);
	fcnResponse<double> dpExpResp("Log",dpRandomPool.getNumbers(), (vdth::dpdpfunction) log);
    fcnResponse<double> dpFastExpResp("Fast Log",dpRandomPool.getNumbers(), (vdth::dpdpfunction) vdt::fast_log);
    fcnComparison<double> dpExpComp("Log - libmVSvdt",
                                    dpRandomPool.getNumbers(),
                                    dpExpResp.getOutput(),
                                    dpFastExpResp.getOutput());
    dpExpComp.printStats();
	dpExpComp.writeFile("test_functionComparison.txt");

	fcnComparison<double> dpExpFromFile("test_functionComparison.txt");
	std::cout <<"Read from file: -----------------\n";
	dpExpFromFile.printStats();
        

	
}
