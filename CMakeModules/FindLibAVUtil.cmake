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

message(STATUS "looking for libavutil-dev")

find_path( AVUTIL_INCLUDE_DIR libavutil/avutil.h
  HINTS
  $ENV{AVUTILDIR}
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

find_path( AVUTIL_STATIC_INCLUDE_DIR libavutil/avutil.h
  HINTS
  $ENV{AVUTILDIR}
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

#message( "AVUTIL_INCLUDE_DIR is ${AVUTIL_INCLUDE_DIR}" )

find_library( AVUTIL_LIBRARY
  NAMES avutil
  HINTS
  $ENV{AVUTILDIR}
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

#message( "AVUTIL_LIBRARY is ${AVUTIL_LIBRARY}" )

find_library( AVUTIL_STATIC_LIBRARY
  NAMES libavutil.a
  HINTS
  $ENV{AVUTILDIR}
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

SET ( AVUTIL_FOUND OFF )
IF ( AVUTIL_INCLUDE_DIR )
    IF ( AVUTIL_STATIC_INCLUDE_DIR )
        IF ( AVUTIL_LIBRARY )
            IF ( AVUTIL_STATIC_LIBRARY )
                SET ( AVUTIL_FOUND "YES" )
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


if(AVUTIL_FOUND)
    message(STATUS "looking for libavutil-dev - done")
else()
    message(FATAL_ERROR "ERROR: libavutil-dev not installed")
endif()
