set(EXECUTABLE_CGIs_with_PD_PI_LIBS
    admin.cgi
    index.cgi
    anyrole_1.cgi
    book.cgi
    company.cgi
    group.cgi
    system.cgi
    companylogouploader.cgi
    complain.cgi
    grouplogouploader.cgi
    )
set(EXECUTABLE_CGIs_with_PI_LIBS
    cron_daily_pd
    )

# --- Libraries
# ---- joint library
set(JOINT_LIBRARY_SRCs
    utilities_connme1.cpp
    utilities_connme2.cpp
)
# ---- list of independent libraries
set(INDEPENDENT_LIBRARIES_SRCs
    )

# --- MACROSes start ---

macro(set_pd_lib_properties target_lib proj_src proj_inc_list proj_lib_list)
    add_library                 (${target_lib}      ${proj_src})
    target_include_directories  (${target_lib}      PUBLIC  ${proj_inc_list})
    target_link_libraries       (${target_lib}      ${proj_lib_list})

    set(PROJECT_PD_LIBS         ${PROJECT_PD_LIBS}  ${target_lib})
endmacro()

# --- MACROSes finish ---

GET_DIRECTORY_PROPERTY  (XL_LIBRARY_DIR DIRECTORY       ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION XL_LIBRARY_DIR)

GET_DIRECTORY_PROPERTY  (PROJECT_PI_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PI_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_LIBS)
GET_DIRECTORY_PROPERTY  (PROJECT_EXTERNAL_LIBS          DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_EXTERNAL_LIBS)

set(PROJECT_PD_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/src/pd/include/
    ${PROJECT_SOURCE_DIR}/cyrillic-facets/      # --- mbstowcs 
    )

SET(PROJECT_ALL_INCLUDE_DIRECTORIES ${PROJECT_PI_INCLUDE_DIRECTORIES} ${PROJECT_PD_INCLUDE_DIRECTORIES})

# ---- list of independent libraries
foreach(INDEPENDENT_LIBRARY_SRC ${INDEPENDENT_LIBRARIES_SRCs})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${INDEPENDENT_LIBRARY_SRC})

    set_pd_lib_properties(${file_without_ext} "${file_without_ext}.cpp" "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${project_link_libraries}")
endforeach()

set_pd_lib_properties(__joint_pd "${JOINT_LIBRARY_SRCs}" "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "")

# --- MSVC grouping
source_group("Headers Private"  FILES ${HDR_PRIVATE})
source_group("Headers Public"   FILES ${HDR_PUBLIC})
source_group("Sources"          FILES ${SOURCES})
source_group("Resources"        FILES ${RESOURCES})

# --- Link part (2 parts: custom build, normal build)
# ---- 1) Custom build speeding up linking process by linking only required libraries (.o)
# ---- example:
#      __add_executable_3libs(admin.cgi "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "utilities_connme1" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
#      list(REMOVE_ITEM EXECUTABLE_CGIs_with_PD_PI_LIBS admin.cgi)

# ---- 2) Normal build link with _all_ libraries (.o), therefore change in any .src trigger re-link for all executables
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_PI_LIBS})
    __add_executable_2libs(${EXECUTABLE_CGI} "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_PD_PI_LIBS})
    __add_executable_3libs(${EXECUTABLE_CGI} "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${PROJECT_PD_LIBS}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()


# --- Install settings

# --- Install part

install(TARGETS index.cgi			    DESTINATION		${CGIDIR})
install(TARGETS anyrole_1.cgi           DESTINATION     ${CGIDIR})
install(TARGETS book.cgi    		    DESTINATION		${CGIDIR})
install(TARGETS company.cgi			    DESTINATION		${CGIDIR})
install(TARGETS group.cgi               DESTINATION		${CGIDIR})
install(TARGETS system.cgi              DESTINATION     ${CGIDIR})
install(TARGETS companylogouploader.cgi DESTINATION     ${CGIDIR})
install(TARGETS complain.cgi            DESTINATION     ${CGIDIR})
install(TARGETS grouplogouploader.cgi   DESTINATION     ${CGIDIR})


install(TARGETS admin.cgi               DESTINATION     ${CGIADMINDIR})

install(TARGETS cron_daily_pd   	   DESTINATION		${CRON_INSTALL_DIR})

# print_all_variables()
