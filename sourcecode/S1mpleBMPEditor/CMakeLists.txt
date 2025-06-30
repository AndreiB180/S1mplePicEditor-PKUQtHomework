cmake_minimum_required(VERSION 3.19)
project(S1mpleBMPEditor LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_resources(RESOURCES
    res.qrc
)




qt_add_executable(S1mpleBMPEditor
    WIN32 MACOSX_BUNDLE
    main.cpp
    imagelabel.h
    imagelabel.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    importwindow.h importwindow.cpp importwindow.ui
    res.qrc
    processimage.h processimage.cpp
    rotatedialog.h rotatedialog.cpp rotatedialog.ui
    adjustdialog.h adjustdialog.cpp adjustdialog.ui
    cropwindow.h cropwindow.cpp cropwindow.ui
    mosaicdialog.h mosaicdialog.cpp mosaicdialog.ui
    lightnessdialog.h lightnessdialog.cpp lightnessdialog.ui
    contrastdialog.h contrastdialog.cpp contrastdialog.ui
    saturationdialog.h saturationdialog.cpp saturationdialog.ui
    clickimagedialog.h clickimagedialog.cpp
    watermarkdialog.h watermarkdialog.cpp
    recentfileswindow.h recentfileswindow.cpp recentfileswindow.ui
    clickablewidget.h clickablewidget.cpp clickablewidget.ui
    pensettingdialog.h pensettingdialog.cpp
    penwindow.h penwindow.cpp
    tempdialog.h tempdialog.cpp
    sharpendialog.h sharpendialog.cpp
    drawmosaicdialog.h drawmosaicdialog.cpp




)

target_sources(S1mpleBMPEditor PRIVATE ${RESOURCES})

qt_add_translations(
    TARGETS S1mpleBMPEditor
    TS_FILES S1mpleBMPEditor_zh_CN.ts
)



target_link_libraries(S1mpleBMPEditor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS S1mpleBMPEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET S1mpleBMPEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
