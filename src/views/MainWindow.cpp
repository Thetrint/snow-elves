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
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    setMinimumSize(961, 652);

    menu_layout = new QVBoxLayout(this);
    ui.widget->setLayout(menu_layout);

    spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    menu_layout->addSpacerItem(spacer);

    home = new HomeWindow(this);
    addPageAndButton("主页", home);

    script = new ScriptWindow(this);
    addPageAndButton("脚本", script);


    run = new RunWindow(this);
    addPageAndButton("运行", run);

    // connect(RunWindow)

    connect(Signals::instance(), &Signals::writejson, this, &MainWindow::writewinconfig);



    // writewinconfig();
    // switchToPage(1);

}

//容器添加界面
void MainWindow::addPageAndButton(const QString &buttonText, QWidget *page) {

    const int index = getSpacerIndex(spacer);

    // 创建一个QPushButton并将其添加到ui.widget的布局中
    auto *button = new QPushButton(buttonText, this);
    menu_layout->insertWidget(index, button);

    // 将按钮的点击信号连接到槽函数，切换到相应的页面
    connect(button, &QPushButton::clicked, this, [this, page]() {
        ui.stackedWidget_2->setCurrentWidget(page);
    });

    ui.stackedWidget_2->addWidget(page);
}

//获取弹簧索引
int MainWindow::getSpacerIndex(const QSpacerItem *spacer) const {
    for (int i = 0; i < menu_layout->count(); ++i) {
        if (menu_layout->itemAt(i)->spacerItem() == spacer) {
            return i;
        }
    }
    return -1; // 如果没有找到弹簧，则返回-1
}

//写入json


void MainWindow::writewinconfig(int id) const {

    // 创建 JSON 数组并将 QListWidget 中的项直接写入
    Json::Value jsonArray(Json::arrayValue);
    for (int i = 0; i < script->ui.listWidget_2->count(); ++i) {
        QListWidgetItem* item = script->ui.listWidget_2->item(i);
        jsonArray.append(item->text().toStdString());  // 转换为 std::string
    }

    // 创建一个 JSON 对象并添加键值对
    Json::Value root;
    root["执行任务"] = jsonArray;
    root["keyu"] = 30;
    root["city"] = "New York";

    // 将 JSON 对象序列化为字符串
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;
    std::string jsonString = Json::writeString(writer, root);

    // 获取临时目录路径并设置文件路径
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    std::filesystem::path filePath = tempDir / ("ElvesConfig_" + std::to_string(id) + ".json");

    // 打开一个文件以写入 JSON 字符串
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
    }

    // 将 JSON 字符串写入文件
    file << jsonString;

    // 关闭文件
    file.close();


}
