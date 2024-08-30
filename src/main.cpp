
#include "main.h"

#include <utils/LocalServer.h>

#include "views/MainWindow.h"

#include "models/ImageProcess.h"
#include "models/WindowManager.h"
#include "views/LoginWindow.h"
#include "views/RenewWindow.h"
#include <utils/signals.h>

#include "utils/Utilities.h"

int main(int argc, char *argv[])
{
    //获取屏幕缩放
    // WindowManager::GetFactor();
    Logger::init(); // 初始化日志

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("windows11"));


    // 创建SingleInstanceGuard，确保程序单例运行
    if (const SingleInstanceGuard guard("SnowElves"); guard.isAnotherInstanceRunning()) {
        // 如果另一个实例正在运行，弹出警告并退出
        QMessageBox::warning(nullptr, "Warning", "已有程序正在运行");
        return 1;
    }

    QPalette win11Palette;
    win11Palette.setColor(QPalette::Window, QColor(255, 255, 255));          // White background
    win11Palette.setColor(QPalette::WindowText, QColor(0, 0, 0));            // Black text
    win11Palette.setColor(QPalette::Base, QColor(245, 245, 245));            // Light gray for input fields
    win11Palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));   // Slightly darker gray for alternate rows in views
    win11Palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));     // White for tooltips
    win11Palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));           // Black text in tooltips
    win11Palette.setColor(QPalette::Text, QColor(0, 0, 0));                  // Black text in input fields
    win11Palette.setColor(QPalette::Button, QColor(240, 240, 240));          // Light gray for buttons
    win11Palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));            // Black text on buttons
    win11Palette.setColor(QPalette::BrightText, QColor(255, 0, 0));          // Bright red for warnings
    win11Palette.setColor(QPalette::Link, QColor(0, 120, 215));              // Blue for links
    win11Palette.setColor(QPalette::Highlight, QColor(0, 120, 215));         // Blue highlight color
    win11Palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // White text on highlighted areas
    // Apply the palette to your application
    QApplication::setPalette(win11Palette);

    LocalServer::getInstance().startServer("SnowElvesLocalServer");


    // 注册资源文件
    if (QResource::registerResource("RESOURCE.rcc")) {
        spdlog::info("资源文件注册成功");
        std::cout << "Resource file snowelves.rcc registered successfully." << std::endl;
    } else {
        spdlog::info("资源文件注册失败");
        std::cerr << "Failed to register resource file snowelves.qrc!" << std::endl;
        return -1; // 资源文件注册失败，退出程序
    }


    // 读取样式表文件
    if (QFile styleFile(":/styles/style.qss"); styleFile.open(QFile::ReadOnly)) {
        spdlog::info("样式表加载成功");
        std::cout << "Opened style.qss file successfully." << std::endl;
        const QString styleSheet = QLatin1String(styleFile.readAll());

        app.setStyleSheet(styleSheet); // 把文件内容传参

        styleFile.close();
    } else {
        spdlog::info("样式表加载失败");
        std::cerr << "Failed to load style.qss file!" << std::endl;
    }

    MainWindow w;
    RenewWindow Renew;
    LoginWindow Login;


    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(&Renew, &RenewWindow::login, [&]() {
        spdlog::info("登陆窗口启动");
        Login.show();
    });

    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(&Login, &LoginWindow::loginSuccess, [&]() {
        spdlog::info("主窗口启动");
        w.username = "admin";
        w.show();
    });

    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(Signals::instance(), &Signals::Update, [&](const std::string& name, const std::string& version) {
        std::cout << version << std::endl;
        const std::string command = "powershell -Command \"Start-Process cmd -ArgumentList '/c Update.bat " + name + " " + version + "' -Verb RunAs\"";
        std::system(command.c_str());

        w.close();
        Login.close();
        Renew.close();

    });

    Renew.checkupdae();

    return QApplication::exec();
}

