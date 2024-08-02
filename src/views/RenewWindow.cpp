//
// Created by y1726 on 24-7-6.
//
#include "views/RenewWindow.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <cpr/cpr.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <iostream>
#include <qabstractbutton.h>
#include <QJsonArray>
#include <QMessageBox>
#include <QTimer>
#include <utils/Signals.h>


RenewWindow::RenewWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素
    ui.progressBar->setRange(0, 0);  // 设置进度条的范围，这里是 0 到 100
    ui.progressBar->setValue(0);


}

void RenewWindow::checkupdae() {
    // 发起 GET 请求
    const cpr::Response r = cpr::Get(cpr::Url{"https://gitee.com/api/v5/repos/IceSnowVersion/snow-elves/releases?access_token=b5127c648ab2025e7911e567a7b8b9c1&page=1&per_page=20&direction=desc"});

    if (r.status_code != 200) {
        std::cerr << "Failed to fetch releases. Status code: " << r.status_code << std::endl;
        emit login();
        return;
    }

    // 转换为 QString
    const QString jsonString = QString::fromStdString(r.text);

    // 解析 JSON
    QJsonParseError error;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        std::cerr << "Error parsing JSON: " << error.errorString().toStdString() << std::endl;
        return;
    }

    // // 读取本地版本号
    // std::string local_version = "v0.0.0"; // 默认版本号
    // std::ifstream input_file("version.txt");
    // if (input_file) {
    //     std::getline(input_file, local_version);
    //     input_file.close();
    // } else {
    //     std::cerr << "Could not open version.txt, using default version: " << local_version << std::endl;
    // }
    //
    //
    // // 输出版本号到控制台
    // std::cout << "Version: " << local_version << std::endl;
    emit Signals::instance()->Log(-1, "目前版本：" + std::string(PROJECT_VERSION));

    // 遍历 JSON 数据找出比本地版本新的版本
    const QJsonArray releases = jsonDoc.array();
    std::vector<ReleaseInfo> newer_versions;

    for (const auto& release : releases) {
        const QJsonObject releaseObj = release.toObject();
        std::string version = releaseObj["tag_name"].toString().toStdString();
        int id = releaseObj["id"].toInt();
        std::string name = releaseObj["name"].toString().toStdString();
        std::string body = releaseObj["body"].toString().toStdString();
        if (compareVersions(PROJECT_VERSION, version)) {
            std::cout << "Version: " << version << ", ID: " << id << ", Name: " << name << ", Body: " << body << std::endl;
            newer_versions.push_back({version, id, name, body});
        }
    }


    std::string selected_version;
    int selected_id{};
    std::string selected_name;
    std::string selected_body;

    // 输出所有新版本
    if (!newer_versions.empty()) {
        std::cout << "Newer versions found:" << std::endl;
        bool found_full = false;
        for (const auto& [version, id, name, body] : newer_versions) {

            if (name == "Full Update" && !found_full) {
                selected_version = version;
                selected_id = id;
                selected_name = name;
                selected_body = body;
                found_full = true;
            }
        }
        if (!found_full) {
            const auto& [version, id, name, body] = newer_versions.front();
            selected_version = version;
            selected_id = id;
            selected_name = name;
            selected_body = body;
        }
    } else {
        std::cout << "No newer versions found." << std::endl;
    }

    if (!newer_versions.empty()) {
        // 创建一个消息框
        const auto msgBox = std::make_unique<QMessageBox>();
        msgBox->setWindowTitle("更新窗口");
        msgBox->setText(QString::fromStdString(selected_body));
        msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox->setDefaultButton(QMessageBox::Yes);

        // 定时器在5秒后检查用户是否已经响应
        QTimer::singleShot(5000, msgBox.get(), [msgBoxPtr = msgBox.get()]() {
            if (msgBoxPtr->isVisible()) {
                // 如果用户没有响应，自动选择 "Yes"
                msgBoxPtr->button(QMessageBox::Yes)->click();
            }
        });

        //等待用户选择
        if (const int ret = msgBox->exec(); ret == QMessageBox::Yes) {
            msgBox->deleteLater();
            std::cout << "name: " << selected_name << std::endl;
            std::cout << "id: " << selected_id << std::endl;
            show();

            auto* downloadThread = new DownloadThread(selected_name, selected_id, selected_version, this);

            connect(downloadThread, &DownloadThread::SetProgressBarSignal, this, [&](const int max) {
                ui.progressBar->setRange(0, max);
            });
            connect(downloadThread, &DownloadThread::UpdateProgressBarSignal, this, [&](const int value) {
                ui.progressBar->setValue(value);
            });
            downloadThread->start();
        } else if (ret == QMessageBox::No) {
            emit login();
        }


    }else {
        emit login();
    }





}
// 版本号比较函数
// 返回值：version1 > version2 返回 true，否则返回 false
bool RenewWindow::compareVersions(const std::string& version1, const std::string& version2) {


    // 将版本号分割成独立的数字部分进行比较
    auto split = [](const std::string& version) {
        std::vector<int> parts;
        // 如果版本号以 'v' 开头，则移除它
        std::string ss1(version.substr(1));

        size_t start = 0, end = 0;
        while ((end = ss1.find('.', start)) != std::string::npos) {
            parts.push_back(std::stoi(ss1.substr(start, end - start)));
            start = end + 1;
        }
        parts.push_back(std::stoi(ss1.substr(start)));
        return parts;
    };

    const std::vector<int> v1 = split(version1);
    const std::vector<int> v2 = split(version2);
    for (size_t i = 0; i < std::max(v1.size(), v2.size()); ++i) {
        const int num1 = i < v1.size() ? v1[i] : 0;
        if (const int num2 = i < v2.size() ? v2[i] : 0; num1 != num2) {
            return num1 < num2;
        }
    }
    return false; // 版本号相同
}

void RenewWindow::UpdateProgressBar(const int val) const {
    ui.progressBar->setValue(val);
}

void RenewWindow::SetProgressBar(const int max) const {
    ui.progressBar->setRange(0, max);
}
