cmake_minimum_required(VERSION 3.25)
project(MazeMaker)

set(CMAKE_CXX_STANDARD 20)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if (wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif ()

add_executable(MazeMaker main.cpp
        models/RenderTimer.cpp
        models/RenderTimer.h
        models/DrawPanel.cpp
        models/DrawPanel.h
        models/Maze.cpp
        models/Maze.h
        models/Styles.h)

target_link_libraries(MazeMaker ${wxWidgets_LIBRARIES})