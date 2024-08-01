cmake_minimum_required(VERSION 3.28)
project(SnowElvesScript)

# 获取传递的版本号
if(NOT DEFINED VERSION)
    set(VERSION "v0.0.0")
endif()
#set(VERSION "v0.0.0")
# 添加宏定义
add_definitions(-DPROJECT_VERSION="${VERSION}")
message(STATUS "使用的版本: ${VERSION}")

#set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(THREADS_PREFER_PTHREAD_FLAG OFF)
set(CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/lib/cmake")

# 设置CMake找到OpenCV的路径
set(OpenCV_DIR "D:/opencv")
find_package(OpenCV REQUIRED)




# 遍历 BMP 图像文件夹并生成图片资源的头文件
file(GLOB IMAGE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/images/*.bmp")
set(OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/resources/images.h")


# 包含目录
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}

)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED
)

# Qt 资源
qt6_add_binary_resources(RESOURCE
        resources/snowelves.qrc
        OPTIONS
        -binary
        -no-compress
        -verbose
)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_subdirectory(external/spdlog)

# 可执行文件
add_executable(SnowElvesScript WIN32
        src/main.cpp
        include/main.h
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
        src/utils/Factory.cpp
        include/utils/Factory.h
        src/utils/LoadJsonFile.cpp
        include/utils/LoadJsonFile.h
        src/utils/FunctionLibrary.cpp
        include/utils/FunctionLibrary.h
        ${RESOURCE}
        ${OUTPUT_HEADER}
        src/views/LoginWindow.cpp
        include/views/LoginWindow.h
        src/views/MainWindow.cpp
        include/views/MainWindow.h
        src/views/RenewWindow.cpp
        include/views/RenewWindow.h
        src/models/DownloadThread.cpp
        include/models/DownloadThread.h
        src/models/Tasks/Submersion.cpp
        include/models/Tasks/Submersion.h
        src/utils/Utilities.cpp
        include/utils/Utilities.h
        include/models/Tasks/SwitchRolesTask.h
        src/models/Tasks/SwitchRolesTask.cpp
        src/models/Tasks/PlaceTask.cpp
        include/models/Tasks/PlaceTask.h
        src/models/Tasks/TheSwordTask.cpp
        include/models/Tasks/TheSwordTask.h
        src/models/Tasks/TheSwordThreeTask.cpp
        include/models/Tasks/TheSwordThreeTask.h
        src/models/Tasks/VientianeTask.cpp
        include/models/Tasks/VientianeTask.h
        src/models/Tasks/HeroListTask.cpp
        include/models/Tasks/HeroListTask.h
        src/models/Tasks/DailyCopiesTask.cpp
        include/models/Tasks/DailyCopiesTask.h
        src/models/Tasks/BountyMissionsTask.cpp
        include/models/Tasks/BountyMissionsTask.h
        src/models/Tasks/TeaStoryTask.cpp
        include/models/Tasks/TeaStoryTask.h
        src/models/Tasks/RiverTask.cpp
        include/models/Tasks/RiverTask.h
        src/models/Tasks/DoorBanTask.cpp
        include/models/Tasks/DoorBanTask.h
        src/models/Tasks/BreakBanTask.cpp
        include/models/Tasks/BreakBanTask.h
        src/models/Tasks/DailyRedemptionTask.cpp
        include/models/Tasks/DailyRedemptionTask.h
        src/models/Tasks/SectTask.cpp
        include/models/Tasks/SectTask.h
        src/models/Tasks/ChivalryShoutTask.cpp
        include/models/Tasks/ChivalryShoutTask.h
        src/models/Tasks/FateSwordTask.cpp
        include/models/Tasks/FateSwordTask.h
        src/utils/LocalServer.cpp
        include/utils/LocalServer.h


)

# 添加资源文件
target_sources(SnowElvesScript PRIVATE resources/main.rc)


target_link_libraries(SnowElvesScript PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        cpr::cpr
        spdlog::spdlog
        ${OpenCV_LIBS}

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
