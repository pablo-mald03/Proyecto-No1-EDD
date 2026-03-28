cmake_minimum_required(VERSION 3.19)
project(ProyectoNo1-EDD LANGUAGES CXX)

find_package(Qt6 6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(ProyectoNo1-EDD
    WIN32 MACOSX_BUNDLE
    main.cpp
    include/mainwindow.h
    include/pantallaprincipal.h
    include/pantallaagregar.h
    include/controlador.h
    include/pantallabuscarporcategoria.h
    include/pantallabuscarpornombre.h
    include/pantallabuscarrango.h
    include/pantallalistarnombre.h
    include/pantallaeliminarproducto.h

    include/pantallaverarboles.h
    include/graphicsviewzoom.h

    include/pantallaarbolavl.h
    include/pantallaarbolb.h
    include/pantallaarbolbmas.h

    src/mainwindow.cpp
    src/pantallaprincipal.cpp
    src/pantallaagregar.cpp
    src/controlador.cpp
    src/pantallabuscarporcategoria.cpp
    src/pantallabuscarpornombre.cpp
    src/pantallabuscarrango.cpp
    src/pantallalistarnombre.cpp
    src/pantallaeliminarproducto.cpp

    src/pantallaverarboles.cpp

    src/pantallaarbolavl.cpp
    src/pantallaarbolb.cpp
    src/pantallaarbolbmas.cpp
    src/graphicsviewzoom.cpp

    ui/mainwindow.ui
    ui/pantallaprincipal.ui
    ui/pantallaagregar.ui
    ui/pantallabuscarporcategoria.ui
    ui/pantallabuscarpornombre.ui
    ui/pantallabuscarrango.ui
    ui/pantallalistarnombre.ui
    ui/pantallaeliminarproducto.ui

    ui/pantallaverarboles.ui

    ui/pantallaarbolavl.ui
    ui/pantallaarbolb.ui
    ui/pantallaarbolbmas.ui

    recursos.qrc


)

target_include_directories(ProyectoNo1-EDD PRIVATE include)


target_link_libraries(ProyectoNo1-EDD
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS ProyectoNo1-EDD
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ProyectoNo1-EDD
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
