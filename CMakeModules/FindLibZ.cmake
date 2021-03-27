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

message(STATUS "looking for libz")

FIND_PATH ( LIBZ_INCLUDE_DIRS zlib.h
    /usr/local/share/libz/include
    /usr/local/include
    /usr/include
)

FIND_PATH ( LIBZ_STATIC_INCLUDE_DIRS zlib.h
    /usr/local/share/libz/include
    /usr/local/include
    /usr/include
)

FIND_LIBRARY ( LIBZ_LIBRARIES z
    /usr/local/share/libz/lib
    /usr/local/lib
    /usr/lib
)

FIND_LIBRARY ( LIBZ_STATIC_LIBRARIES libz.a
    /usr/local/share/libz/lib
    /usr/local/lib
    /usr/lib
)

GET_FILENAME_COMPONENT( LIBZ_LIBRARY_DIRS ${LIBZ_LIBRARIES} PATH )
GET_FILENAME_COMPONENT( LIBZ_STATIC_LIBRARY_DIRS ${LIBZ_STATIC_LIBRARIES} PATH )

SET ( LIBZ_FOUND OFF )
IF ( LIBZ_INCLUDE_DIRS )
    IF ( LIBZ_STATIC_INCLUDE_DIRS )
        IF ( LIBZ_LIBRARIES )
            IF ( LIBZ_STATIC_LIBRARIES )
                SET ( LIBZ_FOUND "YES" )
            ELSE ()
                message(STATUS "static library not found")
            ENDIF ( LIBZ_STATIC_LIBRARIES )
        ELSE ()
            message(STATUS "dynamic library not found")
        ENDIF ( LIBZ_LIBRARIES )
    ELSE ()
        message(STATUS "include static dirs not found")
    ENDIF ()
ELSE ()
    message(STATUS "include dirs not found")
ENDIF ()

MARK_AS_ADVANCED(
    LIBZ_LIBRARY_DIRS
    LIBZ_INCLUDE_DIRS
    LIBZ_LIBRARIES
    LIBZ_STATIC_INCLUDE_DIRS
    LIBZ_STATIC_LIBRARIES
)

IF (LIBZ_FOUND)
    message(STATUS "looking for libz - done")
ELSE()
    message(FATAL_ERROR "ERROR: libz not installed.")
ENDIF()
