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

include(FindPkgConfig)
find_package(LibZ REQUIRED)
# find_package(LibSSL REQUIRED)

message(STATUS "looking for ImageMagick++-dev")

find_path( IMAGEMAGICK_INCLUDE_DIR Magick++.h
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

find_path( IMAGEMAGICK_STATIC_INCLUDE_DIR Magick++.h
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

#message( "IMAGEMAGICK_INCLUDE_DIR is ${IMAGEMAGICK_INCLUDE_DIR}" )

find_library( IMAGEMAGICK_LIBRARY
  NAMES Magick++
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

# message( "IMAGEMAGICK_LIBRARY is ${IMAGEMAGICK_LIBRARY}" )

find_library( IMAGEMAGICK++_STATIC_LIBRARY
  NAMES libGraphicsMagick++.a
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

find_library( IMAGEMAGICK_STATIC_LIBRARY
  NAMES libGraphicsMagick.a
  HINTS
  $ENV{IMAGEMAGICKDIR}
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


list(APPEND
  IMAGEMAGICK++_STATIC_LIBRARY
  ${IMAGEMAGICK_STATIC_LIBRARY}
  ${LIBZ_STATIC_LIBRARIES}
)

list(APPEND
  IMAGEMAGICK_STATIC_LIBRARY
  ${LIBZ_STATIC_LIBRARIES}
)

#set( IMAGEMAGICK_FOUND OFF )
#if(IMAGEMAGICK_INCLUDE_DIR)
#    if(IMAGEMAGICK_LIBRARY)
#	set( IMAGEMAGICK_FOUND "YES" )
#    endif()
#endif()

SET ( IMAGEMAGICK_FOUND OFF )
IF ( IMAGEMAGICK_INCLUDE_DIR )
    IF ( IMAGEMAGICK_STATIC_INCLUDE_DIR )
        IF ( IMAGEMAGICK_LIBRARY )
            IF ( IMAGEMAGICK_STATIC_LIBRARY )
                SET ( IMAGEMAGICK_FOUND "YES" )
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


if(IMAGEMAGICK_FOUND)
    message(STATUS "looking for ImageMagick++-dev - done")
else()
    message(FATAL_ERROR "ERROR: libgraphicsmagick++-dev not installed")
endif()
