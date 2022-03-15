# Auto generate the code for the vector signatures and if needed preload

# We need Python for the following.
find_package (Python COMPONENTS Interpreter REQUIRED)

# Generate the code for the lib
if (PRELOAD)
set (SIGGENOPTS  " -p")
else()
set (SIGGENOPTS  " ")
endif()
EXEC_PROGRAM ("cd src;${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/signatures_generator.py ${SIGGENOPTS} -o ${CMAKE_SOURCE_DIR}/src;cd -")

#generare Vc wrapper and config file
if(USE_VC)
  EXEC_PROGRAM ("cd src;${Python_EXECUTABLE} vc_wrapper_generator.py;cd -")
endif(USE_VC)
configure_file( ${INC_DIR}/externalLibcfg.h.cmake ${INC_DIR}/externalLibcfg.h)
