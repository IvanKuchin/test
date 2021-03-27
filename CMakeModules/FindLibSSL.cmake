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

message(STATUS "looking for libssl")

FIND_PATH ( LIBSSL_INCLUDE_DIRS ssl.h
    /usr/local/share/libssl/include
    /usr/local/include
    /usr/include
    /usr/include/openssl
)

FIND_PATH ( LIBSSL_STATIC_INCLUDE_DIRS ssl.h
    /usr/local/share/libssl/include
    /usr/local/include
    /usr/include
    /usr/include/openssl
)

FIND_LIBRARY ( LIBSSL_LIBRARIES ssl
    /usr/local/share/libssl/lib
    /usr/local/lib
    /usr/lib
)

FIND_LIBRARY ( LIBSSL_STATIC_LIBRARIES libssl.a
    /usr/local/share/libssl/lib
    /usr/local/lib
    /usr/lib
)

GET_FILENAME_COMPONENT( LIBSSL_LIBRARY_DIRS ${LIBSSL_LIBRARIES} PATH )
GET_FILENAME_COMPONENT( LIBSSL_STATIC_LIBRARY_DIRS ${LIBSSL_STATIC_LIBRARIES} PATH )

SET ( LIBSSL_FOUND OFF )
IF ( LIBSSL_INCLUDE_DIRS )
    IF ( LIBSSL_STATIC_INCLUDE_DIRS )
        IF ( LIBSSL_LIBRARIES )
            IF ( LIBSSL_STATIC_LIBRARIES )
                SET ( LIBSSL_FOUND "YES" )
            ELSE ()
                message(STATUS "static library not found")
            ENDIF ( LIBSSL_STATIC_LIBRARIES )
        ELSE ()
            message(STATUS "dynamic library not found")
        ENDIF ( LIBSSL_LIBRARIES )
    ELSE ()
        message(STATUS "include static dirs not found")
    ENDIF ()
ELSE ()
    message(STATUS "include dirs not found")
ENDIF ()

MARK_AS_ADVANCED(
    LIBSSL_LIBRARY_DIRS
    LIBSSL_INCLUDE_DIRS
    LIBSSL_LIBRARIES
    LIBSSL_STATIC_INCLUDE_DIRS
    LIBSSL_STATIC_LIBRARIES
)

IF (LIBSSL_FOUND)
    message(STATUS "looking for libssl - done")
ELSE()
    message(FATAL_ERROR "ERROR: libssl not installed.")
ENDIF()
