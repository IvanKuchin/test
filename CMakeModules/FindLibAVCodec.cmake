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

message(STATUS "looking for libavcodec-dev")

find_path( AVCODEC_INCLUDE_DIR libavcodec/avcodec.h
  HINTS
  $ENV{AVCODECDIR}
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

find_path( AVCODEC_STATIC_INCLUDE_DIR libavcodec/avcodec.h
  HINTS
  $ENV{AVCODECDIR}
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

#message( "AVCODEC_INCLUDE_DIR is ${AVCODEC_INCLUDE_DIR}" )

find_library( AVCODEC_LIBRARY
  NAMES avcodec
  HINTS
  $ENV{AVCODECDIR}
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

#message( "AVCODEC_LIBRARY is ${AVCODEC_LIBRARY}" )

find_library( AVCODEC_STATIC_LIBRARY
  NAMES libavcodec.a
  HINTS
  $ENV{AVCODECDIR}
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

SET ( AVCODEC_FOUND OFF )
IF ( AVCODEC_INCLUDE_DIR )
    IF ( AVCODEC_STATIC_INCLUDE_DIR )
        IF ( AVCODEC_LIBRARY )
            IF ( AVCODEC_STATIC_LIBRARY )
                SET ( AVCODEC_FOUND "YES" )
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


if(AVCODEC_FOUND)
    message(STATUS "looking for libavcodec-dev - done")
else()
    message(FATAL_ERROR "ERROR: libavcodec-dev not installed")
endif()
