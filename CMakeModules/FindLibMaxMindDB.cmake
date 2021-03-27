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

message(STATUS "looking for libmaxminddb-dev")

find_path( MAXMINDDB_INCLUDE_DIR maxminddb.h
  HINTS
  $ENV{MAXMINDDBDIR}
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

find_path( MAXMINDDB_STATIC_INCLUDE_DIR maxminddb.h
  HINTS
  $ENV{MAXMINDDBDIR}
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

#message( "MAXMINDDB_INCLUDE_DIR is ${MAXMINDDB_INCLUDE_DIR}" )

find_library( MAXMINDDB_LIBRARY
  NAMES maxminddb
  HINTS
  $ENV{MAXMINDDBDIR}
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

#message( "MAXMINDDB_LIBRARY is ${MAXMINDDB_LIBRARY}" )

find_library( MAXMINDDB_STATIC_LIBRARY
  NAMES libmaxminddb.a
  HINTS
  $ENV{MAXMINDDBDIR}
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

SET ( MAXMINDDB_FOUND OFF )
IF ( MAXMINDDB_INCLUDE_DIR )
    IF ( MAXMINDDB_STATIC_INCLUDE_DIR )
        IF ( MAXMINDDB_LIBRARY )
            IF ( MAXMINDDB_STATIC_LIBRARY )
                SET ( MAXMINDDB_FOUND "YES" )
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


if(MAXMINDDB_FOUND)
    message(STATUS "looking for libmaxminddb-dev - done")
else()
    message(FATAL_ERROR "ERROR: libmaxminddb-dev not installed")
endif()
