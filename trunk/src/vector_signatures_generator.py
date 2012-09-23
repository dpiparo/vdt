#! /usr/bin/python

"""
Generate the .cc file with the signatures
of the vector functions.
"""

RESTRICT="__restrict__"

FUNCTIONS_LIST=["asin",
                "atan",
                "acos",
                "cos",
                "exp",
                "inv",
                "log",
                "sin",
                "isqrt",
                "tan",
                "identity",
                "fast_asin",
                "fast_acos",
                "fast_atan",
                "fast_cos",
                "fast_exp",
                "fast_inv",
                "fast_approx_inv",
                "fast_log",
                "fast_sin",
                "fast_isqrt",
                "fast_approx_isqrt",
                "fast_tan"]

VDT_VECTOR_HEADER='vdtMath.h'
VDT_VECTOR_IMPL='vdtMath_vector.cc'
                
#------------------------------------------------------------------
                
def create_vector_signature(fcn_name,is_double=False,is_impl=False):
  # For single precision
  suffix="fv"
  float_suffix="f"
  type="float"
  if is_double:
    suffix="v"
    type="double"
    float_suffix=""
  prefix=""
  vfcn_name="%s%s" %(fcn_name,suffix)
  data_type="%s* %s" %(type, RESTRICT)
  new_fcn_name="%s%s" %(prefix,fcn_name)
  code =  "void %s%s(const uint32_t size, %s iarray, %s oarray)" %(new_fcn_name,suffix,data_type,data_type)
  if is_impl:
    code += "{\n"+\
          "  for (uint32_t i=0;i<size;++i)\n"+\
		  "    oarray[i]=%s%s(iarray[i]);\n" %(new_fcn_name,float_suffix)+\
          "}\n\n"
  else:
	code += ";\n"	  
  return code
		 
#------------------------------------------------------------------
		 
def create_vector_signatures(is_impl=False):
  code="namespace vdt{\n"
  for is_double in (True,False):
    for fcn_name in sorted(FUNCTIONS_LIST):
      code+=create_vector_signature(fcn_name,is_double,is_impl)
  code += "} // end of vdt namespace"
  return code


#------------------------------------------------------------------   
   
def get_header_file():
  code= "// Automatically generated\n"\
        '#ifndef __VDT_VECTOR__\n'\
        '#define __VDT_VECTOR__\n'\
        '#include "%"\n' %VDT_VECTOR_HEADER+\
        '#include "inttypes.h"\n'+\
        create_vector_signatures(is_impl=False)+\
        '\n'\
        '#endif'
  return code

#------------------------------------------------------------------   
		   
def get_impl_file():
  code= "// Automatically generated\n"+\
        '#include "%s"\n' %VDT_VECTOR_HEADER+\
        create_vector_signatures(is_impl=True)       
        
  return code
		  
#------------------------------------------------------------------

def create_header():
  ofile=file(VDT_VECTOR_HEADER,'w')
  ofile.write(get_header_file())
  ofile.close()
  
#------------------------------------------------------------------

def create_impl():
  ofile=file(VDT_VECTOR_IMPL,'w')
  ofile.write(get_impl_file())
  ofile.close()

#------------------------------------------------------------------
  
if __name__ == "__main__":
  #create_header()
  create_impl()
