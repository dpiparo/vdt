# Auto generate the code for the vector signatures

# Generate the code for the lib
EXEC_PROGRAM ("cd src;python vector_signatures_generator.py;cd -")

#generare Vc wrapper and config file
if(USE_VC)
  EXEC_PROGRAM ("cd src;python vc_wrapper_generator.py;cd -")
endif(USE_VC)
configure_file( ${INC_DIR}/externalLibcfg.h.cmake ${INC_DIR}/externalLibcfg.h)
