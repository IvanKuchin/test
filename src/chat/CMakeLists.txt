set(EXECUTABLE_CGIs
    chat-server
    )

# --- Libraries
set(INDEPENDENT_LIBRARIES_SRCs
    )

# ---- big joint library
set(JOINT_LIBRARY_SRCs
    chat-server-protocol-dumb.cpp
    chat-server-requestratelimiter.cpp
    chat-server-jsonparser.cpp
    chat-server-protocol-message.cpp
    chat-server-singlemessage.cpp
    chat-server-presencecache.cpp
    chat-server-protocol-status.cpp
    )

# --- MACROSes start ---
macro(set_chat_lib_properties target_lib proj_src proj_inc_list proj_lib_list)
    add_library                 (${target_lib}      ${proj_src})
    target_include_directories  (${target_lib}      PUBLIC  ${proj_inc_list})
    target_link_libraries       (${target_lib}      ${proj_lib_list})

    set(PROJECT_CHAT_LIBS         ${PROJECT_CHAT_LIBS}  ${target_lib})
endmacro()

# --- MACROSes finish ---

# --- Packages and libraries control
include(FindPkgConfig)
find_package(LibWebSockets REQUIRED)
find_package(LibSSL REQUIRED)

set(CHAT_EXTERNAL_INCLUDE_DIRECTORIES
    ${LIBWEBSOCKETS_INCLUDE_DIRS}
)

set(CHAT_EXTERNAL_LIBS
    ${LIBWEBSOCKETS_LIBRARIES}
    ${LIBSSL_LIBRARIES}
)


GET_DIRECTORY_PROPERTY  (PROJECT_PD_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pd/ DEFINITION PROJECT_PD_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PD_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pd/ DEFINITION PROJECT_PD_LIBS)
GET_DIRECTORY_PROPERTY  (PROJECT_PI_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PI_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_LIBS)
GET_DIRECTORY_PROPERTY  (PROJECT_EXTERNAL_LIBS          DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_EXTERNAL_LIBS)

set(PROJECT_CHAT_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/src/chat/include/
    ${PROJECT_PI_INCLUDE_DIRECTORIES}
    ${PROJECT_PD_INCLUDE_DIRECTORIES}
    ${CHAT_EXTERNAL_INCLUDE_DIRECTORIES}
    )

# ---- list of independent libraries
foreach(INDEPENDENT_LIBRARY_SRC ${INDEPENDENT_LIBRARIES_SRCs})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${INDEPENDENT_LIBRARY_SRC})

    set_chat_lib_properties(${file_without_ext} "${file_without_ext}.cpp" "${PROJECT_CHAT_INCLUDE_DIRECTORIES}" "")
endforeach()

set_chat_lib_properties(__joint_chat "${JOINT_LIBRARY_SRCs}" "${PROJECT_CHAT_INCLUDE_DIRECTORIES}" "")

# --- MSVC grouping
source_group("Headers Private"  FILES ${HDR_PRIVATE})
source_group("Headers Public"   FILES ${HDR_PUBLIC})
source_group("Sources"          FILES ${SOURCES})
source_group("Resources"        FILES ${RESOURCES})

# --- Link part (2 parts: custom build, normal build)
# ---- 1) Custom build speeding up linking process by linking only required libraries (.o)

# ---- 2) Normal build link with _all_ libraries (.o), therefore change in any .src trigger re-link for all executables
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs})
    __add_executable_5libs  (${EXECUTABLE_CGI} "${PROJECT_CHAT_INCLUDE_DIRECTORIES}" "${PROJECT_CHAT_LIBS}" "${PROJECT_PD_LIBS}" "${PROJECT_PI_LIBS}" "${CHAT_EXTERNAL_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
    #__add_executable_4libs  (${EXECUTABLE_CGI} "${PROJECT_CHAT_INCLUDE_DIRECTORIES}" "${PROJECT_CHAT_LIBS}"  "${PROJECT_PI_LIBS}" "${CHAT_EXTERNAL_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(chat-server Threads::Threads)

# --- Install settings

# --- Install part
install(TARGETS chat-server       DESTINATION     ${CHAT_INSTALL_DIR})

# print_all_variables()
