# VDT Math LIbrary
cmake_minimum_required (VERSION 2.6)
project (Vdt)

#-------------------------------------------------------------------------------
# Include the defaults

include ( CMakeDefaults.txt )

#-------------------------------------------------------------------------------
add_subdirectory( src )
add_subdirectory( lib )
if (DIAG)
    add_subdirectory( progs )
    add_subdirectory( progs/units )
else(DIAG)
  message("DIAG option is now off, building library only")
endif(DIAG)