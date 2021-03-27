message("-- buid tests/auto/")

# --- this set defined to speed up linking process
# --- these apps links w/ PI_LIBS and EXT_LIBS, not PD_LIBS
set(PI_TESTS
    autotest_basic1.cpp
    autotest_utilities_declension.cpp
    autotest_utilities_rus_to_lower.cpp
    autotest_utilities_date.cpp
    autotest_utilities_db_basic.cpp
    )

# --- MACROSes start ---

# --- MACROSes finish ---

GET_DIRECTORY_PROPERTY  (PROJECT_PI_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PI_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_LIBS)
GET_DIRECTORY_PROPERTY  (PROJECT_EXTERNAL_LIBS          DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_EXTERNAL_LIBS)

GET_DIRECTORY_PROPERTY  (PROJECT_PD_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pd/ DEFINITION PROJECT_PD_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PD_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pd/ DEFINITION PROJECT_PD_LIBS)

GET_DIRECTORY_PROPERTY  (PROJECT_CHAT_LIBS          DIRECTORY ${PROJECT_SOURCE_DIR}/src/chat/ DEFINITION PROJECT_CHAT_LIBS)

set(PROJECT_ALL_INCLUDE_DIRECTORIES ${PROJECT_PI_INCLUDE_DIRECTORIES} ${PROJECT_PD_INCLUDE_DIRECTORIES})

set(CTEST_BINARY_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

file(GLOB ALL_TESTS "autotest_*.cpp")

# --- Link part (2 parts: custom build, normal build)
# ---- 1) Custom build speeding up linking process by linking only required libraries (.o)
set(LINK_DESTINATIONS ${ALL_TESTS})
foreach(file ${PI_TESTS})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})

    __add_executable_2libs("${file_without_ext}" "${PROJECT_PI_INCLUDE_DIRECTORIES}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}" "${PROJECT_CHAT_LIBS}")
    list(REMOVE_ITEM LINK_DESTINATIONS ${PROJECT_SOURCE_DIR}/tests/auto/${file})
endforeach()


# ---- 2) Normal build link with _all_ libraries (.o), therefore change in any .src trigger re-link for all executables
foreach(file ${LINK_DESTINATIONS})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})

    __add_executable_3libs("${file_without_ext}" "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${PROJECT_PD_LIBS}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()

# --- Add tests
foreach(file ${ALL_TESTS})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})

    add_test(${file_without_ext} ${file_without_ext})
    set_tests_properties(${file_without_ext} PROPERTIES PASS_REGULAR_EXPRESSION "Test passed")
    set_tests_properties(${file_without_ext} PROPERTIES FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
    set_tests_properties(${file_without_ext} PROPERTIES TIMEOUT 120)
endforeach()

#print_all_variables()