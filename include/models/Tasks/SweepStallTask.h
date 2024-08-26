//
// Created by y1726 on 24-8-22.
//

#ifndef SWEEPSTALLTASK_H
#define SWEEPSTALLTASK_H
#include <unordered_set>

#include "models/Tasks/BasicTask.h"

class SweepStallTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    SweepStallTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;

    void flushed();

private:
    // 坐标映射
    std::map<int, cv::Point> LocationMap{
        {1, {547, 218}},
        {2, {960, 218}},
        {3, {547, 326}},
        {4, {960, 326}},
        {5, {547, 434}},
        {6, {960, 434}},
        {7, {547, 542}},
        {8, {960, 542}},

    };
    // 商品结构体
    struct Product {
        cv::Point location;
        int priority;

        Product(const cv::Point& loc, const int priority_) : location(loc), priority(priority_) {}
    };

    // 自定义比较器：优先级越高的优先（priority 越大，优先级越高）
    struct Compare {
        bool operator()(const Product& a, const Product& b) const {
            return a.priority < b.priority; // 小顶堆，优先级高的靠前
        }
    };

    std::priority_queue<Product, std::vector<Product>, Compare> productQueue;

    std::unordered_set<int> productSet;

    std::jthread thread;

    /**
     * 0. 记录没有商品次数
     */
    std::vector<int> record_num{0};
    //
    // std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point()};
    /**
     * 0. 加速线程控制
     * 1. 控制刷新完成
     */
    std::vector<bool> record_event{false, false};

};
#endif //SWEEPSTALLTASK_H
