
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(light
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.cpp mainwindow.ui
    physicsthread.hpp physicsthread.cpp
    ScreenDisplayer.cpp ScreenDisplayer.hpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(light PRIVATE phys Qt6::Core Qt6::Widgets)
