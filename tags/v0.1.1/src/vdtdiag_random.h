/**
 * This file contains the routines for the creation and 
 * i/o of random numbers.
 * 
 * Author Danilo Piparo
 * 
 **/

/* 
 * VDT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* FIXME:
 * Some work still to be done on the building side with cmake.
 * I don't want to keep everything in a file, but I want a lib.
 */

#ifndef _VDT_RANDOM_
#define _VDT_RANDOM_

#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <functional>
#include <inttypes.h>
#include <cmath>
#include "vdtcore_common.h"
#include "vdtdiag_filePersistence.h"


/**
 * Class that represents a floating point random numbers pool.
 * It has 2 constructors:
 * 1) specifying min and max number and the size of the collection.
 * 2) from ascii file
 * The objects are able to dump on disk in the form of ascii files with 5 lines header
 * the random numbers. The header is hardcoded!! No comments, nothing. Just 5 lines in 
 * the header. In order not to loose precision, the numbers are written in HEX.
 **/
template<class T>
class randomPool{

public:
	/// Initialise with min and max numbers as well as desired size of the pool.
	randomPool(const T min, const T max, const uint32_t size, const uint32_t seed=1): 
		m_min(min), 
		m_max(max), 
		m_size(size){

		// Allocate the engine with seed one. Always the same numbers.
		std::mt19937_64 mtwister_engine(seed); 

		// allocate the distribution
		// use C++11 long double to be able to generate whole double range
		// This generates pure uniform distribution, which may be not suitable for
		// longer ranges
		std::uniform_real_distribution<long double> uniform_dist(m_min, m_max);

		auto uniform_gen = std::bind(uniform_dist, mtwister_engine);
		
		// Fill the numbers
		m_numbers.reserve(size);
		for (uint32_t i = 0; i < m_size; ++i)
			m_numbers.push_back((T)uniform_gen());
	}

	//-----------------------------------------------------------------------------

	/// Initialise piecewise uniform distribution.
		//mode = 0 .. symetric <-10^maxPwr, -T_min> U <T_min, 10^maxPwr>
		//mode = 1 .. positive <T_min, 10^maxPwr>
		//piecewise is just for distinguishing the two constructors
	randomPool(const uint16_t mode, const int maxPwr, const uint32_t size_per_bin, bool piecewise, const uint32_t seed=1){

		std::cout<<"Piecewise\n";
		// handy values
		int minPwr = std::numeric_limits<T>::min_exponent10;
		int num_bins_positive = maxPwr - minPwr;

		m_min = mode ? (T)pow((T)10,minPwr) : -(T)pow((T)10,maxPwr);
		m_max = (T)pow((T)10,maxPwr);
		m_size = num_bins_positive * size_per_bin * (mode?2:1); 

		std::cout << minPwr << " " << maxPwr << " " << m_min << " " << m_max << " " << m_size;

		// Allocate the engine with seed one. Always the same numbers.
		std::mt19937_64 mtwister_engine(seed); 

		// allocate the distribution
		// use C++11 long double to be able to generate whole double range
		// This generates pure uniform distribution, which may be not suitable for
		// longer ranges
		std::uniform_real_distribution<long double>* uniform_dist;
		m_numbers.reserve(m_size);

		//fill bins
		for(int i=minPwr; i<maxPwr; i++){
			T cur_min = (T)pow((T)10,i);
			uniform_dist = new std::uniform_real_distribution<long double>(cur_min,cur_min*10);
			auto uniform_gen = std::bind(*uniform_dist, mtwister_engine);
			//fill numbers
			for (uint32_t j = 0; j < size_per_bin; j++){
				// positive branch
				m_numbers.push_back((T)uniform_gen());
				if(!mode)
					// negative branch
					m_numbers.push_back(-(T)uniform_gen());
			}
			delete uniform_dist;
		}
	}

	//-----------------------------------------------------------------------------

	/// Initialise with an ascii file. The numbers are in hex format.
	randomPool(const std::string input_filename){
		m_ifile_name=input_filename;
		std::ifstream ifile ( input_filename );
		std::string line;
		// skip the 5 header lines
		for (uint16_t i;i<5;++i)
			std::getline(ifile,line);
		// read from file
		fpFromHex<T> input_val;
		while(ifile >> input_val) {
			m_numbers.push_back(input_val.getValue());
		}
		// particular of file init!!!
		m_min=m_max=m_size=-1;
	}

	//-----------------------------------------------------------------------------

	/// Really nothing to do
	~randomPool(){}

	//-----------------------------------------------------------------------------

	/// Write numbers on disk in the form of an ascii file. The numbers are in HEX format.
	void writeFile(const std::string output_filename) const{
		const std::string preamble("VDT Random numbers file (the first 5 lines are the header)\n");
		std::ofstream ofile ( output_filename );
		// Copy the input file if the object was created from file
		if (m_size==-1 ){
			std::string line;
			std::ifstream ifile ( m_ifile_name );
			getline(ifile,line);
			ofile << "Dumped by an object initialised by " << m_ifile_name << " - "
					<< preamble;
			ofile << ifile.rdbuf() ;
		}
		else{ // Write an header and the numbers in the other case
			ofile << preamble;
			if (sizeof(T)==8) // some kind of RTTC
				ofile << "Double Precision\n";
			else
				ofile << "Single Precision\n";
			ofile << "Minimum Number = " << m_min << std::endl
			<< "Maximum Number = " << m_max << std::endl
			<< "Total numbers = " << m_size << std::endl;
			// Do not write dec, but HEX!
			for (uint32_t i=0;i<m_numbers.size();++i)
					ofile << fpToHex<T>(m_numbers[i]);
			ofile << std::setbase(10);
		}
	}

	//-----------------------------------------------------------------------------

	/// Return a vector of the numbers
	const std::vector<T>& getNumbers() const {return m_numbers;};

	//-----------------------------------------------------------------------------
	
	// Prints the random numbers on screen
	void print() const{
		uint32_t counter=1;
		std::cout << "Random Pool:\n";
		const uint32_t numbers_size = m_numbers.size();
		std::cout << std::setprecision(std::numeric_limits<T>::digits10);
		for(uint32_t i=0;i<numbers_size;++i)
			std::cout << counter++ << "/" << numbers_size << " - " << m_numbers[i] << std::endl;
	}

	//-----------------------------------------------------------------------------

private:
	long double m_min;
	long double m_max;
	int64_t m_size;
	std::string m_ifile_name;
	std::vector<T> m_numbers;
};

#endif
