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
        // 发起 GET 请求，不需要身份验证
    const cpr::Response r = cpr::Get(cpr::Url{"https://gitee.com/api/v5/repos/IceSnowVersion/snow-elves/releases/latest?access_token=b5127c648ab2025e7911e567a7b8b9c1"});

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

    // 读取版本号
    std::string local_version;
    // 创建文件输入流对象
    std::ifstream input_file("version.txt");

    // 检查文件是否成功打开
    if (!input_file) {
        local_version = "v0.0.0";
    }

    std::getline(input_file, local_version);

    // 输出版本号到控制台
    std::cout << "Version: " << local_version << std::endl;

    // 关闭文件流
    input_file.close();

    // 获取根对象
    const QJsonObject jsonObj = jsonDoc.object();
    if (const QString version = jsonObj.value("tag_name").toString(); compareVersions(version.toStdString(), local_version)) {
        // 创建一个消息框
        auto msgBox = std::make_unique<QMessageBox>();
        msgBox->setWindowTitle("更新窗口");
        msgBox->setText(jsonObj.value("body").toString());
        msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox->setDefaultButton(QMessageBox::Yes);

        // 定时器在5秒后检查用户是否已经响应
        QTimer::singleShot(5000, msgBox.get(), [msgBoxPtr = msgBox.get()]() {
            if (msgBoxPtr->isVisible()) {
                // 如果用户没有响应，自动选择 "Yes"
                msgBoxPtr->button(QMessageBox::Yes)->click();
            }
        });


        if (const int ret = msgBox->exec(); ret == QMessageBox::Yes) {
            msgBox->deleteLater();
            std::cout << "name: " << jsonObj.value("name").toString().toStdString() << std::endl;
            std::cout << "id: " << jsonObj.value("id").toInt() << std::endl;

            // // 创建一个输出文件流对象
            // std::ofstream outFile("version.txt");
            //
            // // 检查文件是否成功打开
            // if (!outFile) {
            //     std::cerr << "无法打开文件" << std::endl;
            // }
            //
            // // 写入数据到文件
            // outFile << jsonObj.value("tag_name").toString().toStdString() << std::endl;
            //
            // // 关闭文件
            // outFile.close();

            show();

            auto* downloadThread = new DownloadThread(jsonObj.value("name").toString().toStdString(), jsonObj.value("id").toInt(), jsonObj.value("tag_name").toString().toStdString(), this);

            connect(downloadThread, &DownloadThread::SetProgressBarSignal, this, [=](const int max) {
                ui.progressBar->setRange(0, max);
            });
            connect(downloadThread, &DownloadThread::UpdateProgressBarSignal, this, [=](const int value) {
                ui.progressBar->setValue(value);
            });
            downloadThread->start();
        } else if (ret == QMessageBox::No) {
            emit login();
        }

        msgBox->deleteLater();
    }else {
        emit login();
    }




}
// 版本号比较函数
// 返回值：version1 > version2 返回 true，否则返回 false
bool RenewWindow::compareVersions(const std::string& version1, const std::string& version2) {
    std::istringstream ss1(version1.substr(1)); // 去掉开头的 'v'
    std::istringstream ss2(version2.substr(1)); // 去掉开头的 'v'

    int num1, num2;
    char dot = '.';

    // 当两个字符串流都没有读完时继续循环
    while (ss1.good() || ss2.good()) {
        if (ss1.good()) {
            ss1 >> num1;
        } else {
            num1 = 0; // 如果ss1读完了，则设置为0
        }

        if (ss2.good()) {
            ss2 >> num2;
        } else {
            num2 = 0; // 如果ss2读完了，则设置为0
        }

        if (num1 > num2) {
            return true;
        }
        if(num1 < num2) {
            return false;
        }

        // 跳过点号
        ss1 >> dot;
        ss2 >> dot;
    }

    return false; // 如果版本号完全相同，返回false

}

void RenewWindow::UpdateProgressBar(const int val) const {
    ui.progressBar->setValue(val);
}

void RenewWindow::SetProgressBar(const int max) const {
    ui.progressBar->setRange(0, max);
}
