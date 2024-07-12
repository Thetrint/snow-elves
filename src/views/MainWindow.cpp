//
// Created by y1726 on 2024/6/18.
//


#include "views/MainWindow.h"
#include "views/HomeWindow.h"
#include "views/RunWindow.h"
#include "views/ScriptWindow.h"
#include "utils/signals.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    setMinimumSize(961, 652);



    home = new HomeWindow(this);
    addPageAndButton("主页", home);

    script = new ScriptWindow(this);
    addPageAndButton("脚本", script);


    run = new RunWindow(this);
    addPageAndButton("运行", run);

    // connect(RunWindow)
    buttonGroup.setExclusive(true);

    connect(Signals::instance(), &Signals::writejson, this, &MainWindow::writewinconfig);



    // writewinconfig();
    // switchToPage(1);

}

//容器添加界面
void MainWindow::addPageAndButton(const QString &buttonText, QWidget *page) {

    const int index = getSpacerIndex(ui.verticalSpacer);

    // 创建一个QPushButton并将其添加到ui.widget的布局中
    auto *button = new QPushButton(buttonText, this);
    button->setCheckable(true);
    ui.verticalLayout->insertWidget(index, button);

    // 将按钮的点击信号连接到槽函数，切换到相应的页面
    connect(button, &QPushButton::clicked, this, [this, page]() {
        ui.stackedWidget->setCurrentWidget(page);
    });

    buttonGroup.addButton(button);
    ui.stackedWidget->addWidget(page);
}

//获取弹簧索引
int MainWindow::getSpacerIndex(const QSpacerItem *spacer) const {
    for (int i = 0; i < ui.verticalLayout->count(); ++i) {
        if (ui.verticalLayout->itemAt(i)->spacerItem() == spacer) {
            return i;
        }
    }
    return -1; // 如果没有找到弹簧，则返回-1
}

//写入json


void MainWindow::writewinconfig(const int id) const {

    // 创建 JSON 数组并将 QListWidget 中的项直接写入
    QJsonArray jsonArray;
    for (int i = 0; i < script->ui.listWidget_2->count(); ++i) {
        const QListWidgetItem* item = script->ui.listWidget_2->item(i);
        jsonArray.append(item->text());
    }

    // 创建 JSON 对象并添加键值对
    QJsonObject root;
    root["执行任务"] = jsonArray;
    root["keyu"] = 30;
    root["city"] = "New York";

    // 创建 JSON 文档
    const QJsonDocument jsonDoc(root);

    // 将 JSON 文档转换为字符串
    const QByteArray jsonData = jsonDoc.toJson();

    // 获取临时目录路径并设置文件路径
    const QString tempDir = QDir::tempPath();
    const QString filePath = tempDir + "/ElvesConfig_" + QString::number(id) + ".json";

    // 打开文件以写入 JSON 字符串
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << jsonData << Qt::endl;
        file.close();
        qDebug() << "JSON data written to file:" << filePath;
    } else {
        qWarning() << "Failed to open file for writing:" << file.errorString();
    }


}
