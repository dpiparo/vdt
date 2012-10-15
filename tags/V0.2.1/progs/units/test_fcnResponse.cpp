/*
 * test_fcnResponse.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */
#include "vdtdiag_random.h"
#include "vdtdiag_fcnResponse.h"
#include "vdtdiag_helper.h"
#include <cmath>
#include <exp.h>

#include <iostream>

int main(){
	
	const uint32_t size = 10;
	
	// Test the FcnResponse in double precision
	const std::string dpfilename("testDpFcnPerf.txt");
	randomPool<double> dpRandomPool(-500,500,size);
	fcnResponse<double> dpExpResp("Exp",dpRandomPool.getNumbers(), (vdth::dpdpfunction) exp);
	dpExpResp.writeFile(dpfilename);
	fcnResponse<double> dpExpRespFromFile(dpfilename);
	dpExpRespFromFile.print();
	dpExpRespFromFile.writeFile("testDpFcnPerf_fromFile.txt");

	// Test the FcnResponse in single precision
	const std::string spfilename("testSpFcnPerf.txt");
	randomPool<float> spRandomPool(-50,50,size);
	fcnResponse<float> spExpResp("Exp",spRandomPool.getNumbers(), (vdth::spspfunction) expf);
	spExpResp.writeFile(spfilename);
	fcnResponse<float> spExpRespFromFile(spfilename);
	spExpRespFromFile.print();
	spExpRespFromFile.writeFile("testSpFcnPerf_fromFile.txt");
	
}
