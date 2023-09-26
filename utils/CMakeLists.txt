add_library(utils SHARED

    include/utils/utils.hpp
    include/utils/error.hpp
    impl/error.cpp
    impl/utils.cpp
)
set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)



target_include_directories(utils INTERFACE
        include
        )

target_include_directories(utils PRIVATE
        include
        impl
        )

install(TARGETS utils DESTINATION bin)

