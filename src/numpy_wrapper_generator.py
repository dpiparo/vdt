#! /usr/bin/python

"""
Generates numpy wrapper - both header and .cc file
"""

RESTRICT="__restrict__"
VDT_PREF="vdt_"


FUNCTIONS_LIST=["asin",
                "atan",
                "cos",
                "exp",
                "inv",
                "log",
                "sin",
                "isqrt",
                "identity"]

VDT_WRAPPER_HEADER='vdtdiag_numpyWrapper.h'
VDT_WRAPPER_IMPL='vdtdiag_numpyWrapper.cc'
VDT_PYTHON_MODULE='vdt.py'
                
#------------------------------------------------------------------

def get_type_dependent_parts(is_double, is_vector):
  suffix=""
  type="double"
  if(is_double):
    if(is_vector):
      suffix="v"
  else:
    type="float"
    suffix="f"
    if(is_vector):
      suffix="fv" 
      
  data_type="%s" %(type)
  if(is_vector):
    data_type="%s* %s" %(type, RESTRICT)
  return (type, data_type, suffix)
  
#------------------------------------------------------------------
  
def get_function_prototype(fcn_name,is_double,is_vector):
  (type,data_type,suffix)=get_type_dependent_parts(is_double,is_vector)
  prototype="%s%s%s(%s x)" %(VDT_PREF,fcn_name,suffix,data_type)
  ret = type
  if(is_vector):
    prototype="%s%s%s(const %s iarray, %s oarray, long size)" %(VDT_PREF,fcn_name,suffix,data_type,data_type)
    ret = 'void'
  return (ret,prototype)  

#-------------------------------------------------------------------

# translation of raw name withnout suffixes
def get_fcnname_translation(fcn_name):
  if(fcn_name == "inv"):
    _name = "reciprocal"
  elif(fcn_name == "isqrt"):
    _name = "rsqrt"
  else:
    _name = fcn_name
  return _name
  
#-------------------------------------------------------------------

def get_function_code(fcn_name,is_vector):
  if(is_vector):
    code = "{" +\
           "for (long i=0;i<size;++i) {oarray[i]=vdt::fast_"+fcn_name+"(iarray[i]);};"\
           +"}"
  else:
    code = "{return vdt::fast_"+fcn_name+"(x);}"

  
  return code

def get_return_code(fcn_name,is_double,is_vector):
  (type,data_type,suffix)=get_type_dependent_parts(is_double,is_vector)
  code = ''
  if(is_vector):
    code = 'fat_vdt_'+fcn_name+suffix+'(iarray,oarray,size);'
  else:
    code = 'return fat_vdt_'+fcn_name+suffix+'(x);'
  return code

#---------------------------------------------------------------------
                
def create_Wrapper_signature(fcn_name,is_double=False,is_vector=False,is_impl=False):
  (ret,sign) = get_function_prototype(fcn_name,is_double,is_vector)
  code = ''
  ss=''
  if(not is_double): ss='f'
  if (is_vector) : ss+='v'
  fatM = 'FAT'+fcn_name+ss
  if is_impl:
    code += '#define '+fatM + ' fat_'+ sign + get_function_code(fcn_name,is_vector)
    code += '\nnamespace {FATLIB('+ret+','+fatM+')}\n'+\
            'extern "C"{'+ret + ' ' + sign+'{'+get_return_code(fcn_name,is_double,is_vector)+'}}\n\n'
  else:
    code = ret + ' ' + sign +";\n"
  return code
		 
#------------------------------------------------------------------
		 
def create_Wrapper_signatures(is_impl=False):
  code=""
  for is_vector in (False,True):
    for is_double in (True,False):
      for fcn_name in sorted(FUNCTIONS_LIST):
        code+=create_Wrapper_signature(fcn_name,is_double,is_vector,is_impl)
  return code


#------------------------------------------------------------------   

def get_python_function(fcn_name,is_double,is_vector):
  # only vector
  if not is_double : fcn_name+='f'
  code = ''
  if(is_vector):
    fn ='%s%s' %(VDT_PREF,fcn_name)
    code='def %s(vi): return vdt_invoke("%s",vi)' %(fn,fn) + '\n'
  return code

#------------------------------------------------------------------
  

def create_python():
  code="from vdtBase import *\n"
  for is_vector in (False,True):
    for is_double in (True,False):
      for fcn_name in sorted(FUNCTIONS_LIST):
        code+=get_python_function(fcn_name,is_double,is_vector)

  ofile=file(VDT_PYTHON_MODULE,'w')
  ofile.write(code)
  ofile.close()

#------------------------------------------------------------------   

def get_header_file():
  code= "// Automatically generated\n"\
        '#ifndef __VDT_NUMPY_WRAPPER__\n'\
        '#define __VDT_NUMPY_WRAPPER__\n'\
        'extern "C"{\n'+\
        create_Wrapper_signatures(is_impl=False)+\
        '}\n'+\
        '\n'+\
        '#endif\n'
  return code

#------------------------------------------------------------------   
		   
def get_impl_file():
  code= "// Automatically generated\n"+\
        '#include "%s"\n' %VDT_WRAPPER_HEADER+\
        '#include "vdtMath.h"\n'+\
        '#include "fatlib.h"\n\n'+\
        create_Wrapper_signatures(is_impl=True)
  return code
		  
#------------------------------------------------------------------

def create_header():
  ofile=file(VDT_WRAPPER_HEADER,'w')
  ofile.write(get_header_file())
  ofile.close()
  
#------------------------------------------------------------------

def create_impl():
  ofile=file(VDT_WRAPPER_IMPL,'w')
  ofile.write(get_impl_file())
  ofile.close()

#------------------------------------------------------------------
  
if __name__ == "__main__":
  create_header()
  create_impl()
  create_python()
