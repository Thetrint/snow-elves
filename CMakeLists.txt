cmake_minimum_required(VERSION 3.28)
project(SnowElvesScript)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_COMPILER "E:/MinGW/bin/g++.exe")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "E:/Qt/6.7.2/mingw_64/lib/cmake")

# 设置CMake找到OpenCV的路径
set(OpenCV_DIR "E:/opencv")
find_package(OpenCV REQUIRED)

# 设置jsoncpp的路径
set(JSONCPP_DIR "E:/jsoncpp")



# 遍历 BMP 图像文件夹并生成图片资源的头文件
file(GLOB IMAGE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/images/*.bmp")
set(OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/resources/images.h")

# 生成图片资源的头文件
file(WRITE ${OUTPUT_HEADER} "#ifndef IMAGE_RESOURCES_H_\n#define IMAGE_RESOURCES_H_\n\n")
set(IMAGE_MAP_CONTENT "")
set(IMAGE_SIZE_MAP_CONTENT "")

foreach(img ${IMAGE_FILES})
    get_filename_component(img_name ${img} NAME_WE)
    string(MD5 img_hash ${img_name})
    set(img_var_name "img_${img_hash}")

    message(STATUS "Processing image: ${img_name}")

    file(READ ${img} img_content HEX)
    string(REGEX REPLACE "(..)" "0x\\1, " img_content "${img_content}")
    string(REGEX REPLACE ", $" "" img_content "${img_content}")

    file(READ ${img} IMG_FILE_CONTENT HEX)
    string(LENGTH ${IMG_FILE_CONTENT} IMG_FILE_SIZE)

    list(APPEND IMAGE_MAP_CONTENT "    { L\"${img_name}\", ${img_var_name} },\n")
    list(APPEND IMAGE_SIZE_MAP_CONTENT "    { L\"${img_name}_size\", ${img_var_name}_size },\n")

    file(APPEND ${OUTPUT_HEADER} "const unsigned char ${img_var_name}[] = { ${img_content} };\n\n")
    file(APPEND ${OUTPUT_HEADER} "const size_t ${img_var_name}_size = sizeof(${img_var_name});\n\n")
endforeach()

file(APPEND ${OUTPUT_HEADER} "static std::map<std::wstring, const unsigned char*> Images = {\n")
foreach(entry ${IMAGE_MAP_CONTENT})
    file(APPEND ${OUTPUT_HEADER} "${entry}")
endforeach()
file(APPEND ${OUTPUT_HEADER} "};\n\n")

file(APPEND ${OUTPUT_HEADER} "static std::map<std::wstring, const size_t> Images_size = {\n")
foreach(entry ${IMAGE_SIZE_MAP_CONTENT})
    file(APPEND ${OUTPUT_HEADER} "${entry}")
endforeach()
file(APPEND ${OUTPUT_HEADER} "};\n\n")
file(APPEND ${OUTPUT_HEADER} "#endif // IMAGE_RESOURCES_H_\n")

# 包含目录
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${JSONCPP_DIR}/include
)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

# Qt 资源
qt6_add_binary_resources(RESOURCE
        resources/snowelves.qrc
        OPTIONS
        -no-compress
        -verbose
)

# 可执行文件
add_executable(SnowElvesScript WIN32
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
        src/views/ScriptWindow.cpp
        include/views/ScriptWindow.h
        src/models/Tasks/LessonTask.cpp
        include/models/Tasks/LessonTask.h
        include/models/Tasks/BasicTask.h
        src/models/Tasks/BasicTask.cpp
        src/models/Tasks/FactionTask.cpp
        include/models/Tasks/FactionTask.h
        src/models/TaskSchedul.cpp
        include/models/TaskSchedul.h
        src/utils/signals.cpp
        include/utils/signals.h
        include/utils/Factory.h
        src/utils/Factory.cpp
        src/utils/LoadJsonFile.cpp
        include/utils/LoadJsonFile.h
        src/utils/FunctionLibrary.cpp
        include/utils/FunctionLibrary.h
        ${RESOURCE}
        ${OUTPUT_HEADER}
)


target_link_libraries(SnowElvesScript
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OpenCV_LIBS}
        ${JSONCPP_DIR}/lib/libjsoncpp.a
)



# 处理 Windows 平台的特定设置
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
