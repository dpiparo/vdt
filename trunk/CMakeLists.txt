# VDT Math LIbrary
cmake_minimum_required (VERSION 2.6)
project (Vdt)

#-------------------------------------------------------------------------------
# Include the defaults
include ( CMakeDefaults.txt )
#-------------------------------------------------------------------------------

# configuration options -- you may change them when running cmake ==============
# with 'cmake -D <OPT>=<value> .'

option( DIAG "Build in diagnostic mode - all diagnostic exes (default cache entry: OFF)" OFF)
option( AVX "Use AVX instruction set (default cache entry: OFF)" OFF)
option( AVX2 "Use AVX2 instruction set (default cache entry: OFF)" OFF)
option( SSE "Use SSE instruction set (default cache entry: ON)" ON)
option( NEON "Use NEON instruction set (default cache entry: OFF)" OFF)
option( BUILD_SHARED_LIBS "Build libraries as SHARED instead of STATIC (default cache entry: ON)" ON)
option( USE_VC "Use Vc library - requires symlink to Vc from ${CMAKE_SOURCE_DIR} (default cache entry: OFF)" OFF)
option( DEBUG "Compile library with debug symbols (default is OFF)" OFF)

# determine compiler type and version ==========================================
set(COMPILER_IS_47_GCC false)
set(COMPILER_IS_OLD_GCC false)
set(COMPILER_IS_ICC false)

if(CMAKE_CXX_COMPILER MATCHES "/(icpc|icc)$")
  set(COMPILER_IS_ICC true)
  message(STATUS "Found icc")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  # check the GCC version (taken from Vc library)
   exec_program(${CMAKE_C_COMPILER} ARGS -dumpversion OUTPUT_VARIABLE _gcc_version)
   # some distributions patch their GCC to return nothing or only major and minor version on -dumpversion.
   # In that case we must extract the version number from --version.
   if(NOT _gcc_version OR _gcc_version MATCHES "^[0-9]\\.[0-9]+$")
      exec_program(${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE _gcc_version)
      string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" _gcc_version "${_gcc_version}")
      # message(STATUS "GCC Version from --version: ${_gcc_version}")
   endif()
  message(STATUS "Found gcc version ${_gcc_version}")
  
  if("${_gcc_version}" VERSION_LESS "4.7.0")
    set(COMPILER_IS_OLD_GCC true)
    message(STATUS "You are running gcc older than 4.7, turning off Vc, AVX and DIAG options")
    change_option(USE_VC 0)
    change_option(AVX 0)
    change_option(AVX2 0)
    change_option(DIAG 0)
  else()
    set(COMPILER_IS_47_GCC true)
  endif()
else()
  message("Unsupported compiler !!!")
endif()

# process options that can be processed here ===================================

# SIMD instructions set---------------------------------------------------------

if (NEON)
  message(STATUS "Using NEON instructions!")
  set(SSE_AVX "-mfpu=neon ")
else()
  if (SSE AND (NOT (AVX OR AVX2) ))
    message(STATUS "Using SSE instructions!")
    set(SSE_AVX "-msse")
  endif ()

  if (AVX AND (NOT AVX2))
    message(STATUS "Using AVX instructions!")
    set (SSE_AVX "-mavx")
    if(COMPILER_IS_ICC)
      set(SSE_AVX "-xavx")
    endif()
  endif ()

  if (AVX2)
    message(STATUS "Using AVX2 instructions!")
    set (SSE_AVX "-mavx2")
    if(COMPILER_IS_ICC)
      set(SSE_AVX "-xavx2")
    endif()
  endif ()
endif()

# To use svml at CERN ----------------------------------------------------------
set (INTEL_SVML_FLAGS "")
if (SVML)
  message (STATUS "Linking SVML library")
  set (INTEL_SVML_FLAGS "-mveclibabi=svml -L/afs/cern.ch/sw/IntelSoftware/linux/x86_64/Compiler/11.1/072/lib/intel64/ -lsvml -lirc")  
endif (SVML)

# Vc setup ---------------------------------------------------------------------

if(USE_VC)
  message(STATUS "VC usage is turned on now, if you do not intend to use it, run 'cmake -D USE_VC=0 .'")
  set (VC_SYMLINK_MSG "To use Vc you must have a (symlink) 'Vc' leading to the Vc rootdir in your ${CMAKE_SOURCE_DIR}")
  #check for files
  set (VC_LIB_NAME "${CMAKE_SOURCE_DIR}/Vc/libVc.a")
  set (VC_HEADER_NAME "${CMAKE_SOURCE_DIR}/Vc/include/Vc/Vc")

  if(NOT EXISTS ${VC_LIB_NAME}) 
    message(STATUS "Vc lib not found at ${VC_LIB_NAME}, turning off Vc usage")
    message(STATUS ${VC_SYMLINK_MSG})
    change_option(USE_VC 0)
  endif(NOT EXISTS ${VC_LIB_NAME})  
  
  if (EXISTS ${VC_LIB_NAME})
    if(NOT EXISTS ${VC_HEADER_NAME}) 
      message(STATUS "Vc header not found at ${VC_HEADER_NAME}, turning off Vc usage")
      message(STATUS ${VC_SYMLINK_MSG})
      change_option(USE_VC 0)
    endif(NOT EXISTS ${VC_HEADER_NAME}) 
  endif(EXISTS ${VC_LIB_NAME})

  
  link_directories( ${CMAKE_SOURCE_DIR}/Vc ) 
endif(USE_VC)

# set compiler options =========================================================

set (LIBTIMING "")
set (LIBTIMINGAPPLE "")
if(DIAG)
  # Library for time measurement: OSx and Linux
  set (LIBTIMING "rt")
  # do not set it if on OSx
  if (APPLE)
    set (LIBTIMINGAPPLE "-framework Carbon")
  endif (APPLE) 
endif(DIAG)

#-------------------------------------------------------------------------------
# Compiler optimisations

set (VECT_OPT "-O3 -ffast-math -ftree-vectorize")
set (VECTORIZER_VERBOSITY "-ftree-vectorizer-verbose=0")
if ("4.7.0" VERSION_LESS "${_gcc_version}")
 set (CPP11_OPT "-std=gnu++0x")
else()
 set (CPP11_OPT "-std=c++11")
endif()
set (INLINE_OPT " --param vect-max-version-for-alias-checks=50 --param inline-unit-growth=150")
set (VERBOSITY_OPT "-Winline ")

# compiler dependent changes ---------------------------------------------------
if(COMPILER_IS_ICC)
  set (VECT_OPT "-O2")
  set (VECTORIZER_VERBOSITY "")
  set (INLINE_OPT "")
elseif(COMPILER_IS_OLD_GCC)
  set (VECT_OPT "-O2")
  set (VECTORIZER_VERBOSITY "")
  set (INLINE_OPT "")
  set (VERBOSITY_OPT "")
  set (CPP11_OPT "")
endif()

set (WARNING_FLAGS "-W -Wall -Werror -Wno-error=unused-parameter")

set (DEBUG_FLAGS " ")
if (DEBUG)
  set (DEBUG_FLAGS " -g")
  message(STATUS "Adding debugging symbols")
endif ()


set (COMMON_FLAGS "${INTEL_SVML_FLAGS} ${SSE_AVX}  ${INLINE_OPT} ${WARNING_FLAGS} ${DEBUG_FLAGS}")
set (LIB_FLAGS "${VERBOSITY_OPT} ${VECT_OPT} ${VECTORIZER_VERBOSITY} ${COMMON_FLAGS}")
set (DIAG_FLAGS "${CPP11_OPT} ${COMMON_FLAGS} ${LIBTIMINGAPPLE} ${VECT_OPT}")

# Locations ====================================================================
# Location of executables
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Location of sources
set( SRC_DIR ${CMAKE_SOURCE_DIR}/src )

# Location of library
set( LIB_DIR ${CMAKE_SOURCE_DIR}/lib )

# Common Includes
set (INC_DIR ${CMAKE_SOURCE_DIR}/include )


#-------------------------------------------------------------------------------

add_subdirectory( src )
add_subdirectory( lib )
if (DIAG)
    message("DIAG option is now on, building diagnostic programs")
    add_subdirectory( progs )
    add_subdirectory( progs/units )
else(DIAG)
  message("DIAG option is now off, building library only")
endif(DIAG)

#-------------------------------------------------------------------------------
# Installation

# Install location
INSTALL(FILES
        include/asin.h
        include/atan.h
        include/atan2.h
        include/cos.h
        include/exp.h
        include/identity.h
        include/inv.h
        include/log.h
        include/sincos.h
        include/sin.h
        include/sqrt.h
        include/tan.h
        include/vdtcore_common.h
        include/vdtMath.h        
        DESTINATION include/vdt)

