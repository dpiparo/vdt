/* vdtArithmComparison.cpp
*
*	created: 16.7.2012
*
*	Reads files dumped by vdtAtritmBenchmark.cpp 
*
*	Author: Ladislav Horky
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "vdtdiag_fcnResponse.h"
#include "vdtdiag_fcnTuples.h"
#include "vdtdiag_fcnComparison.h"
#include "vdtdiag_simpleCmd.h"


//converts (comma) separated list (in string) to vector of strings
void list2vector(std::string csList, std::vector<std::string>* vect, char separator = ','){
	int lastCommaPos = -1, commaPos;

	vect->clear();
	while(true){
		//no other comma, break
		commaPos = csList.find(separator,lastCommaPos+1);
		if(commaPos < 0)
			break;
		//store string between commas
		vect->push_back(csList.substr(lastCommaPos+1,commaPos-lastCommaPos-1));
		lastCommaPos = commaPos;
	}
	//process the last string after the last separator if there is any
	if(csList.size() > lastCommaPos+1)
		vect->push_back(csList.substr(lastCommaPos+1));
}

/// Performs comparison of arithmetic precision

int main(int argc, char **argv){

	// set and parse commandline options
	CmdOptions opt;
	opt.addOption("-n","--nick","Output (!) file nickname. Make sure it contains all information about compared function "
		"(libraries used, vect VS single signature comaprison...).");
	opt.addOption("-R","--reference","List of comma-separated filenames that will be used as a reference values"
		"(i.e. will be in the second column of output comparison file). The propper function names for output files will "
		"be refined from the filenames, so do not rename the files from vdtArithmBenchmark. See -T for more.");
	opt.addOption("-T","--test","List of comma-separated filenames that will be compared to reference files"
		"(their values will appear in the third column of output file). It is the responsibility of the user"
		"to make sure the reference and the test files are compatibile (same functions, same order of files, same # of entries, "
		"same inputs values...).");
	//opt.addOption("-s","--separator","Alternative seperator for the list of files (default: ',')");

	// parse and process cmd options
	if(!opt.parseCmd(argc,argv)){
		std::cout << "Something is wrong with cmd options, try --help\n"
			<< "usage: vdtArithmComparison -n=<out_nick> -R=<reffile1,reffile2...> -T=<testfile1,testfile2...>";
		return 0;
	}
	// if help was printed, exit
	if(opt.isSet("-h"))
		return 1;

	std::string nick;
	std::vector<std::string> ref, test;

	// process cmd options
	nick = opt.getArgument("-n");
	if(nick == ""){
		std::cout << "Error: Nickname was not specified!\n";
		opt.printHelp("-n");
		return 0;
	}

	std::string tmp;
	tmp = opt.getArgument("-R");
	list2vector(tmp,&ref);
	if(!ref.size()){
		std::cout << "Error: No reference files specified!\n";
		opt.printHelp("-R");
		return 0;
	}
	tmp = opt.getArgument("-T");
	list2vector(tmp,&test);
	if(!test.size()){
		std::cout << "Error: No test files specified!\n";
		opt.printHelp("-T");
		return 0;
	}
	if(test.size() != ref.size()){
		std::cout << "Error: Number of test and ref files not equal!\n";
		opt.printHelp();
		return 0;
	}
	// pheeew.. all errors hopefully checked

	//print table column names:--------------------------------------------------------------------
	std::cout << std::setw(30) << std::left << "function name"
			<< std::setw(5) << std::right << "max"
			<< std::setw(5) << std::right << "min"
			<< std::setw(6) << std::right << "mean"
			<< std::setw(6) << std::right << "RMS\n";

	for(int i=0; i<ref.size(); i++){
		//try to open files
		std::ifstream ifile(ref[i]);
		if(!ifile.is_open()){
			std::cout << "Cannot find the " << ref[i] << " file, skipping.\n";
			continue;
		}
		std::string fcname;
		bool singlePrec;
		std::getline(ifile,tmp);
		tmp.clear();

		//determine type
		std::getline(ifile,tmp);
		singlePrec = (tmp == "Double Precision")?false:true;

		//store function name (without Fast_prefix, if there is any)
		tmp.clear();
		std::getline(ifile,tmp);
		tmp = tmp.substr(tmp.find('=')+2);		//get rid of the 'Function name = ' string
		fcname = tmp.substr(tmp.find('_')+1);	//remove Fast_/VC_ prefix if present
		ifile.close();

		//check for test file;
		ifile.open(test[i]);
		if(!ifile.is_open()){
			std::cout << "Cannot find the " << test[i] << " file, skipping.\n";
			continue;
		}
		ifile.close();

		//read responces, perform comparison

		if(!singlePrec){
			fcnResponse<double> rrefDP(ref[i]),rtestDP(test[i]);
			fcnComparison<double> cDP(fcname+" "+ref[i].substr(0,ref[i].find("__"))+" VS "+
												test[i].substr(0,test[i].find("__")),
									  rrefDP.getInput(),rrefDP.getOutput(),rtestDP.getOutput());
			cDP.printStats(true);
			cDP.writeFile("comparison__"+nick+"__"+fcname+".txt");
		}else{
			fcnResponse<float> rrefSP(ref[i]),rtestSP(test[i]);
			fcnComparison<float> cSP(fcname+" "+ref[i].substr(0,ref[i].find("__"))+" VS "+
												test[i].substr(0,test[i].find("__")),
									 rrefSP.getInput(),rrefSP.getOutput(),rtestSP.getOutput());
			cSP.printStats(true);
			cSP.writeFile("comparison__"+nick+"__"+fcname+".txt");
		}
	}

//	opt.addOption("-v","--vector","Do vector VS single signature comparison (mostly debug purposes)."
//								  "Incompatible with -l switch.");
//	opt.addOption("-l","--libs","Compare libraries, which cannot be compiled at the same time (e.g. libm VS svml)."
//								"This switch requies specification of two nicks in nick argument separated by"
//								"semicolon (e.g. libm_nick;svml_nick). Then only functions without Fast_ prefixes will"
//								"be compared and the rest will be omitted. No argument required.");
//	// options with argument
//	opt.addOption("-n","--nick","Nickname to distinguish different runs/libraries used (required), see -l"
//								"for special cases.");
//
//	//parse options, check for errors, check file existence
//	if(!opt.parseCmd(argc,argv)){
//		std::cout << "Something is wrong with cmd options, try --help\n";
//		return 0;
//	}
//
//	std::string nick1,nick2;	//the 2 is used when -l specified
//
//	if(opt.isSet("-v") && opt.isSet("-l")){
//		std::cout << "Error: Impossible to combine -v -l switches (yet), try --help\n";
//		return 0;
//	}
//	//parse nicks and check for file existence
//	if(opt.getArgument("-n") == ""){
//		std::cout << "Error: Nickname(s) not specified!\n";
//		opt.printHelp("-n");
//		return 0;
//	}
//
//	if(!opt.isSet("-l")){
//		nick1 = opt.getArgument("-n");
//
//		std::ifstream testFile(nick1+"__Exp__response.txt");
//		if(!testFile.is_open()){
//			std::cout << "Wrong nickname or missing file " << nick1+"__Exp__response.txt\n";
//			return 0;
//		}
//		testFile.close();
//	}else{
//		std::string nicks = opt.getArgument("-n");
//		nick1 = nicks.substr(0,nicks.find(';'));
//		nick2 = nicks.substr(nicks.find(';')+1);
//
//		//test file presence (on one file only)
//		std::ifstream testFile(nick1+"__Exp__response.txt");
//		if(!testFile.is_open()){
//			std::cout << "Wrong nickname or missing file " << nick1+"__Exp__response.txt\n";
//			return 0;
//		}
//		testFile.close();
//		testFile.open(nick2+"__Exp__response.txt");
//		if(!testFile.is_open()){
//			std::cout << "Wrong nickname or missing file " << nick2+"__Exp__response.txt\n";
//			return 0;
//		}
//		testFile.close();
//	}
//
//	/// Do the comparison
//	std::string begin(nick1);
//	begin += "__";
//	std::string ending("__response.txt");
//
//	//print table column names:
//	std::cout << std::setw(27) << std::left << "function name"
//			<< std::setw(5) << std::right << "max"
//			<< std::setw(5) << std::right << "min"
//			<< std::setw(6) << std::right << "mean"
//			<< std::setw(6) << std::right << "RMS\n";
//	for(const auto& i_fcname : fcnNames){
//		// single nick usage
//		if(!opt.isSet("-l")){
//			//prepare filenames
//			std::string basicDP(begin+i_fcname);
//			std::string basicSP(begin+i_fcname);
//			std::string fastDP(begin+"Fast_"+i_fcname);
//			std::string fastSP(begin+"Fast_"+i_fcname);
//
//			std::string basicDPv(basicDP+"v");
//			std::string basicSPv(basicSP+"fv");
//			std::string fastDPv(fastDP+"v");
//			std::string fastSPv(fastSP+"fv");
//
//			basicSP  += "f";	fastSP	 += "f";
//
//			std::string outDP("vdtVSxx_comparison__"+basicDP+".txt");
//			std::string outDPv("vdtVSxx_comparison__"+basicDPv+".txt");
//			std::string outSP("vdtVSxx_comparison__"+basicSP+".txt");
//			std::string outSPv("vdtVSxx_comparison__"+basicSPv+".txt");
//
//			basicDP	 += ending;		fastDP	 += ending;
//			basicDPv += ending;		fastDPv	 += ending;
//			basicSP  += ending;		fastSP	 += ending;
//			basicSPv += ending;		fastSPv	 += ending;
//
//			// Load files
//			fcnResponse<double> rbasicDP(basicDP), rfastDP(fastDP),
//								rbasicDPv(basicDPv), rfastDPv(fastDPv);
//			fcnResponse<float>	rbasicSP(basicSP), rfastSP(fastSP),
//								rbasicSPv(basicSPv), rfastSPv(fastSPv);
//
//			// Do classic comparison, between a funtion and it's Fast_ equivalent
//			if(!opt.isSet("-v")){
//				fcnComparison<double>	cDP(i_fcname+" vs Fast_"+i_fcname,       rbasicDP.getInput(),rbasicDP.getOutput(),rfastDP.getOutput()),
//										cDPv(i_fcname+"v vs Fast_"+i_fcname+"v", rbasicDPv.getInput(),rbasicDPv.getOutput(),rfastDPv.getOutput());
//				fcnComparison<float>	cSP(i_fcname+"f vs Fast_"+i_fcname+"f" , rbasicSP.getInput(),rbasicSP.getOutput(),rfastSP.getOutput()),
//										cSPv(i_fcname+"fv vs Fast_"+i_fcname+"fv",rbasicSPv.getInput(),rbasicSPv.getOutput(),rfastSPv.getOutput());
//
//				// Print stats
//
//				cDP.printStats(true);
//				cDPv.printStats(true);	
//				cSP.printStats(true);
//				cSPv.printStats(true);
//
//				// Wrrrrrrite files
//				cDP.writeFile(outDP);
//				cDPv.writeFile(outDPv);
//				cSP.writeFile(outSP);
//				cSPv.writeFile(outSPv);
//
//			// Debugging comparison between single and vector signature of the function
//				// result should be the same ideally 
//			}else{
//				fcnComparison<double>cDP(i_fcname+" vs "+i_fcname+"v",					rbasicDP.getInput(),rbasicDP.getOutput(),rbasicDPv.getOutput()),
//									 cDPfast("Fast_"+i_fcname+" vs Fast_"+i_fcname+"v",  rfastDP.getInput(), rfastDP.getOutput(), rfastDPv.getOutput());
//				fcnComparison<float> cSP(i_fcname+"f vs "+i_fcname+"fv",					rbasicSP.getInput(),rbasicSP.getOutput(),rbasicSPv.getOutput()),
//									 cSPfast("Fast_"+i_fcname+"f vs Fast_"+i_fcname+"fv",rfastSP.getInput(), rfastSP.getOutput(), rfastSPv.getOutput());
//
//				// Print stats in tabular form
//				cDP.printStats(true);
//				cDPfast.printStats(true);	
//				cSP.printStats(true);
//				cSPfast.printStats(true);
//
//				// If different results, create files
//				if(cDP.hasDifference())
//							cDP.writeFile("singleVSvect_"+nick1+"__"+i_fcname+".txt");
//				if(cDPfast.hasDifference())
//							cDPfast.writeFile("singleVSvect_"+nick1+"__Fast_"+i_fcname+".txt");
//				if(cSP.hasDifference())
//							cSP.writeFile("singleVSvect_"+nick1+"__"+i_fcname+"_float.txt");
//				if(cSPfast.hasDifference())
//							cSPfast.writeFile("singleVSvect_"+nick1+"__Fast_"+i_fcname+"_float.txt");
//			}
////----------------------------- do libm VS svml library comparison
//		}else{ // i.e. -l set
//			//prepare filenames
//			std::string basicDP1(nick1+"__"+i_fcname);
//			std::string basicSP1(basicDP1);
//			std::string basicDP2(nick2+"__"+i_fcname);
//			std::string basicSP2(basicDP2);
//
//			std::string basicDPv1(basicDP1+"v");
//			std::string basicSPv1(basicSP1+"fv");
//			std::string basicDPv2(basicDP2+"v");
//			std::string basicSPv2(basicSP2+"fv");
//
//			basicSP1  += "f";	basicSP2 += "f";
//
//			std::string outDP("libmVSsvml_comparison__"+basicDP1+".txt");
//			std::string outDPv("libmVSsvml_comparison__"+basicDPv1+".txt");
//			std::string outSP("libmVSsvml_comparison__"+basicSP1+".txt");
//			std::string outSPv("libmVSsvml_comparison__"+basicSPv1+".txt");
//
//			basicDP1  += ending;		basicDP2	 += ending;
//			basicDPv1 += ending;		basicDPv2	 += ending;
//			basicSP1  += ending;		basicSP2	 += ending;
//			basicSPv1 += ending;		basicSPv2	 += ending;
//			
//			// Load files
//			fcnResponse<double> rbasicDP1(basicDP1), rbasicDP2(basicDP2),
//								rbasicDPv1(basicDPv1), rbasicDPv2(basicDPv2);
//			fcnResponse<float>	rbasicSP1(basicSP1), rbasicSP2(basicSP2),
//								rbasicSPv1(basicSPv1), rbasicSPv2(basicSPv2);
//
//			//comparison
//			std::string libmstr("libm_");
//			fcnComparison<double>	cDPl(libmstr+i_fcname+" vs svml_"+i_fcname,       rbasicDP1.getInput(),rbasicDP1.getOutput(),rbasicDP2.getOutput()),
//									cDPvl(libmstr+i_fcname+"v vs svml_"+i_fcname+"v", rbasicDPv1.getInput(),rbasicDPv1.getOutput(),rbasicDPv2.getOutput());
//			fcnComparison<float>	cSPl(libmstr+i_fcname+"f vs svml_"+i_fcname+"f" , rbasicSP1.getInput(),rbasicSP1.getOutput(),rbasicSP2.getOutput()),
//									cSPvl(libmstr+i_fcname+"fv vs svml_"+i_fcname+"fv",rbasicSPv1.getInput(),rbasicSPv1.getOutput(),rbasicSPv2.getOutput());
//
//			// Print stats
//			cDPl.printStats(true);
//			cDPvl.printStats(true);	
//			cSPl.printStats(true);
//			cSPvl.printStats(true);
//
//			// Write files
//			cDPl.writeFile(outDP);
//			cDPvl.writeFile(outDPv);
//			cSPl.writeFile(outSP);
//			cSPvl.writeFile(outSPv);
//		}
//	} 

	return 1;
}
