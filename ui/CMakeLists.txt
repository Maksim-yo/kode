qt6_add_resources(QT_RESOURCES ${CMAKE_SOURCE_DIR}/resources.qrc)

add_library(ui SHARED
    impl/ObjectItem.cpp
    impl/UIHandler.cpp
    impl/FileHandler.cpp
    impl/SorterFilter.cpp

    include/ui/SorterFilter.hpp
    include/ui/FileHandler.hpp
    include/ui/objectItem.hpp
    include/ui/UIHandler.hpp

    )
set_target_properties(ui PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_include_directories(ui INTERFACE
    include
    )
target_include_directories(ui PRIVATE
    impl
    include
    )

target_link_libraries(ui PRIVATE  Qt6::Qml Qt6::Widgets Qt6::Gui utils)
qt_add_executable(kode
    main.cpp
    ${QT_RESOURCES}

    )

qt_add_qml_module(kode
    URI kode
    VERSION 1.0
    QML_FILES
    qml/CustomButton.qml
    qml/ImageButton.qml
    qml/main.qml
    qml/ObjectList.qml
    qml/ObjectListItem.qml
    qml/ObjectGroup.qml
    qml/DialogLabel.qml
    qml/CustomPopupElement.qml
    qml/SortButton.qml
    qml/AddDialog.qml
    qml/SortDialogHelper.qml
    )
target_link_libraries(kode
    PRIVATE Qt6::Quick Qt6::Multimedia Qt6::Qml Qt6::Widgets Qt6::Gui utils ui)

set(BUILD_TESTING ON)

if(BUILD_TESTING)
        add_subdirectory(test)
endif()

#target_link_libraries(kode PRIVATE utils)
