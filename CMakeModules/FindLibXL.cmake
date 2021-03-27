# Locate XL library
# This module defines
# XL_LIBRARY, the name of the library to link against
# XL_FOUND, if false, do not try to link to XL
# XL_INCLUDE_DIR, where to find libxl.h
#

message(STATUS "looking for libxl-dev")

set( XL_FOUND OFF )

find_path( XL_INCLUDE_DIR libxl.h
  HINTS
  $ENV{XLDIR}
  PATH_SUFFIXES include 
  PATHS
  ${LIBXL_PROJECT_DIR}/include_cpp
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

# message( "XL_INCLUDE_DIR is ${XL_INCLUDE_DIR}" )


find_library( XL_LIBRARY_64
  NAMES xl
  HINTS
  ${LIBXL_INSTALL_DIR}
  ${LIBXL_PROJECT_DIR}
  $ENV{XLDIR}
  PATH_SUFFIXES lib64 bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

find_library( XL_LIBRARY_32
  NAMES xl
  HINTS
  ${LIBXL_INSTALL_DIR}
  ${LIBXL_PROJECT_DIR}
  $ENV{XLDIR}
  PATH_SUFFIXES lib bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

# message( "XL_LIBRARY is ${XL_LIBRARY}" )

if(XL_INCLUDE_DIR)
    if(XL_LIBRARY_32)
        if(XL_LIBRARY_64)
	    set( XL_FOUND "YES" )
	endif()
    endif()
endif()


if(XL_FOUND)
    EXECUTE_PROCESS( COMMAND getconf LONG_BIT COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

    if( ${ARCHITECTURE} STREQUAL "64" )
	set( XL_LIBRARY ${XL_LIBRARY_64} )
    else()
	set( XL_LIBRARY ${XL_LIBRARY_32} )
    endif()

    string(REGEX REPLACE "([^/]+$)" "" XL_LIBRARY_DIR ${XL_LIBRARY})
    # message( "XL_LIBRARY_DIR is ${XL_LIBRARY_DIR}" )
    message(STATUS "looking for libxl-dev - done")
else()
    message(FATAL_ERROR "ERROR: libxl-dev not installed")
endif()
