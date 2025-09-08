cmake_minimum_required(VERSION 3.16)

project(test2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick ShaderTools)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apptest2
    main.cpp
)

qt_add_qml_module(apptest2
    URI test2
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES source/vpnservice.cpp source/vpnservice.h
)

qt_add_shaders(apptest2 "shaders"
    PREFIX "/"
    FILES
        shaders/patterrneffect.frag
        shaders/patterrneffect.vert
        shaders/blurandglass.frag
        shaders/blurandglass.vert
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptest2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptest2
    PRIVATE Qt6::Quick Qt6::ShaderTools

)
target_include_directories(apptest2
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

include(GNUInstallDirs)
install(TARGETS apptest2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
