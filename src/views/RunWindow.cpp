#include "views/RunWindow.h"
#include "models/WindowManager.h"
#include "utils/Signals.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <iostream>
#include <thread>
#include <QTableWidget>

RunWindow::RunWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    //任务开始
    connect(ui.pushButton_7, &QPushButton::clicked, this, [&]() {
        const int id = getrowindex();
        Signals::instance()->emitWriteJson(id);
        HWND hwnd = WindowManager::getWindowHandle();
        std::cout << hwnd << std::endl;
        WindowManager::setWinodw(hwnd);


        const cv::Mat image = ImageProcessor::HBITMAPToMat(WindowManager::CaptureAnImage(hwnd));
        const cv::Rect roi(120, 730, 115, 20);
        cv::Mat persionImage = image(roi);
        // cv::imshow("11", image_1);
        // cv::imshow("12", image);
        // cv::imshow("13", persionImage);
        // cv::waitKey(0);
        // 将 cv::Mat 转换为 QImage
        QImage qImage;
        if (persionImage.type() == CV_8UC3) {
            cv::cvtColor(persionImage, persionImage, cv::COLOR_BGR2RGB);
            qImage = QImage(persionImage.data, static_cast<int>(persionImage.cols), static_cast<int>(persionImage.rows),
                            static_cast<int>(persionImage.step), QImage::Format_RGB888);
        } else if (persionImage.type() == CV_8UC1) {
            qImage = QImage(persionImage.data, static_cast<int>(persionImage.cols), static_cast<int>(persionImage.rows),
                            static_cast<int>(persionImage.step), QImage::Format_Grayscale8);
        } else if (persionImage.type() == CV_8UC4) {
            qImage = QImage(persionImage.data, static_cast<int>(persionImage.cols), static_cast<int>(persionImage.rows),
                            static_cast<int>(persionImage.step), QImage::Format_ARGB32);
        }

        // 将 QImage 转换为 QPixmap
        const QPixmap pixmap = QPixmap::fromImage(qImage);

        // 创建一个 QTableWidgetItem 并设置图像数据
        const auto item = new QTableWidgetItem();
        item->setData(Qt::DecorationRole, pixmap);

        // 将 QTableWidgetItem 添加到 QTableWidget
        ui.tableWidget->setItem(id, 0, item);


        // 检查是否已经存在指定 ID 的实例
        if (!instances.contains(id)) {
            // 如果不存在，创建一个新的 MyClass 实例并存储在共享指针中
            const auto instance = std::make_shared<TaskManager>(id, hwnd);

            // 将新创建的实例存储在 instances 容器中
            instances[id] = instance;

            threads[id] = std::jthread(&TaskManager::start, instance);
        }
    });

    //任务解绑
    connect(ui.pushButton_5, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
            item->setData(Qt::DecorationRole, QVariant());
        }

        if (instances.contains(id)) {
            instances[id]->stop();
            instances.erase(id);
        }

    });


    //任务全部解绑
    connect(ui.pushButton_6, &QPushButton::clicked, this, [&](){

        for(int id = 0; id < 10; id++) {
            if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
                item->setData(Qt::DecorationRole, QVariant());
            }
            if (instances.contains(id)) {
                instances[id]->stop();
            }
        }

    });


    //任务暂停
    connect(ui.pushButton, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); instances.contains(id)) {
            instances[id]->pause();
        }
    });

    //任务全部暂停
    connect(ui.pushButton_2, &QPushButton::clicked, this, [&](){

        for(int id = 0; id < 10; id++) {
            if (instances.contains(id)) {
                instances[id]->pause();
            }
        }
    });

    //任务恢复
    connect(ui.pushButton_3, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); instances.contains(id)) {
            instances[id]->resume();
        }
    });

    //任务全部恢复
    connect(ui.pushButton_4, &QPushButton::clicked, this, [&](){

        for(int id = 0; id < 10; id++) {
            if (instances.contains(id)) {
                instances[id]->resume();
            }
        }
    });

    ui.tableWidget->setRowCount(10);  // 设置行数为10行
    ui.tableWidget->setColumnCount(2);  // 设置列数为2列

    ui.tableWidget->setHorizontalHeaderLabels({"角色信息", "任务状态"});

    // 设置每列的宽度
    ui.tableWidget->setColumnWidth(0, 120);  // 第一列宽度为200像素
    ui.tableWidget->setColumnWidth(1, 100);  // 第二列宽度为200像素

     //设置每列的高度
    for (int i = 0; i < ui.tableWidget->rowCount(); ++i) {
        ui.tableWidget->setRowHeight(i, 20);
    }

    // 禁用水平滚动条
    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁用垂直滚动条
    ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}

int RunWindow::getrowindex() const {
    // 假设 tableWidget 是您的 QTableWidget 对象
    QModelIndex currentIndex = ui.tableWidget->currentIndex();
    if (currentIndex.isValid()) {
        const int rowIndex = currentIndex.row();
        return rowIndex;
    }
    return 0;
}




// RunWindow::~RunWindow() {
//     // Qt会自动清理子对象
// }