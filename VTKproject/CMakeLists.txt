cmake_minimum_required(VERSION 3.5)

project(VTKproject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
    message(FATAL_ERROR "Unable to find the VTK build folder.")
endif()

set(TS_FILES VTKproject_en_GB.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
    VRRenderThread.cpp # Add VRRenderThread.cpp to the project sources
    VRRenderThread.h
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VTKproject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        icons.qrc
        optiondialog.h optiondialog.cpp optiondialog.ui
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(VTKproject SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(VTKproject
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(VTKproject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VTKproject)
endif()
set_target_properties(VTKproject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VTKproject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VTKproject)
endif()

add_custom_target(  VRBindings )
add_custom_command( TARGET VRBindings PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E
            copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )

#INSTALLER

include(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "VTKproject") 
set(CPACK_PACKAGE_VENDOR "Group 28")
#set (CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeInstall.bmp")
#set (CPACK_RESOURCE_FILE_LICENSE "")


set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_CREATE_DESKTOP_LINKS VTKproject)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VRproject")

set(CPACK_PACKAGE_EXECUTABLES "VTKproject" "Renderer")

include(CPack)

install(TARGETS VTKproject RUNTIME DESTINATION bin)

install(DIRECTORY "C:/VRlibs/VS" DESTINATION debugx64 COMPONENT debugx64 FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/VRlibs/ucrt" DESTINATION debugx64 COMPONENT debugx64 FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/VRlibs/VTK" DESTINATION VTK COMPONENT VTK FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/VRlibs/QtVRlibs" DESTINATION Qt COMPONENT Qt)
install(DIRECTORY "C:/VRlibs/OpenVR" DESTINATION OpenVR COMPONENT OpenVR FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/VRlibs/Examples" DESTINATION stl_files COMPONENT stl_files)

cpack_add_component_group(Libraries DESCRIPTION "Libraries required")
cpack_add_component(Qt DISPLAY_NAME "QT Libraries" DESCRIPTION "Qt dynamic libraries" GROUP Libraries)
cpack_add_component(VTK DISPLAY_NAME "VTK Libraries" DESCRIPTION "VTK dynamic libraries" GROUP Libraries)
cpack_add_component(OpenVR DISPLAY_NAME "OpenVR" DESCRIPTION "OpenVR dynamic libraries" GROUP Libraries)
cpack_add_component(stl_files DISPLAY_NAME "STL Examples" DESCRIPTION "STL examples for the software")