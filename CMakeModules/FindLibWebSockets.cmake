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

# include(FindPkgConfig)
# find_package(LibSSL REQUIRED)
# find_package(LibZ REQUIRED)

message(STATUS "looking for libwebsockets")

FIND_PATH ( LIBWEBSOCKETS_INCLUDE_DIRS libwebsockets.h
    /usr/local/share/libwebsockets/include
    /usr/local/include
    /usr/include
)

FIND_PATH ( LIBWEBSOCKETS_STATIC_INCLUDE_DIRS libwebsockets.h
    /usr/local/share/libwebsockets/include
    /usr/local/include
    /usr/include
)

FIND_LIBRARY ( LIBWEBSOCKETS_LIBRARIES websockets
    /usr/local/share/libwebsockets/lib
    /usr/local/lib
    /usr/lib
)

FIND_LIBRARY ( LIBWEBSOCKETS_STATIC_LIBRARIES libwebsockets.a
    /usr/local/share/libwebsockets/lib
    /usr/local/lib
    /usr/lib
)

GET_FILENAME_COMPONENT( LIBWEBSOCKETS_LIBRARY_DIRS ${LIBWEBSOCKETS_LIBRARIES} PATH )
GET_FILENAME_COMPONENT( LIBWEBSOCKETS_STATIC_LIBRARY_DIRS ${LIBWEBSOCKETS_STATIC_LIBRARIES} PATH )


#list ( APPEND
#    LIBWEBSOCKETS_STATIC_LIBRARIES 
#    "${LIBWEBSOCKETS_STATIC_LIBRARIES}" 
#    "${LIBSSL_STATIC_LIBRARIES}"
#    "${LIBZ_STATIC_LIBRARIES}"
#)

SET ( LIBWEBSOCKETS_FOUND OFF )
IF ( LIBWEBSOCKETS_INCLUDE_DIRS )
    IF ( LIBWEBSOCKETS_STATIC_INCLUDE_DIRS )
        IF ( LIBWEBSOCKETS_LIBRARIES )
            IF ( LIBWEBSOCKETS_STATIC_LIBRARIES )
                SET ( LIBWEBSOCKETS_FOUND "YES" )
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

IF (LIBWEBSOCKETS_FOUND)
    message(STATUS "looking for libwebsockets - done")
ELSE()
    message(FATAL_ERROR "ERROR: libwebsockets not installed. Clone guthub repo https://github.com/warmcat/libwebsockets.git and build")
ENDIF()
