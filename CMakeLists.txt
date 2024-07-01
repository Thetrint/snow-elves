cmake_minimum_required(VERSION 3.28)
project(SnowElves VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "E:/MinGW/bin/g++.exe")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "E:/Qt/6.7.2/mingw_64/lib/cmake")

# 设置 jsoncpp 的安装路径
set(JSONCPP_DIR "C:/Program Files (x86)/jsoncpp")


# 设置OpenCV的路径，修改为你的OpenCV安装路径
set(OpenCV_DIR E:/opencv-4.10.0/install)



find_package(OpenCV REQUIRED)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

# Set include directories for the project
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${JSONCPP_DIR}/include
        ${OpenCV_DIR}
)

qt6_add_binary_resources(RESOURCE
        resources/snowelves.qrc
        OPTIONS
        -no-compress
        -verbose
)

# 添加库目录
link_directories(
        ${JSONCPP_DIR}/lib
)

add_executable(SnowElves WIN32
        src/main.cpp
        tests/tests.cpp
        src/views/MainWindow.cpp
        include/views/MainWindow.h
        src/views/HomeWindow.cpp
        include/views/HomeWindow.h
        src/views/RunWindow.cpp
        include/views/RunWindow.h
        src/models/WindowManager.cpp
        include/models/WindowManager.h
        src/models/TaskManager.cpp
        include/models/TaskManager.h
        src/models/ImageProcess.cpp
        include/models/ImageProcess.h
        src/views/ScriptWIndow.cpp
        include/views/ScriptWindow.h
        src/models/Tasks/LessonTask.cpp
        include/models/Tasks/LessonTask.h
        include/models/Tasks/BasicTask.h
        src/models/Tasks/BasicTask.cpp
        src/models/Tasks/FactionTask.cpp
        include/models/Tasks/FactionTask.h
        src/models/TaskSchedul.cpp
        include/models/TaskSchedul.h
        ${RESOURCE}
        src/utils/signals.cpp
        include/utils/signals.h
        include/utils/Factory.h
        src/utils/Factory.cpp
        src/utils/LoadJsonFile.cpp
        include/utils/LoadJsonFile.h
        src/utils/FunctionLibrary.cpp
        include/utils/FunctionLibrary.h
)



target_link_libraries(SnowElves
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OpenCV_LIBS}
        jsoncpp
)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
