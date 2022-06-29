cmake_minimum_required(VERSION 3.22)
project(lab3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
        Charts
        Sql
        )

add_executable(lab3 main.cpp IOCContainer.h dataReader.h dataReader.cpp chartDrawer.cpp chartDrawer.h Chart.cpp Chart.h mainwindow.cpp mainwindow.h)
target_link_libraries(lab3
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        Qt5::Sql
        )

