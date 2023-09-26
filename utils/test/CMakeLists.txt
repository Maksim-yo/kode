add_library(utils_test SHARED)
set_target_properties(utils_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(utils_test PUBLIC utils)
set_target_properties(utils_test PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)


set(tests
    utls_test
    )

foreach(test_target ${tests})
    if(EXISTS "${PROJECT_SOURCE_DIR}/${test_target}.h")
        add_executable(${test_target} "${test_target}.cpp" "${test_target}.h"     ${QT_RESOURCES})
    else()
        add_executable(${test_target} "${test_target}.cpp"     ${QT_RESOURCES})
    endif()
    set_target_properties(${test_target} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
    )
    target_link_libraries(${test_target} PRIVATE utils_test Qt6::Test)

    add_test(NAME    ${test_target}
             COMMAND ${test_target})
endforeach()
