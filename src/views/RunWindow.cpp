#include "main.h"
#include "views/RunWindow.h"
#include <utils/LocalServer.h>
#include <views/MainWindow.h>

#include "models/WindowManager.h"
#include "utils/Signals.h"

RunWindow::RunWindow(QWidget *parent):
    QWidget(parent),
    mainWindow(nullptr)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    // 游戏窗口启动完成
    connect(&LocalServer::instance(), &LocalServer::newClientConnected, this, [&](QLocalSocket *clientSocket) {
        std::lock_guard<std::mutex> lock(mtxQueue);
        // 取出正在等待任务
        auto [id, hwnd] = idHWNDQueue.front();
        // 移除已处理的任务
        idHWNDQueue.pop();
        // 建立连接映射
        LocalServer::instance().setConnect(id, clientSocket);
        // 回传 id_hwnd_factor
        LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("id_hwnd_factor#{}:{}:{}", id, reinterpret_cast<uintptr_t>(hwnd), FACTOR)));

    });

    // 启动任务
    connect(&LocalServer::instance(), &LocalServer::Start, this, [&](const int& id, const HWND& hwnd) {
        const auto instance = std::make_shared<TaskManager>(id, hwnd, mainWindow->createJsonDocument());
        mapManagerData[id].instance = instance;
        mapManagerData[id].thread = std::jthread(&TaskManager::start, instance);
        mapManagerData[id].hwnd = hwnd;
        std::cout << "任务启动" << std::endl;
    });

    // 使用 Lambda 表达式作为槽函数
    // 任务开始
    connect(Signals::instance(), &Signals::Start, this, [&]() {
        SHOUCT = true;
        emit ui.pushButton_7->click();
        SHOUCT = false;
    });

    connect(ui.pushButton_7, &QPushButton::clicked, this, [&]() {
        HWND hwnd;
        std::wstring wintitle;
        if (int id; detectWin(id, hwnd, wintitle)) {

            // 游戏窗口路径
            const std::string exePath = "GameWindow.exe";

            // 使用 ShellExecute 启动程序并隐藏窗口，且不等待
            SHELLEXECUTEINFOA shExecInfo = { 0 };
            shExecInfo.cbSize = sizeof(shExecInfo);
            shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            shExecInfo.lpVerb = "open";
            shExecInfo.lpFile = exePath.c_str();
            shExecInfo.lpParameters = "";
            shExecInfo.nShow = SW_HIDE;

            if (ShellExecuteExA(&shExecInfo)) {
                // 启动成功，不等待直接返回
                std::cout << "Process started." << std::endl;
            } else {
                std::cerr << "Failed to start process. Error: " << GetLastError() << std::endl;
            }

            hwndSet.insert(hwnd);

            // 进入队列等待任务开始
            idHWNDQueue.emplace(id, hwnd);
            mapManagerData[id] = ManagerData();
        }

    });

    //任务解绑
    connect(ui.pushButton_5, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        // 清楚状态信息
        if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
            item->setData(Qt::DecorationRole, QVariant());
        }

        if(QTableWidgetItem* item = ui.tableWidget->item(id, 1)) {
            item->setText("");
        }


        if (mapManagerData.contains(id)) {
            mapManagerData[id].instance->stop();
            mapManagerData[id].isActive = false;
            hwndSet.erase(mapManagerData[id].hwnd);
            LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Close")));
        }
    });


    //任务全部解绑
    connect(Signals::instance(), &Signals::Close, this, [&]() {
        emit ui.pushButton_6->click();
    });

    connect(ui.pushButton_6, &QPushButton::clicked, this, [&](){
        for (auto&[id, data] : mapManagerData) {

            if(QTableWidgetItem* item = ui.tableWidget->item(id, 0)) {
                item->setData(Qt::DecorationRole, QVariant());
            }

            if(QTableWidgetItem* item = ui.tableWidget->item(id, 1)) {
                item->setText("");
            }

            data.instance->stop();
            data.isActive = false;
            hwndSet.erase(mapManagerData[id].hwnd);
            LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Close")));
        }

    });


    //任务暂停
    connect(ui.pushButton, &QPushButton::clicked, this, [&](){

        if (const int id = getrowindex(); mapManagerData.contains(id)) {
            mapManagerData[id].instance->pause();
        }
    });

    //任务全部暂停
    connect(ui.pushButton_2, &QPushButton::clicked, this, [&](){
        for (const auto&[id, data] : mapManagerData) {
            data.instance->pause();
        }

    });

    //任务恢复
    connect(ui.pushButton_3, &QPushButton::clicked, this, [&](){
        if (const int id = getrowindex(); mapManagerData.contains(id)) {
            mapManagerData[id].instance->resume();
        }
    });

    //任务全部恢复
    connect(ui.pushButton_4, &QPushButton::clicked, this, [&](){
        for (const auto&[id, data] : mapManagerData) {
            data.instance->resume();
        }
    });

    //任务隐藏
    connect(ui.pushButton_9, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Hide")));
    });

    //任务全部隐藏
    connect(ui.pushButton_10, &QPushButton::clicked, this, [&](){
        for (const auto& [id, data] : mapManagerData) {
            LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Hide")));
        }

    });

    //任务虚化
    connect(ui.pushButton_13, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#De_Focusing")));
    });

    //任务全部虚化
    connect(ui.pushButton_14, &QPushButton::clicked, this, [&](){
        for (const auto& [id, data] : mapManagerData) {
            LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#De_Focusing")));
        }

    });

    //任务显示
    connect(ui.pushButton_11, &QPushButton::clicked, this, [&](){
        const int id = getrowindex();
        LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Show")));

    });

    //任务全部显示
    connect(ui.pushButton_12, &QPushButton::clicked, this, [&](){
        for (const auto& [id, data] : mapManagerData) {
            LocalServer::instance().sendMessage(id, QString::fromStdString(std::format("command#Show")));
        }
    });

    // 任务状态设置
    connect(Signals::instance(), &Signals::State, this, [&](const int id, const std::string& message) {
        const auto item = new QTableWidgetItem();
        item->setText(QString::fromStdString(message));
        item->setTextAlignment(Qt::AlignCenter);
        ui.tableWidget->setItem(id, 1, item);
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
    connect(Signals::instance(), &Signals::setPersion, this, [&](const int id, const HWND& hwnd) {
        try {
            spdlog::info("设置角色信息");

            HBITMAP hbitmap = WindowManager::CaptureAnImage(hwnd);
            const cv::Mat image = ImageProcessor::HBITMAPToMat(hbitmap);
            DeleteObject(hbitmap);
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
        // std::wstring wintitle;
        // HBITMAP hBitmap = WindowManager::CaptureAnImage(WindowManager::getWindowHandle(wintitle));
        HBITMAP hBitmap = WindowManager::CaptureAnImage(mapManagerData[getrowindex()].hwnd);
        auto name = std::chrono::system_clock::now().time_since_epoch().count();
        WindowManager::SaveBitmapToFile(hBitmap, std::format(L"Testing/{}.bmp", name).c_str());
        const cv::Mat mat = ImageProcessor::HBITMAPToMat(hBitmap);
        cv::Mat gray;
        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);

        // // 应用高斯模糊
        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 1.2);

        // 应用 Canny 边缘检测
        cv::Mat edges;
        cv::Canny(blurred, edges, 50, 150);
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

void RunWindow::setMainWindow(const MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
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

    if (!hwndSet.contains(hwnd)) {
        if (!mapManagerData.contains(id) || (mapManagerData.contains(id) && !mapManagerData[id].isActive)) {
            return true;
        }

        for (int i = 0; i < 10; i++) {
            if (!mapManagerData.contains(i) || (mapManagerData.contains(id) && !mapManagerData[id].isActive)) {
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

