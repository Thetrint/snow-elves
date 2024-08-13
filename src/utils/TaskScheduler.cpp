//
// Created by y1726 on 24-8-4.
//
#include "utils/TaskScheduler.h"



void TaskScheduler::addTask(const TimerTask &timerTask) {
    timerTaskList.push_back(timerTask);
}