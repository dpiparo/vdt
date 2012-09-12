# Define this directory's flags:
SET(CMAKE_CXX_FLAGS  ${DIAG_FLAGS})

# Benchmarks
add_exe_and_link ( vdtPerfBenchmark vdtPerfBenchmark.cpp ${LIBTIMING})
add_exe_and_link ( vdtArithmBenchmark vdtArithmBenchmark.cpp None)
add_exe_and_link ( vdtArithmComparison vdtArithmComparison.cpp None)
