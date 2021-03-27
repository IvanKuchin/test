# This module tries to find libraries and include files
#
# <PREFIX>_FOUND                    do not use library, if false
# <PREFIX>_INCLUDE_DIRS             path where to find headers
# <PREFIX>_LIBRARY_DIRS             path where to find dynamic lib
# <PREFIX>_LIBRARIES                dynamic libraries to link
# <PREFIX>_STATIC_INCLUDE_DIRS      path where to find static headers
# <PREFIX>_STATIC_LIBRARY_DIRS      path where to find static lib
# <PREFIX>_STATIC_LIBRARIES         static libraries to link
#
# This works only on Linux

message(STATUS "looking for rapidjson-dev")

set( RAPIDJSON_FOUND OFF )

find_path( RAPIDJSON_INCLUDE_DIR rapidjson/rapidjson.h
  HINTS
  $ENV{RAPIDJSONDIR}
  PATH_SUFFIXES include 
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include 
  /opt/include
  /mingw/include
)

#message( "RAPIDJSON_INCLUDE_DIR is ${RAPIDJSON_INCLUDE_DIR}" )

if(RAPIDJSON_INCLUDE_DIR)
	set( RAPIDJSON_FOUND "YES" )
endif()


if(RAPIDJSON_FOUND)
    message(STATUS "looking for rapidjson-dev - done")
else()
    message(FATAL_ERROR "ERROR: rapidjson-dev not installed")
endif()
