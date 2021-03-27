set(EXECUTABLE_CGIs
    helpdesk.cgi
    account.cgi
    avataruploader.cgi
    generalimageuploader.cgi
    imageuploader.cgi
    noauth.cgi
    )

set(EXECUTABLE_CRONs
    cron_daily_pi
    cron_minute_pi
    )

# --- Libraries
# ---- big joint library
set(JOINT_LIBRARY_SRCs
    cuser.cpp
    cactivator.cpp
    ccgi.cpp
    ccookie.cpp
    csession.cpp
    crequest.cpp
    chtml.cpp
    cmail.cpp
    cvars.cpp
    cmysql.cpp
    cexception.cpp
    ctemplate.cpp
    cfiles.cpp
    cstatistics.cpp
    cvideoconverter.cpp
    c_float.cpp
    c_config.cpp
    c_smsc.cpp
    utilities1.cpp
    utilities2.cpp
    utilities3.cpp
    utilities_common.cpp
    utilities_specific.cpp
    c_cache_obj.cpp
    c_date_spelling.cpp
    )
# ---- list of independent libraries
set(INDEPENDENT_LIBRARIES_SRC
#    c_archive.cpp
#    c_price_spelling.cpp
    )

# --- MACROSes start ---

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")

    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()

    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

macro(set_pi_lib_properties target_lib proj_src proj_inc_list proj_lib_list)
    add_library                 (${target_lib}		${proj_src})
    target_include_directories  (${target_lib}		PUBLIC	${proj_inc_list})
    target_link_libraries       (${target_lib}		${proj_lib_list})

    set(PROJECT_PI_LIBS         ${PROJECT_PI_LIBS}     ${target_lib})
endmacro()

macro(__add_executable_1lib target_executable include_directories lib_list)
    __add_executable_4libs("${target_executable}" "${include_directories}" "${lib_list1}" "" "" "")
endmacro()

macro(__add_executable_2libs target_executable include_directories lib_list1 lib_list2)
    __add_executable_4libs("${target_executable}" "${include_directories}" "${lib_list1}" "${lib_list2}" "" "")
endmacro()

macro(__add_executable_3libs target_executable include_directories lib_list1 lib_list2 lib_list3)
    __add_executable_4libs("${target_executable}" "${include_directories}" "${lib_list1}" "${lib_list2}" "${lib_list3}" "")
endmacro()

macro(__add_executable_4libs target_executable include_directories lib_list1 lib_list2 lib_list3 lib_list4)
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${target_executable})

    add_executable              (${target_executable}    ${file_without_ext}.cpp)
    target_include_directories  (${target_executable}    PRIVATE ${include_directories})
    target_link_libraries       (${target_executable}    ${lib_list1} ${lib_list2} ${lib_list3} ${lib_list4})
endmacro()

macro(__add_executable_5libs target_executable include_directories lib_list1 lib_list2 lib_list3 lib_list4 lib_list5)
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${target_executable})

    add_executable              (${target_executable}    ${file_without_ext}.cpp)
    target_include_directories  (${target_executable}    PRIVATE ${include_directories})
    target_link_libraries       (${target_executable}    ${lib_list1} ${lib_list2} ${lib_list3} ${lib_list4} ${lib_list5})
endmacro()

# --- MACROSes finish ---


# --- Packages and libraries control
include(FindPkgConfig)

pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
#find_library(LIBMYSQLCLIENT NAMES libmysqlclient.a)

#find_package(OpenSSL REQUIRED)
find_package(LibSSL REQUIRED)

find_package(CURL REQUIRED)
#find_package(Libhpdf REQUIRED)
#find_package(Libarchive REQUIRED)
#find_package(Libxl REQUIRED)


if(IMAGEMAGICK_DISABLE)
    message("Magick++ turned off")
else(IMAGEMAGICK_DISABLE)
#    find_package(ImageMagick COMPONENTS Magick++)
    find_package(LibImageMagick REQUIRED)
endif(IMAGEMAGICK_DISABLE)

if(FFPMPEG_DISABLE)
    message("Videoconverter/ffmpeg turned off")
else(FFPMPEG_DISABLE)
    find_package(LibAVUtil REQUIRED)
    find_package(LibAVCodec REQUIRED)
    find_package(LibAVFormat REQUIRED)
endif(FFPMPEG_DISABLE)

if(MAXMIND_DISABLE)
    message("GeoIP turned off")
else(MAXMIND_DISABLE)
    find_package(LibMaxMindDB REQUIRED)
endif(MAXMIND_DISABLE)

set(PROJECT_PI_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/src/pi/include/
    ${PROJECT_BINARY_DIR}/src/pi/include/       # --- rendered config.h, generated table.h
    ${LIBMYSQLCLIENT_INCLUDE_DIRS}
    ${LIBSSL_STATIC_INCLUDE_DIR}
    ${IMAGEMAGICK_STATIC_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
    ${AVUTIL_INCLUDE_DIR}
    ${AVFORMAT_INCLUDE_DIR}
    ${HPDF_INCLUDE_DIR}
    ${ARCHIVE_INCLUDE_DIR}
    ${XL_INCLUDE_DIR}
    )

set(PROJECT_EXTERNAL_LIBS
    ${LIBMYSQLCLIENT_STATIC_LIBRARIES}
    
    # ${OPENSSL_STATIC_CRYPTO_LIBRARIES}
    ${LIBSSL_STATIC_LIBRARIES}

    ${IMAGEMAGICK_LIBRARY}
    #${IMAGEMAGICK++_STATIC_LIBRARY}

    ${CURL_LIBRARIES}
    ${AVUTIL_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${MAXMINDDB_LIBRARY}
    ${HPDF_LIBRARY}
    ${ARCHIVE_LIBRARY}
    ${XL_LIBRARY}
    )

# --- does this system provide the exp function ?
include(CheckFunctionExists)
check_function_exists(exp HAVE_EXP)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/pi/include/localy.h.in"
    "${PROJECT_BINARY_DIR}/src/pi/include/localy.h"
    )


# ---- list of independent libraries
foreach(INDEPENDENT_LIBRARY_SRC ${INDEPENDENT_LIBRARIES_SRC})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${INDEPENDENT_LIBRARY_SRC})

    set_pi_lib_properties(${file_without_ext} "${file_without_ext}.cpp" "${PROJECT_PI_INCLUDE_DIRECTORIES}" "")
endforeach()

# ---- big joint library
# ---- ATTENTION !!!
# ---- order is important: joint_lib must follow INDEPENDENT LIBS to help g++-linker resolve dependencies
set_pi_lib_properties(__joint_pi "${JOINT_LIBRARY_SRCs}" "${PROJECT_PI_INCLUDE_DIRECTORIES}" "")

# --- MSVC grouping
source_group("Headers Private"  FILES ${HDR_PRIVATE})
source_group("Headers Public"   FILES ${HDR_PUBLIC})
source_group("Sources"          FILES ${SOURCES})
source_group("Resources"        FILES ${RESOURCES})

# --- Link part (2 parts: custom build, normal build)
# ---- 1) Custom build speeding up linking process by linking only required libraries (.o)

# ---- 2) Normal build link with _all_ libraries (.o), therefore change in any .src trigger re-link for all executables
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs})
    __add_executable_2libs  (${EXECUTABLE_CGI}      "${PROJECT_PI_INCLUDE_DIRECTORIES}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()

foreach(EXECUTABLE_CRON ${EXECUTABLE_CRONs})
    __add_executable_2libs  (${EXECUTABLE_CRON}      "${PROJECT_PI_INCLUDE_DIRECTORIES}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()

# --- Install settings

# --- Install part
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs})
    install(TARGETS ${EXECUTABLE_CGI}       DESTINATION		${CGIDIR})
endforeach()

foreach(EXECUTABLE_CRON ${EXECUTABLE_CRONs})
    install(TARGETS ${EXECUTABLE_CRON}       DESTINATION     ${CRON_INSTALL_DIR})
endforeach()

# print_all_variables()
