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

message(STATUS "looking for libswscale-dev")

find_path( SWSCALE_INCLUDE_DIR libswscale/swscale.h
  HINTS
  $ENV{SWSCALEDIR}
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

find_path( SWSCALE_STATIC_INCLUDE_DIR libswscale/swscale.h
  HINTS
  $ENV{SWSCALEDIR}
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

#message( "SWSCALE_INCLUDE_DIR is ${SWSCALE_INCLUDE_DIR}" )

find_library( SWSCALE_LIBRARY
  NAMES swscale
  HINTS
  $ENV{SWSCALEDIR}
  PATH_SUFFIXES lib64 lib bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

#message( "SWSCALE_LIBRARY is ${SWSCALE_LIBRARY}" )

find_library( SWSCALE_STATIC_LIBRARY
  NAMES libswscale.a
  HINTS
  $ENV{SWSCALEDIR}
  PATH_SUFFIXES lib64 lib bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

SET ( SWSCALE_FOUND OFF )
IF ( SWSCALE_INCLUDE_DIR )
    IF ( SWSCALE_STATIC_INCLUDE_DIR )
        IF ( SWSCALE_LIBRARY )
            IF ( SWSCALE_STATIC_LIBRARY )
                SET ( SWSCALE_FOUND "YES" )
            ELSE ()
                message(STATUS "static library not found")
            ENDIF ()
        ELSE ()
            message(STATUS "dynamic library not found")
        ENDIF ()
    ELSE ()
        message(STATUS "include static dirs not found")
    ENDIF ()
ELSE ()
    message(STATUS "include dirs not found")
ENDIF ()


if(SWSCALE_FOUND)
    message(STATUS "looking for libswscale-dev - done")
else()
    message(FATAL_ERROR "ERROR: libswscale-dev not installed")
endif()
