cmake_minimum_required(VERSION 3.19)
project(DeterminantProject LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# Dosya isimlerini burada TEK BİR YERDE tanımlıyoruz
set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

qt_add_executable(DeterminantProject
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

target_link_libraries(DeterminantProject
    PRIVATE
    Qt6::Core
    Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS DeterminantProject
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DeterminantProject
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
