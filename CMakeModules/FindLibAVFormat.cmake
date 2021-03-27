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

message(STATUS "looking for libavformat-dev")

find_path( AVFORMAT_INCLUDE_DIR libavformat/avformat.h
  HINTS
  $ENV{AVFORMATDIR}
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

find_path( AVFORMAT_STATIC_INCLUDE_DIR libavformat/avformat.h
  HINTS
  $ENV{AVFORMATDIR}
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

#message( "AVFORMAT_INCLUDE_DIR is ${AVFORMAT_INCLUDE_DIR}" )

find_library( AVFORMAT_LIBRARY
  NAMES avformat
  HINTS
  $ENV{AVFORMATDIR}
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

#message( "AVFORMAT_LIBRARY is ${AVFORMAT_LIBRARY}" )

find_library( AVFORMAT_STATIC_LIBRARY
  NAMES libavformat.a
  HINTS
  $ENV{AVFORMATDIR}
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

SET ( AVFORMAT_FOUND OFF )
IF ( AVFORMAT_INCLUDE_DIR )
    IF ( AVFORMAT_STATIC_INCLUDE_DIR )
        IF ( AVFORMAT_LIBRARY )
            IF ( AVFORMAT_STATIC_LIBRARY )
                SET ( AVFORMAT_FOUND "YES" )
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


if(AVFORMAT_FOUND)
    message(STATUS "looking for libavformat-dev - done")
else()
    message(FATAL_ERROR "ERROR: libavformat-dev not installed")
endif()
