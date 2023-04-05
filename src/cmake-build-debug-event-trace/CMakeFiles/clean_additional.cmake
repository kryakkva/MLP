# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MLP_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MLP_autogen.dir/ParseCache.txt"
  "MLP_autogen"
  )
endif()
