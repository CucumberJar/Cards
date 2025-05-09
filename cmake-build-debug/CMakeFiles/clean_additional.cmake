# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Cards_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Cards_autogen.dir\\ParseCache.txt"
  "Cards_autogen"
  )
endif()
