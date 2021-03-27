# --- in-source build prohibited

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# ---------------------------

cmake_minimum_required(VERSION 3.5)

project("connme.ru" CXX)
set(MAIN_DOMAIN_NAME "dev.${PROJECT_NAME}")
set(IMAGE_DOMAIN_NAME "devimages.${PROJECT_NAME}")
set(My_VERSION_MAJOR 0)
set(My_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 14)
# --- changing CXX_FLAGS is _not_ the best practise
# --- cmake 3.12 have -D switch, capable remove definition per-target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic")

# --- Doesn't work properly, enable and give it a try
# --- set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/ )

option(IMAGEMAGICK_DISABLE	"turn off ImageMagick++" OFF)
option(MAXMIND_DISABLE		"turn off GeoIP" OFF)
option(FFPMPEG_DISABLE		"turn off VideoConverter" OFF)

# --- Set required variables
# ---- build variables
set(CRON_PROJECT_DIR		${PROJECT_BINARY_DIR}/share/${MAIN_DOMAIN_NAME}/cron)
set(ARCHIVE_PROJECT_DIR		${PROJECT_BINARY_DIR}/share/${MAIN_DOMAIN_NAME}/archive)
set(MAXMIND_PROJECT_DIR		${PROJECT_BINARY_DIR}/share/${MAIN_DOMAIN_NAME}/maxmind)
set(CI_SCRIPT_PROJECT_DIR	${PROJECT_BINARY_DIR}/share/${MAIN_DOMAIN_NAME}/ci-scripts)
set(LOCALE_PROJECT_DIR		${PROJECT_BINARY_DIR}/share/${MAIN_DOMAIN_NAME}/locale)
set(GIT_PROJECT_DIR             ${PROJECT_SOURCE_DIR}/supplemental/git)
set(PDF_FONT_PROJECT_DIR	${PROJECT_SOURCE_DIR}/supplemental/libpdf-font)
set(LIBXL_PROJECT_DIR		${PROJECT_SOURCE_DIR}/libxl-3.8.4.0/)

# ---- install variables
set(ROOTDIR			"/home/httpd/${MAIN_DOMAIN_NAME}")
set(IMAGEROOTDIR		"/home/httpd/${IMAGE_DOMAIN_NAME}")
set(CGIDIR			${ROOTDIR}/cgi-bin)
set(CGIADMINDIR			${ROOTDIR}/cgi-bin/admin)
set(LOCAL_INSTALL_DIR           ${CMAKE_INSTALL_PREFIX}/share/${MAIN_DOMAIN_NAME})
set(LOG_DIR                      ${LOCAL_INSTALL_DIR}/log/)
set(CONFIG_DIR                   ${LOCAL_INSTALL_DIR}/config)
set(CRON_INSTALL_DIR		${LOCAL_INSTALL_DIR}/cron)
set(ARCHIVE_INSTALL_DIR		${LOCAL_INSTALL_DIR}/archive)
set(MAXMIND_INSTALL_DIR		${LOCAL_INSTALL_DIR}/maxmind)
set(LOCALE_INSTALL_DIR		${LOCAL_INSTALL_DIR}/locale)
set(CHAT_INSTALL_DIR		${LOCAL_INSTALL_DIR}/chat)
set(GIT_INSTALL_DIR             ${LOCAL_INSTALL_DIR}/git)
set(PDF_FONT_INSTALL_DIR	${LOCAL_INSTALL_DIR}/libpdf-font)
set(LIBXL_INSTALL_DIR		${LOCAL_INSTALL_DIR}/libxl)

# ---- libxl library is not in apt-reporsitory, therefore is not installed on the OS
# ---- to treat it as regular lib I have to copy it to /usr/local/share..... folder
# ---- 1) copy libxl to application location
# ---- 2) work with it as w/ regular library
# if(EXISTS ${LIBXL_INSTALL_DIR})
#     message	(STATUS	"install libxl to ${LIBXL_INSTALL_DIR}")
#     file	(COPY	${LIBXL_PROJECT_DIR}lib/libxl.so	DESTINATION	${LIBXL_INSTALL_DIR}/lib)
#     file	(COPY	${LIBXL_PROJECT_DIR}lib64/libxl.so	DESTINATION	${LIBXL_INSTALL_DIR}/lib64)
# else()
#     message	(WARNING "libxl.so will be taken from source-tree, because ${LIBXL_INSTALL_DIR} doesn't exist")
# endif()

add_subdirectory(src/)
add_subdirectory(supplemental/cron/)
add_subdirectory(supplemental/archive/)
add_subdirectory(supplemental/maxmind/)
add_subdirectory(supplemental/ci-scripts/)
add_subdirectory(supplemental/libpdf-font/)
add_subdirectory(supplemental/git/)
add_subdirectory(po/)

# --- Install time
install(DIRECTORY DESTINATION ${LOG_DIR}  DIRECTORY_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ)
install(DIRECTORY DESTINATION ${CONFIG_DIR} DIRECTORY_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)

# --- CTest
include(CTest)
add_subdirectory(tests/)

# --- CPack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${My_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${My_VERSION_MINOR}")

include(CPack)

