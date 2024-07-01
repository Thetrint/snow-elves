//
// Created by y1726 on 2024/6/30.
//
#include "utils/Signals.h"

// 初始化静态成员变量
Signals* Signals::_instance = nullptr;

// 私有构造函数
Signals::Signals(QObject *parent) : QObject(parent) {
    // 初始化代码（如有需要）
}

// 获取单例实例的方法
Signals* Signals::instance() {
    if (!_instance) {
        _instance = new Signals();
    }
    return _instance;
}

// 发射 itemSelected 信号的方法
void Signals::emitWriteJson(int id) {
    emit writejson(id);
}