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

message(STATUS "looking for libwkhtmltox-dev")

find_path( WKHTMLTOX_INCLUDE_DIR wkhtmltox/pdf.h
  HINTS
  $ENV{WKHTMLTOXDIR}
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

find_path( WKHTMLTOX_STATIC_INCLUDE_DIR wkhtmltox/pdf.h
  HINTS
  $ENV{WKHTMLTOXDIR}
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

#message( "WKHTMLTOX_INCLUDE_DIR is ${WKHTMLTOX_INCLUDE_DIR}" )

find_library( WKHTMLTOX_LIBRARY
  NAMES wkhtmltox
  HINTS
  $ENV{WKHTMLTOXDIR}
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

#message( "WKHTMLTOX_LIBRARY is ${WKHTMLTOX_LIBRARY}" )

#find_library( WKHTMLTOX_STATIC_LIBRARY
#  NAMES libwkhtmltox.a
#  HINTS
#  $ENV{WKHTMLTOXDIR}
#  PATH_SUFFIXES lib64 lib bin
#  PATHS
#  /usr/local
#  /usr
#  /sw
#  /opt/local
#  /opt/csw
#  /opt
#  /mingw
#)

SET ( WKHTMLTOX_FOUND OFF )
IF ( WKHTMLTOX_INCLUDE_DIR )
    IF ( WKHTMLTOX_STATIC_INCLUDE_DIR )
        IF ( WKHTMLTOX_LIBRARY )
#            IF ( WKHTMLTOX_STATIC_LIBRARY )
                SET ( WKHTMLTOX_FOUND "YES" )
#            ELSE ()
#                message(STATUS "static library not found")
#            ENDIF ()
        ELSE ()
            message(STATUS "dynamic library not found")
        ENDIF ()
    ELSE ()
        message(STATUS "include static dirs not found")
    ENDIF ()
ELSE ()
    message(STATUS "include dirs not found")
ENDIF ()


if(WKHTMLTOX_FOUND)
    message(STATUS "looking for libwkhtmltox-dev - done")
else()
    message(FATAL_ERROR "ERROR: libwkhtmltox-dev not installed")
endif()
