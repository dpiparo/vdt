# VDT Math Library
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project (Vdt VERSION 0.4.4)

#-------------------------------------------------------------------------------
# Include the defaults
include ( CMakeDefaults.txt )
#-------------------------------------------------------------------------------

# configuration options -- you may change them when running cmake ==============
# with 'cmake -D <OPT>=<value> .'

option( DIAG "Build in diagnostic mode - all diagnostic exes (default cache entry: OFF)" OFF)
option( AVX "Use AVX instruction set (default cache entry: OFF)" OFF)
option( AVX2 "Use AVX2 instruction set (default cache entry: OFF)" OFF)
option( FMA "Use FMA instruction set (default cache entry: OFF)" OFF)
option( USERFLAGS "Pass arbitrary flags to the compiler")
option( SSE "Use SSE instruction set (default cache entry: ON)" ON)
option( NEON "Use NEON instruction set (default cache entry: OFF)" OFF)
option( BUILD_SHARED_LIBS "Build libraries as SHARED instead of STATIC (default cache entry: ON)" ON)
option( PRELOAD "Create in the library the symbols to preload the library (default cache entry: OFF)" OFF)
option( USE_VC "Use Vc library - requires symlink to Vc from ${CMAKE_SOURCE_DIR} (default cache entry: OFF)" OFF)
option( DEBUG "Compile library with debug symbols (default is OFF)" OFF)

message(${CMAKE_CXX_COMPILER_ID})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "VDT requires GCC version >= 4.8")
    set(COMP_IS_GCC TRUE)
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    message(FATAL_ERROR "VDT requires AppleClang version >= 5.0")
  endif()
  set(COMP_IS_CLANG TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
    message(FATAL_ERROR "VDT requires Clang version >= 3.3")
  endif()
  set(COMP_IS_CLANG TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0)
    message(FATAL_ERROR "VDT requires ICC >= 15.0")
    set(COMP_IS_ICC TRUE)
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang, ICC and GCC.")
endif()

# SIMD and FMA instructions set-------------------------------------------------
if (NEON)
  message(STATUS "Using NEON instructions!")
  set(PACKED_INSTR "-mfpu=neon ")
else()
  if (SSE AND (NOT (AVX OR AVX2) ))
    message(STATUS "Using SSE instructions!")
    set(PACKED_INSTR "-msse")
  endif ()

  if (AVX AND (NOT AVX2))
    message(STATUS "Using AVX instructions!")
    set (PACKED_INSTR "-mavx")
    if(CMAKE_COMPILER_IS_ICC)
      set(PACKED_INSTR "-xavx")
    endif()
  endif ()

  if (AVX2)
    message(STATUS "Using AVX2 instructions!")
    set (PACKED_INSTR "-mavx2")
    if(CMAKE_COMPILER_IS_ICC)
      set(PACKED_INSTR "-xavx2")
    endif()
  endif ()

  if (FMA)
    message(STATUS "Using FMA instructions!")
    set (FMA_INSTR "-mfma")
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
if(DIAG)
  # Library for time measurement: macOS and Linux
  set (LIBTIMING "rt")
  # do not set it if on macOS
  if (APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
    set (LIBTIMINGAPPLE "-framework Carbon")
  endif ()
endif(DIAG)

#-------------------------------------------------------------------------------
# Compiler optimisations

set (VECT_OPT "-Ofast")
if (CMAKE_COMPILER_IS_ICC)
  set (VECT_OPT "")
endif()

if (${COMP_IS_GCC})
   set (VECTORIZER_VERBOSITY "-ftree-vectorizer-verbose=0")
   set (INLINE_OPT " --param vect-max-version-for-alias-checks=50 --param inline-unit-growth=150")   
endif()   

set (CPP11_OPT "-std=c++11")
set (VERBOSITY_OPT "-Winline")

# set it for clang until it understands __always_inline
set (CLANG_INLINE_DEFINE "")
if (${COMP_IS_CLANG})
 set (CLANG_INLINE_DEFINE "-D__extern_always_inline=inline")
endif()

# compiler dependent changes ---------------------------------------------------
if(${COMP_IS_ICC})
  set (VECTORIZER_VERBOSITY "")
  set (INLINE_OPT "")
endif()

set (WARNING_FLAGS "-W -Wall -Werror -Wno-error=unused-parameter")

if (DEBUG)
  set (DEBUG_FLAGS " -g")
  message(STATUS "Adding debugging symbols")
endif ()


set (COMMON_FLAGS "${CPP11_OPT} ${INTEL_SVML_FLAGS} ${PACKED_INSTR} ${FMA_INSTR} ${INLINE_OPT} ${WARNING_FLAGS} ${DEBUG_FLAGS} ${CLANG_INLINE_DEFINE}")
if (USERFLAGS)
  set (COMMON_FLAGS "${COMMON_FLAGS} ${USERFLAGS}")
endif()
set (LIB_FLAGS "${VERBOSITY_OPT} ${VECT_OPT} ${VECTORIZER_VERBOSITY} ${COMMON_FLAGS}")
set (DIAG_FLAGS "${LIBTIMINGAPPLE} ${VECT_OPT} ${COMMON_FLAGS}")

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
        include/tanh.h
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

