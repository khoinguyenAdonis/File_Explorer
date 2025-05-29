cmake_minimum_required(VERSION 3.16)

project(file_System_Ex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appfile_System_Ex

    "Source/main.cpp"
)

qt_add_qml_module(appfile_System_Ex
    URI file_System_Ex
    VERSION 1.0
    QML_FILES
        About.qml
        Editor.qml
        FileList.qml
        Main.qml
        MyMenu.qml
        MyMenuBar.qml
        Sidebar.qml
        ViewFile.qml
        WindowDragHandler.qml

    RESOURCES
        "icons/folder.png"
        "icons/folder_open.svg"
        "icons/generic_file.svg"
        "icons/github-logo.png"
        "icons/information.png"
        "icons/cutting.png"
        "icons/copy.png"
        "icons/paste.png"
        "icons/resize.svg"
        "icons/qt_logo.svg"
        "icons/trash.png"
        "icons/app_icon.png"

    SOURCES
        "Header/treemodel.h"
        "Source/treemodel.cpp"
        "Header/file.hpp"
        "Header/abstractClass.hpp"
        "Source/file.cpp"
        "Header/fileeditor.h"
        "Source/fileeditor.cpp"
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfile_System_Ex PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfile_System_Ex
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfile_System_Ex
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appfile_System_Ex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
