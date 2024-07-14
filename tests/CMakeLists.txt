set(CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/lib/cmake")
set(OpenCV_DIR "D:/opencv")
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)
# 添加可执行文件
add_executable(test_exec
        tests.cpp
        ../src/models/ImageProcess.cpp
        ../include/models/ImageProcess.h
        ../src/models/WindowManager.cpp
        ../include/models/WindowManager.h

)

target_link_libraries(test_exec PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OpenCV_LIBS}
)