#include "main.h"
#include "views/RunWindow.h"
#include "models/WindowManager.h"
#include "utils/Signals.h"

RunWindow::RunWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素


    //任务开始
    connect(Signals::instance(), &Signals::Start, this, [&]() {
        emit ui.pushButton_7->click();
    });

    connect(ui.pushButton_7, &QPushButton::clicked, this, [&]() {
        HWND hwnd;
        if (int id; detectWin(id, hwnd)) {
            Signals::instance()->emitWriteJson(id);

            WindowManager::setWinodw(hwnd);

            // 如果不存在，创建一个新的 MyClass 实例并存储在共享指针中
            const auto instance = std::make_shared<TaskManager>(id, hwnd);

            // 将新创建的实例存储在 instances 容器中
            instances[id] = instance;
            windowHwnd[id] = hwnd;
            try {
                threads[hwnd] = std::jthread(&TaskManager::start, instance);
            } catch (const std::exception& e) {
                spdlog::error("线程创建: {}", e.what());
            }
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
            threads.erase(windowHwnd[id]);
            windowHwnd.erase(id);
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
                instances.erase(id);
                threads.erase(windowHwnd[id]);
                windowHwnd.erase(id);
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

    //日志更新
    connect(Signals::instance(), &Signals::Log, this, [&](const int id, const std::string& message) {
        // 获取当前时间
        try {
            const auto now = std::chrono::system_clock::now();
            const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            const std::tm* now_tm = std::localtime(&now_time);

            // 格式化时间为 "12:34:34"
            std::ostringstream oss;
            oss << std::put_time(now_tm, "%H:%M:%S");
            const std::string time_str = oss.str();
            std::string text = "[" +time_str + "]" + "[窗口 " + std::to_string(id + 1) + "]" + " " + ">>> " + message;
            if (id == -1) {
                text = "[" +time_str + "]"  + " >>> " + message;
            }

            ui.textEdit->append(QString::fromUtf8(text.c_str()));
        } catch (const std::exception& e) {
            spdlog::error("线程创建: {}", e.what());
        }


    });

    //设置角色信息
    connect(Signals::instance(), &Signals::setPersion, this, [&](const int id, HWND hwnd) {
        try {
            spdlog::info("设置角色信息");
            const cv::Mat image = ImageProcessor::HBITMAPToMat(WindowManager::CaptureAnImage(hwnd));
            const cv::Rect roi(120, 730, 115, 20);
            cv::Mat persionImage = image(roi);
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
        } catch (const std::exception& e) {
            spdlog::error("线程创建: {}", e.what());
        }

    });

    //截图
    connect(ui.pushButton_8, &QPushButton::clicked, this, [&](){
        HBITMAP hBitmap = WindowManager::CaptureAnImage(WindowManager::getWindowHandle());
        WindowManager::SaveBitmapToFile(hBitmap, L"1.bmp");
        const cv::Mat mat = ImageProcessor::HBITMAPToMat(hBitmap);



        cv::Mat gray;
        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);

        // 应用高斯模糊
        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 1.2);

        // 应用 Canny 边缘检测
        cv::Mat edges;
        cv::Canny(blurred, edges, 100, 200);
        cv::imwrite("2.bmp", edges);
        // cv::imshow("Source Image", edges);
        // cv::waitKey(0);

        DeleteObject(hBitmap);
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

    // 设置单选模式
    ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 禁用水平滚动条
    ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁用垂直滚动条
    ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}

int RunWindow::getrowindex() const {
    // 假设 tableWidget 是您的 QTableWidget 对象
    if (const QModelIndex currentIndex = ui.tableWidget->currentIndex(); currentIndex.isValid()) {
        const int rowIndex = currentIndex.row();
        return rowIndex;
    }
    return 0;
}


bool RunWindow::detectWin(int &id, HWND &hwnd) {

    id = getrowindex();
    hwnd = WindowManager::getWindowHandle();
    if (hwnd == nullptr) {
        auto *msgBox = new QMessageBox();
        msgBox->setWindowTitle("提示");
        msgBox->setText("请勿绑定非游戏窗口");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setAttribute(Qt::WA_DeleteOnClose); // 自动删除
        msgBox->show();

        // 设置定时器，在一段时间后关闭消息框
        QTimer::singleShot(2000, msgBox, &QMessageBox::close);
        return false;
    }

    if (!threads.contains(hwnd)) {
        if (!instances.contains(id)) {
            return true;
        }
        for (int i = 0; i < 10; i++) {
            if (!instances.contains(i)) {
                id = i;
                return true;
            }
        }
        auto *msgBox = new QMessageBox();
        msgBox->setWindowTitle("提示");
        msgBox->setText("当前没有空余窗口");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setAttribute(Qt::WA_DeleteOnClose); // 自动删除
        msgBox->show();

        // 设置定时器，在一段时间后关闭消息框
        QTimer::singleShot(2000, msgBox, &QMessageBox::close);
        return false;
    }
    auto *msgBox = new QMessageBox();
    msgBox->setWindowTitle("提示");
    msgBox->setText("当前游戏已被绑定");
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setAttribute(Qt::WA_DeleteOnClose); // 自动删除
    msgBox->show();

    // 设置定时器，在一段时间后关闭消息框
    QTimer::singleShot(2000, msgBox, &QMessageBox::close);
    return false;
}

// RunWindow::~RunWindow() {
//     // Qt会自动清理子对象
// }