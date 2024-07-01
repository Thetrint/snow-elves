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
    connect(ui.pushButton_7, &QPushButton::clicked, this, &RunWindow::StartTask);

    ui.tableWidget->setRowCount(10);  // 设置行数为10行
    ui.tableWidget->setColumnCount(2);  // 设置列数为2列

    ui.tableWidget->setHorizontalHeaderLabels({"角色信息", "任务状态"});

    // 设置每列的宽度
    ui.tableWidget->setColumnWidth(0, 150);  // 第一列宽度为200像素
    ui.tableWidget->setColumnWidth(1, 100);  // 第二列宽度为200像素

    // 禁用水平滚动条
    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁用垂直滚动条
    ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    constexpr QSize minSize(280, 660);  // 设置最小大小为宽度600像素，高度400像素
    constexpr QSize maxSize(280, 660);  // 设置最大大小为宽度800像素，高度600像素

    ui.widget->setMinimumSize(minSize);
    ui.widget->setMaximumSize(maxSize);

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

void RunWindow::StartTask() {
    const int id = getrowindex();

    Signals::instance()->emitWriteJson(id);


    HWND hwnd = WindowManager::getWindowHandle();
    std::cout << hwnd << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    // 检查是否已经存在指定 ID 的实例
    if (!instances.contains(id)) {
        // 如果不存在，创建一个新的 MyClass 实例并存储在共享指针中
        const auto instance = std::make_shared<TaskManager>(id, hwnd);

        // 将新创建的实例存储在 instances 容器中
        instances[id] = instance;

        // 创建一个新线程来运行该实例的 operator() 函数
        threads[id] = std::jthread(&TaskManager::start, instance);

    } else {
        // 如果实例已经存在，输出相应的提示消息
        std::cout << "Instance with ID " << id << " already exists.\n";
    }

}

// RunWindow::~RunWindow() {
//     // Qt会自动清理子对象
// }