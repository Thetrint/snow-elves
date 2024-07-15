//
// Created by y1726 on 24-7-6.
//
#include "models/DownloadThread.h"

#include <iostream>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <utils/Signals.h>
#include "utils/Utilities.h"

void DownloadThread::run(){

    if (name == "Incremental Update") {

        const cpr::Url url{"https://gitee.com/api/v5/repos/IceSnowVersion/snow-elves/raw/SnowElvesScript.exe?access_token=b5127c648ab2025e7911e567a7b8b9c1&ref=release"};

        const cpr::Response d = Get(cpr::Url{url}, cpr::ProgressCallback([this](cpr::cpr_off_t downloadTotal, cpr::cpr_off_t downloadNow, cpr::cpr_off_t uploadTotal, cpr::cpr_off_t uploadNow, intptr_t userdata) -> bool
        {

            emit SetProgressBarSignal(static_cast<int>(downloadTotal));
            emit UpdateProgressBarSignal(static_cast<int>(downloadNow));

            return true;
        }));

        // 检查请求是否成功
        if (d.status_code == 200) {
            // 获取临时目录路径
            const QString tempDir = QDir::tempPath();
            const QString filePath = tempDir + "/SnowElvesScript.exe";

            // 将响应内容写入文件
            std::ofstream outfile(filePath.toStdString(), std::ofstream::binary);
            outfile << d.text;
            outfile.close();

            emit Signals::instance()->Update(name, version);

        }


    }else if(name == "Full Update") {

        // 发起 GET 请求，不需要身份验证
        const cpr::Response r = cpr::Get(cpr::Url{"https://gitee.com/api/v5/repos/IceSnowVersion/snow-elves/releases/" + std::to_string(id) + "/attach_files?access_token=b5127c648ab2025e7911e567a7b8b9c1&page=1&per_page=20"});

        std::cout << r.text << std::endl;

        // 假设 r.text 是你从网络获取的 JSON 字符串
        const QString jsonString = QString::fromStdString(r.text);

        // 解析 JSON
        QJsonParseError error;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

        if (error.error != QJsonParseError::NoError) {
            std::cerr << "Error parsing JSON: " << error.errorString().toStdString() << std::endl;
            return;  // 处理解析错误
        }


        // 获取根数组
        if (!jsonDoc.isArray()) {
            std::cerr << "Error: JSON is not an array." << std::endl;
            return;  // 处理类型错误
        }
        const QJsonArray jsonArray = jsonDoc.array();
        const QJsonObject jsonObj = jsonArray.first().toObject();

        std::cout << "id: " << jsonObj.value("id").toInt() << std::endl;



        const cpr::Url url{"https://gitee.com/api/v5/repos/IceSnowVersion/snow-elves/releases/" + std::to_string(id) + "/attach_files/" + std::to_string(jsonObj.value("id").toInt()) + "/download?access_token=b5127c648ab2025e7911e567a7b8b9c1"};

        const cpr::Response d = Get(cpr::Url{url}, cpr::ProgressCallback([this](cpr::cpr_off_t downloadTotal, cpr::cpr_off_t downloadNow, cpr::cpr_off_t uploadTotal, cpr::cpr_off_t uploadNow, intptr_t userdata) -> bool
        {

            emit SetProgressBarSignal(static_cast<int>(downloadTotal));
            emit UpdateProgressBarSignal(static_cast<int>(downloadNow));

            return true;
        }));

        // 检查请求是否成功
        // 检查请求是否成功
        if (d.status_code == 200) {
            // 获取临时目录路径
            const QString tempDir = QDir::tempPath();
            const QString filePath = tempDir + "/ScriptApp.zip";

            // 将响应内容写入文件
            if (std::ofstream outfile(filePath.toStdString(), std::ofstream::binary); outfile.is_open()) {
                outfile << d.text;
                outfile.close();
                emit Signals::instance()->Update(name, version);
            } else {
                spdlog::error("更新文件写入失败");
            }
        } else {
            spdlog::error("网络请求失败");
        }

    }


}
