add_library(utils SHARED

    include/utils/utils.hpp
    include/utils/error.hpp
    impl/error.cpp
    impl/utils.cpp
)
set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

#set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/test/")

#configure_file (cmake_data.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/utils/cmake_data.h)


target_include_directories(utils INTERFACE
        include
#        ${CMAKE_CURRENT_BINARY_DIR}/include/
        )

target_include_directories(utils PRIVATE
        include
        impl
        )

#target_link_libraries(utils PRIVATE Qt6::Gui Qt6::Core)

set(BUILD_TESTING ON)

if(BUILD_TESTING)
        add_subdirectory(test)
endif()

install(TARGETS utils DESTINATION bin)

