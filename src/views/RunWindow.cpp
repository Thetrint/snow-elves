#include "main.h"
#include "views/RunWindow.h"
#include <QProcess>
#include <utils/LocalServer.h>

#include "models/WindowManager.h"
#include "utils/Signals.h"

RunWindow::RunWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    // 使用 Lambda 表达式作为槽函数
    //任务开始
    connect(Signals::instance(), &Signals::Start, this, [&]() {
        emit ui.pushButton_7->click();
    });

    connect(ui.pushButton_7, &QPushButton::clicked, this, [&]() {
        HWND hwnd;
        std::wstring wintitle;
            if (int id; detectWin(id, hwnd, wintitle)) {
            Signals::instance()->emitWriteJson(id);
            std::wcout << wintitle << std::endl;
            if (wintitle == L"一梦江湖") {
                std::stringstream ss;
                ss << std::hex << std::setw(8) << std::setfill('0') << reinterpret_cast<uintptr_t>(hwnd);
                std::string hwndStr = ss.str();

                QString filePath = "./id.txt";

                // 打开文件
                std::ofstream file(filePath.toStdString(), std::ios::out);
                if (!file.is_open()) {
                    std::cerr << "无法打开文件 " << filePath.toStdString() << std::endl;
                    return;
                }

                file << id << std::endl;

                file << hwndStr << std::endl; // 添加换行符

                // 写入 FACTOR
                file << FACTOR << std::endl;

                // 关闭文件
                file.close();

                std::string exePath = R"(./GameWindow.exe)";


                // 构建 PowerShell 命令字符串，添加 -WindowStyle Hidden 来隐藏窗口
                std::string command = "powershell -Command \"Start-Process '" + exePath + "' -WindowStyle Hidden -Verb RunAs\"";
                // std::string command = "powershell -Command \"Start-Process '" + exePath + "' -Verb RunAs\"";
                // 使用 std::system 执行命令

                if (int result = std::system(command.c_str()); result == 0) {
                    std::cout << "Process started successfully." << std::endl;
                } else {
                    std::cerr << "Failed to start process. Error code: " << result << std::endl;
                }

            }


            // 如果不存在，创建一个新的 MyClass 实例并存储在共享指针中
            const auto instance = std::make_shared<TaskManager>(id, hwnd);
            winHwnd.push_back(hwnd);
            managerDictionary[id] = {instance, std::jthread(&TaskManager::start, instance), hwnd};


        }

    });

    //任务解绑
    connect(ui.pushButton_5, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
            item->setData(Qt::DecorationRole, QVariant());
        }

        if (managerDictionary.contains(id)) {
            managerDictionary[id].instance->stop();
            // restoreWindowAttributes(managerDictionary[id].windowHwnd, managerDictionary[id].originalAttributes);
            winHwnd.remove(managerDictionary[id].windowHwnd);
            // 从游戏窗口映射中删除
            managerDictionary.erase(id);

            LocalServer::getInstance().connections[id]->write("Close");
            LocalServer::getInstance().connections[id]->flush();

            LocalServer::getInstance().connections.erase(id);

        }


    });


    //任务全部解绑
    connect(Signals::instance(), &Signals::Close, this, [&]() {
        emit ui.pushButton_6->click();
    });

    connect(ui.pushButton_6, &QPushButton::clicked, this, [&](){

        for (const auto&[id, data] : managerDictionary) {

            if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
                item->setData(Qt::DecorationRole, QVariant());
            }

            data.instance->stop();
            // restoreWindowAttributes(managerDictionary[id].windowHwnd, managerDictionary[id].originalAttributes);
            winHwnd.remove(data.windowHwnd);

            LocalServer::getInstance().connections[id]->write("Close");
            LocalServer::getInstance().connections[id]->flush();


        }

        LocalServer::getInstance().connections.clear();
        managerDictionary.clear();


    });


    //任务暂停
    connect(ui.pushButton, &QPushButton::clicked, this, [&](){

        if (const int id = getrowindex(); managerDictionary.contains(id)) {
            managerDictionary[id].instance->pause();
        }
    });

    //任务全部暂停
    connect(ui.pushButton_2, &QPushButton::clicked, this, [&](){
        for (const auto&[id, data] : managerDictionary) {
            data.instance->pause();
        }

    });

    //任务恢复
    connect(ui.pushButton_3, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); managerDictionary.contains(id)) {
            managerDictionary[id].instance->resume();
        }
    });

    //任务全部恢复
    connect(ui.pushButton_4, &QPushButton::clicked, this, [&](){
        for (const auto&[id, data] : managerDictionary) {
            data.instance->resume();
        }

    });

    //任务隐藏
    connect(ui.pushButton_9, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); LocalServer::getInstance().connections.contains(id)) {
            LocalServer::getInstance().connections[id]->write("Hide");
            LocalServer::getInstance().connections[id]->flush();

        }

    });

    //任务全部隐藏
    connect(ui.pushButton_10, &QPushButton::clicked, this, [&](){
        for (const auto& [id, connection] : LocalServer::getInstance().connections) {
            connection->write("Hide");
            connection->flush();
        }

    });

    //任务虚化
    connect(ui.pushButton_13, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); LocalServer::getInstance().connections.contains(id)) {
            LocalServer::getInstance().connections[id]->write("De_Focusing");
            LocalServer::getInstance().connections[id]->flush();

        }

    });

    //任务全部虚化
    connect(ui.pushButton_14, &QPushButton::clicked, this, [&](){
        for (const auto& [id, connection] : LocalServer::getInstance().connections) {
            connection->write("De_Focusing");
            connection->flush();
        }


    });

    //任务显示
    connect(ui.pushButton_11, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); LocalServer::getInstance().connections.contains(id)) {
            LocalServer::getInstance().connections[id]->write("Show");
            LocalServer::getInstance().connections[id]->flush();

        }
    });

    //任务全部虚化
    connect(ui.pushButton_12, &QPushButton::clicked, this, [&](){
        for (const auto& [id, connection] : LocalServer::getInstance().connections) {
            connection->write("Show");
            connection->flush();
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
        std::wstring wintitle;
        HWND hwnd = WindowManager::getWindowHandle(wintitle);
        // WindowManager::setWinodw(hwnd);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        HBITMAP hBitmap = WindowManager::CaptureAnImage(hwnd);
        auto name = std::chrono::system_clock::now().time_since_epoch().count();
        WindowManager::SaveBitmapToFile(hBitmap, std::format(L"Testing/{}.bmp", name).c_str());
        const cv::Mat mat = ImageProcessor::HBITMAPToMat(hBitmap);



        cv::Mat gray;
        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);

        // 应用高斯模糊
        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 1.2);

        // 应用 Canny 边缘检测
        cv::Mat edges;
        cv::Canny(blurred, edges, 100, 200);
        cv::imwrite(std::format("Testing/{}_edges.bmp", name), edges);
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


bool RunWindow::detectWin(int &id, HWND &hwnd, std::wstring& wintitle) {

    id = getrowindex();
    hwnd = WindowManager::getWindowHandle(wintitle);
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

    if (const auto it = std::ranges::find(winHwnd, hwnd); it == winHwnd.end()) {
        if (!managerDictionary.contains(id)) {
            return true;
        }

        for (int i = 0; i < 10; i++) {
            if (!managerDictionary.contains(i)) {
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

