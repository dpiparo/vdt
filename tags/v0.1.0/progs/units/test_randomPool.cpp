/*
 * test_randomPool.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */
#include "vdtdiag_random.h"

#include <iostream>

int main(){
	
	constexpr uint32_t size = 1000;
	
	// Test the Random Pool in double precision
	std::string dpfilename("testDpRandomNumbers.txt");
	randomPool<double> dprp_fromScratch(1,2,5,true);
	dprp_fromScratch.writeFile(dpfilename);
	randomPool<double> dprp_fromFile(dpfilename);
	dprp_fromFile.print();
	dpfilename="testDpRandomNumbers_rewritten.txt";
	dprp_fromFile.writeFile(dpfilename);
	
	// Test the Random Pool in single precision
	std::string spfilename("testSpRandomNumbers.txt");
	randomPool<float> sprp_fromScratch(-2e2,2e2,size);
	sprp_fromScratch.writeFile(spfilename);
	randomPool<float> sprp_fromFile(spfilename);	
	sprp_fromFile.print();
	spfilename="testSpRandomNumbers_rewritten.txt";
	sprp_fromFile.writeFile(spfilename);
	
	

}



