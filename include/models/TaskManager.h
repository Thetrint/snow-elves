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


    void start();

    void stop();

    void pause();

    void resume();

    void setState(const std::string &task) const;

private:
    int id;
    HWND hwnd;
    bool disrupted;
    std::mutex pause_event;
    bool unbind_event;
};
#endif //TASKMANAGER_H




