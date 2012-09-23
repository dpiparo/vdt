# Rules for making the so library with the vector signatures

# Define this directory's flags:
SET(CMAKE_CXX_FLAGS  ${LIB_FLAGS})

include_directories ( ${CMAKE_SOURCE_DIR}/Include )

if(BUILD_SHARED_LIBS)
  message(STATUS "Libraries are configured as: SHARED")
else(BUILD_SHARED_LIBS)
  message(STATUS "Libraries are configured as: STATIC")
endif(BUILD_SHARED_LIBS)

# The library
ADD_LIBRARY(vdt ${SRC_DIR}/vdtMath_vector.cc ${INC_DIR}/vdtMath.h )

# Build Vc wrapper (without c++11)
if(USE_VC)
    SET(CMAKE_CXX_FLAGS  "${LIB_FLAGS} -fabi-version=6")
    include_directories( ${CMAKE_SOURCE_DIR}/Vc  ${CMAKE_SOURCE_DIR}/Vc/include )
    add_library(VcWrapper ${SRC_DIR}/vdtdiag_vcWrapper.cc ${SRC_DIR}/vdtdiag_vcWrapper.h)
    target_link_libraries(VcWrapper libVc.a)
endif(USE_VC)