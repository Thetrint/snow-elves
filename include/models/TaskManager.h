//
// Created by y1726 on 2024/6/26.
//

#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <qwindowdefs_win.h>
#include <semaphore>

#include "models//TaskManager.h"
#include "models/ImageProcess.h"
#include "models/Tasks/BasicTask.h"
#include "Tasks/LessonTask.h"
#include "Tasks/FactionTask.h"


class TaskManager {

public:
    explicit TaskManager(int id, HWND hwnd);

    // 定义一个枚举类型
    enum FunctionType {
        TM_CCORR_NORMED,
        TM_SQDIFF_NORMED,

    };



    [[nodiscard]] std::vector<ImageProcessor::Match> ImageMatch(FunctionType funcType) const;

    void start();

    void stop();

    void pause();

    void resume();


private:
    int id;
    HWND hwnd;

    std::counting_semaphore<1> pause_event;
    std::counting_semaphore<1> unbind_event;
};
#endif //TASKMANAGER_H




