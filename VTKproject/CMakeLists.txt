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

set(CPACK_SOURCE_GENERATOR "NSIS")


include(CPack)

# set the install/uninstall icon used for the installer itself
#set (CPACK_NSIS_MUI_ICON
 #    "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeLogo.ico")
#set (CPACK_NSIS_MUI_UNIICON
 #    "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeLogo.ico")

install(TARGETS VTKproject RUNTIME DESTINATION bin)

install(DIRECTORY "C:/Program FIles (x86)/VTK/bin" DESTINATION bin COMPONENT VTK FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/Qt/6.6.2/msvc2019_64/bin" DESTINATION bin COMPONENT Qt FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/OpenVR/bin/win64" DESTINATION bin COMPONENT OpenVR FILES_MATCHING PATTERN "*.dll")
# /calc_cmake/CMakeLists.txt CHANGE ALL BACK SLASHES TO FORWARD SLASHES do openvr files by self
set (CPACK_NSIS_PACKAGE_NAME "Group 28 VR Project")
#set (CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeInstall.bmp")
#set (CPACK_RESOURCE_FILE_LICENSE "")
#set (CPACK_CREATE_DESKTOP_LINKS cmake-gui)
set (CPACK_PACKAGE_INSTALL_DIRECTORY "VRproject")