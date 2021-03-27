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

message(STATUS "looking for libarchive-dev")

find_path( ARCHIVE_INCLUDE_DIR archive.h
  HINTS
  $ENV{ARCHIVEDIR}
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

find_path( ARCHIVE_STATIC_INCLUDE_DIR archive.h
  HINTS
  $ENV{ARCHIVEDIR}
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

#message( "ARCHIVE_INCLUDE_DIR is ${ARCHIVE_INCLUDE_DIR}" )

find_library( ARCHIVE_LIBRARY
  NAMES archive
  HINTS
  $ENV{ARCHIVEDIR}
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

#message( "ARCHIVE_LIBRARY is ${ARCHIVE_LIBRARY}" )

find_library( ARCHIVE_STATIC_LIBRARY
  NAMES libarchive.a
  HINTS
  $ENV{ARCHIVEDIR}
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

SET ( ARCHIVE_FOUND OFF )
IF ( ARCHIVE_INCLUDE_DIR )
    IF ( ARCHIVE_STATIC_INCLUDE_DIR )
        IF ( ARCHIVE_LIBRARY )
            IF ( ARCHIVE_STATIC_LIBRARY )
                SET ( ARCHIVE_FOUND "YES" )
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


if(ARCHIVE_FOUND)
    message(STATUS "looking for libarchive-dev - done")
else()
    message(FATAL_ERROR "ERROR: libarchive-dev not installed")
endif()
