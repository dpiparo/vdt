/**
 * This file contains the abstract interfaces for the diagnostic classes
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

#include <random>
#include <string>
#include <vector>
#include <functional>

#ifndef _VDT_INTERFACES_
#define _VDT_INTERFACES_

/**
 *  Abstract interface for classes that can be printed on screen and written from a file.
 **/
class Iprintable{
public:
    Iprintable(){};
    virtual ~Iprintable(){};
	virtual void writeFile(const std::string& output_filename) =0;
	virtual void print() =0;
};

template<typename T>
class IfcnResponse:Iprintable{
public:
	IfcnResponse(const std::string& fcnName, std::vector<T> input):
		m_input1(input),
		m_from_file(false),
		m_fcn_name(fcnName),
		m_ifile_name("From scratch"){m_output.reserve(input.size());};
//-----------------------------------------------------------------------------
	IfcnResponse(const std::string& fcnName, std::vector<T> input1, std::vector<T> input2):
		m_input1(input1),
		m_input2(input2),
		m_from_file(false),
		m_fcn_name(fcnName),
		m_ifile_name("From scratch"){m_output.reserve(input1.size());};
//-----------------------------------------------------------------------------
	/// Construct from ascii file
	IfcnResponse(const std::string& input_filename):
		m_from_file(true),
		m_fcn_name(std::string("From ")+input_filename),
		m_ifile_name(input_filename){};
//-----------------------------------------------------------------------------
	~IfcnResponse(){};
//-----------------------------------------------------------------------------
	const std::string& getFcnName() const {return m_fcn_name;}
//-----------------------------------------------------------------------------
	const std::string& getIfileName() const {return m_ifile_name;}
//-----------------------------------------------------------------------------
	bool isFromFile() const {return m_from_file;}
//-----------------------------------------------------------------------------
   inline std::vector<T>& getOutput()  {return m_output;};
   void pushOutputVal(T value) {m_output.push_back(value);};
   inline const T outputVal(uint64_t index) const {return m_output[index];};
//-----------------------------------------------------------------------------
   inline std::vector<T>& getInput1()  {return m_input1;};
   void pushInput1Val(T value) {m_input1.push_back(value);};
   inline const T input1Val(uint64_t index) const {return m_input1[index];};
//-----------------------------------------------------------------------------
   inline std::vector<T>& getInput2()  {return m_input2;};
   void pushInput2Val(T value) {m_input2.push_back(value);};
   inline const T input2Val(uint64_t index) const {return m_input2[index];};
private:
	std::vector<T> m_output;
    std::vector<T> m_input1;
	std::vector<T> m_input2;
	const bool m_from_file;
	const std::string m_fcn_name;
	const std::string m_ifile_name;
};


class IrandomPool:public Iprintable{
public:
	IrandomPool(const uint64_t size, const int32_t seed=1):
		m_size(size),
		m_mtwister_engine(seed),
		m_ifile_name(""){};
	IrandomPool(const std::string& input_filename):
		m_size(0),
		m_mtwister_engine(0),
		m_ifile_name(input_filename){};
	~IrandomPool(){};
	uint64_t getSize() const {return m_size;};
	const std::string& getFileName() const {return m_ifile_name;};
protected:
	template <typename T> void fillVector(std::vector<T>& randomv,T min, T max){
		// allocate the distribution
		// use C++11 long double to be able to generate whole double range
		// This generates pure uniform distribution, which may be not suitable for
		// longer ranges
		std::uniform_real_distribution<long double> uniform_dist(min, max);
		auto uniform_gen = std::bind(uniform_dist, m_mtwister_engine);
		// Fill the numbers
		randomv.reserve(m_size);
		for (uint64_t i = 0; i < m_size; ++i)
			randomv.push_back((T)uniform_gen());
		}
private:
	const uint64_t m_size;
	std::mt19937_64 m_mtwister_engine;
	const std::string m_ifile_name;
};


#endif
