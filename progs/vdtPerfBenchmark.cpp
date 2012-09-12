/*
 * vdtPerfBenchmark.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: danilopiparo
 */

#include "vdtdiag_random.h"
#include "vdtMath.h"
#include "vdtdiag_helper.h"
#include "vdtdiag_fcnPerformance.h"
#include "vdtdiag_fcnTuples.h"
#include "vdtdiag_simpleCmd.h"

#include <iostream>
#include <fstream>
#include <string>
  
  
/**
 * Loop on the functions, and measure performance
**/

/*
 - log
 - exp
 - sin
 - cos
 - tan
 - asin
 - acos
 - atan
 - inverse sqrt
 - inverse (faster than division, based on isqrt)
*/
template <class T,class TUPLE>
void print_avg(const TUPLE& dpfcntuple, std::ofstream& ofile,uint32_t repetitions){
  fcnPerformance<T> dpExpPerf(std::get<0>(dpfcntuple),
                                   std::get<2>(dpfcntuple),
                                   std::get<1>(dpfcntuple),
                                   repetitions);
  dpExpPerf.print();
  dpExpPerf.print(ofile);
}

int main(int argc, char **argv){

	//set cmd options
	CmdOptions opt;
	opt.addOption("-n","--nick","Nickname to distinguish different runs/libraries used (required)");
	opt.addOption("-s","--size","# of numbers to be tested (default 50000)");
	opt.addOption("-r","--repetitions","# of repetitions from which statistics are calculated (default 150)");
	opt.addOption("-M","--pool_max","Upper limit of the pool interval");
	opt.addOption("-m","--pool_min","Lower limit of the pool interval");

	double POOL_MAX=5000;
	double POOL_MIN=-POOL_MAX;

	uint32_t SIZE = 50000;
	uint32_t REPETITIONS = 150;
	std::string nick = "";

	if(!opt.parseCmd(argc,argv)){
		std::cout << "Something is wrong with cmd options, try --help\n"
			<<"usage: vdtPerfBenchmark -n=<run88libmVSvdt>\n";
		return 0;
	}
	// if help was printed, exit
	if(opt.isSet("-h"))
		return 1;

	// process cmd options
	nick = opt.getArgument("-n");
	if(nick == ""){
		std::cout << "Error: Nickname was not specified!\n";
		opt.printHelp("-n");
		return 0;
	}
	

	//getArgument() contains isSet check 
	if(opt.getArgument("-s") != "")
		SIZE = std::stoi(opt.getArgument("-s").c_str());

	if(opt.getArgument("-r") != "")
		REPETITIONS = std::stoi(opt.getArgument("-r").c_str());

	if(opt.getArgument("-m") != "")
		POOL_MIN = std::stod(opt.getArgument("-m").c_str());

	if(opt.getArgument("-M") != "")
		POOL_MAX = std::stod(opt.getArgument("-M").c_str());

	// Control print
	std::cout << "Running with nick: " << nick << ", size: " << SIZE << ", repetitions: "<< REPETITIONS
			  << ", the pool max:" << POOL_MAX << "and the pool min:" << POOL_MIN << "\n";
  
	// setup filename
	std::string fname = nick + "__performance_benchmark.txt";

	std::ofstream ofile(fname);

    std::cout << "Double Precision\n";

    randomPool<double> symmrpool (POOL_MIN,POOL_MAX,SIZE);
    randomPool<double> asymmrpool (.00001,POOL_MAX,SIZE);
    randomPool<double> mone2onerpool (-1,1,SIZE);
    randomPool<double> expPool (-705,705,SIZE);

	std::vector<genfpfcn_tuple<double>> dp_fcns;
    getFunctionTuples(&dp_fcns,symmrpool,asymmrpool,mone2onerpool,expPool);
    
    for (const auto& dpfcntuple : dp_fcns)
      print_avg<double,genfpfcn_tuple<double>>(dpfcntuple,ofile,REPETITIONS);
 
	// double precision vectorised

	std::vector<genfpfcnv_tuple<double>> dp_fcnsv;
    getFunctionTuplesvect(&dp_fcnsv,symmrpool,asymmrpool,mone2onerpool,expPool);

    for (const auto& dpfcntuple : dp_fcnsv)
      print_avg<double,genfpfcnv_tuple<double>>(dpfcntuple,ofile,REPETITIONS);    

    
	// NOW SINGLE PRECISION    
      
    std::cout << "Single Precision\n";
      
    randomPool<float> symmrpoolf (POOL_MIN,POOL_MAX,SIZE);
    randomPool<float> asymmrpoolf (.00001,POOL_MAX,SIZE);
    randomPool<float> mone2onerpoolf (-1,1,SIZE);
	randomPool<float> expPoolf (-80,80,SIZE);
    
	std::vector<genfpfcn_tuple<float>> sp_fcns;
    getFunctionTuples(&sp_fcns,symmrpoolf,asymmrpoolf,mone2onerpoolf,expPoolf);
    
    for (const auto& spfcntuple : sp_fcns)
      print_avg<float,genfpfcn_tuple<float>>(spfcntuple,ofile,REPETITIONS);
      
	// single precision vectorised

	std::vector<genfpfcnv_tuple<float>> sp_fcnsv;
    getFunctionTuplesvect(&sp_fcnsv,symmrpoolf,asymmrpoolf,mone2onerpoolf,expPoolf);

    for (const auto& spfcntuple : sp_fcnsv)
      print_avg<float,genfpfcnv_tuple<float>>(spfcntuple,ofile,REPETITIONS);   

	ofile.close();
}
