/*
 * deleteme.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */

#ifndef DELETEME_CPP_
#define DELETEME_CPP_


#include "log.h"
#include "vdtdiag_helper.h"
using namespace vdt;
using namespace vdth;

int main(){

std::cout << getbsasstr(-0.) << std::endl;
uint64_t mask = dp2uint64(-0.);
double a=3;
a=dpORuint64(a,mask);
std::cout << getbsasstr(a) << std::endl;

  
}

#endif /* DELETEME_CPP_ */
