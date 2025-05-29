# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appfile_System_Ex_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appfile_System_Ex_autogen.dir/ParseCache.txt"
  "appfile_System_Ex_autogen"
  )
endif()
