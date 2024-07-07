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
    const QString tempDir = QDir::tempPath();
    const QString filePath = tempDir + "/ElvesConfig_" + QString::number(id) + ".json";

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
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        return;
    }

    // 根据 id 存储不同的 JSON 数据
    switch (id) {
        case 0:
            file_0 = jsonDoc.object();
        break;
        case 1:
            file_1 = jsonDoc.object();
        break;
        case 2:
            file_2 = jsonDoc.object();
        break;
        case 3:
            file_3 = jsonDoc.object();
        break;
        case 4:
            file_4 = jsonDoc.object();
        break;
        case 5:
            file_5 = jsonDoc.object();
        break;
        case 6:
            file_6 = jsonDoc.object();
        break;
        case 7:
            file_7 = jsonDoc.object();
        break;
        case 8:
            file_8 = jsonDoc.object();
        break;
        case 9:
            file_9 = jsonDoc.object();
        break;
        default:
            qDebug() << "Invalid id:" << id;
        break;
    }

    qDebug() << "JSON data read from file:" << filePath;

}



