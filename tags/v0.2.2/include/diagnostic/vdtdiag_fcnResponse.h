/**
 * This file contains the classes to store the 
 * arithmetical performance of the mathematical functions.
 * 
 * Author Danilo Piparo
 * 
 **/


#include <string>
#include <functional>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include "inttypes.h"
#include "vdtcore_common.h"
#include "vdtdiag_filePersistence.h"


/**
 * Class that represents the response of a mathematical function.
 * The quantities stored are the input numbers and the output numbers.
 * The Ascii file i/o is supported. A dump on an ascii file can be performed
 * as well as the object construction from ascii file.
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


template<class T>
class fcnResponse{
public:
	
	//-----------------------------------------------------------------------------
	
	/// Construct from name, input and scalar function
	fcnResponse(const std::string& fcnName, const std::vector<T>& input, const std::function<T(T)>& fcn):
		m_from_file(false),
		m_fcn_name(fcnName),
		m_fcn(fcn),
		m_input(input){
		
		m_output.reserve(m_input.size());
		for (auto& inp : m_input )
			m_output.push_back(fcn(inp));
	}
	
	//-----------------------------------------------------------------------------
	
	/// Construct from name, input and vector function
	fcnResponse(const std::string& fcnName, const std::vector<T>& input, const std::function<void (uint32_t, T*, T*)>& fcnv):
		m_from_file(false),
		m_fcn_name(fcnName),
		m_ifile_name("From Scratch"),
		m_fcnv(fcnv),
		m_input(input){
		const uint32_t size=m_input.size();
		m_output.reserve(size);
                T* input_arr = const_cast<T*> (&m_input[0]);
		T* output_arr = new T[size];
		fcnv(size,input_arr,output_arr);
		for (uint32_t i=0;i<size;++i )
			m_output.push_back(output_arr[i]);
		delete [] output_arr;
	}
	//-----------------------------------------------------------------------------
		
	/// Construct from ascii file
	fcnResponse(const std::string& input_filename):
		m_from_file(true),
		m_fcn_name(std::string("From ")+input_filename),
		m_ifile_name(input_filename){
		
			std::ifstream ifile ( input_filename );
			std::string line;
			//skip the 5 header lines
			for (uint16_t i=0;i<5;++i)
				std::getline(ifile,line);
			//read data from file	
			fpFromHex<T> in_val, out_val;
			while(ifile >> in_val >> out_val) {
				m_input.push_back(in_val.getValue());
				m_output.push_back(out_val.getValue());
			}
	}
	
	//-----------------------------------------------------------------------------
	
	/// Nothing to do
	~fcnResponse(){};
	
    //-----------------------------------------------------------------------------
        
    /// Return the output
    const std::vector<T>& getOutput() const {return m_output;};

	/// Return the input
    const std::vector<T>& getInput() const {return m_input;};
        
	//-----------------------------------------------------------------------------
	
	/// Dump on ascii file
	void writeFile(const std::string& output_filename){
		const std::string preamble("VDT function arithmetics performance file (the first 5 lines are the header)\n");
		std::ofstream ofile ( output_filename );
		// Copy the input file if the object was created from file
		if (m_from_file){
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
			ofile << "Function Name = " << m_fcn_name << std::endl
					<< "--\n--\n";
			//ofile << std::setprecision(std::numeric_limits<T>::digits10);
			// Do not write dec, but HEX!
			for (uint32_t i=0;i<m_input.size();++i)
				ofile << fpToHex<T>(m_input[i]) << fpToHex<T>(m_output[i]) << std::endl;
		}
	}

	//-----------------------------------------------------------------------------
	
	/// Print to screen
	void print(){
		uint32_t counter=0;
		const uint32_t size=m_input.size();
		std::cout << "Function Performance:\n";
		for (uint32_t i=0;i<size;++i){
			std::cout << std::setprecision(std::numeric_limits<T>::digits10);
			std::cout <<  counter++ << "/" << size << " " << m_fcn_name 
					<< "(" << m_input[i] << ") = " << m_output[i] << std::endl;
		}
	}
	
private:
	const bool m_from_file;
	const std::string m_fcn_name;
	const std::string m_ifile_name;
	const std::function<T(T)> m_fcn;
	const std::function<void(const uint32_t, T*, T*)> m_fcnv;
	std::vector<T> m_input;
	std::vector<T> m_output;
		
};


