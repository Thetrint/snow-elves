//
// Created by y1726 on 2024/7/1.
//
#include "utils/LoadJsonFile.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QJsonParseError>
#include <QDebug>

// 定义 instance() 方法
LoadJsonFile& LoadJsonFile::instance() {
    static LoadJsonFile load_json_file;
    return load_json_file;
}


void LoadJsonFile::LoadFile(const int id) {
    // 获取临时目录路径并设置文件路径
    const QString filePath = QDir::tempPath() + "/ElvesConfig_" + QString::number(id) + ".json";

    // 打开文件以读取 JSON 数据
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    // 读取 JSON 数据
    const QByteArray jsonData = file.readAll();
    file.close();

    // 解析 JSON 数据
    QJsonParseError parseError;
    const auto jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        return;
    }

    // 存储 JSON 数据到 QMap 中
    jsonFiles[id] = jsonDoc.object();

}



