cmake_minimum_required(VERSION 3.25)
project(picture_viewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置变量PROJECT_ROOT_DIR为项目根目录路径
get_filename_component(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)

set(CMAKE_PREFIX_PATH "F:\\QT5_9_9\\5.9.9\\mingw53_32")

# 将PROJECT_ROOT_DIR定义为预处理器宏
add_definitions(-DPROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

# ico.rc 是为了修改程序的图标，其中ico.rc内容如下： IDI_ICON1 ICON "ui/images/pic_2d/images-solid.ico"
# .qrc 是资源文件，包含一些图片，视频资源
#         src/my_qrc.qrc
#  WIN 32
add_executable( picture_viewer

        src/main.cpp
        src/ico.rc
        src/function.cpp
        src/function.h
        src/ui/ui_file/Qtree_ui.h
        src/some_function/my_photo_Graphics.cpp
        src/some_function/my_photo_Graphics.h
        src/some_function/my_Qtreewidget.cpp
        src/some_function/my_Qtreewidget.h
        src/some_function/Item_Interface.cpp
        src/some_function/Item_Interface.h
        src/some_function/Pic_Thread.cpp
        src/some_function/Pic_Thread.h)


target_link_libraries(picture_viewer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        )

