
#include "main.h"
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
    WindowManager::GetFactor();
    Logger::init(); // 初始化日志

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("windows11"));

    SingleInstanceGuard guard("UniqueAppKey");
    if (guard.isAnotherInstanceRunning()) {
        QMessageBox::warning(nullptr, "Warning", "Another instance is already running.");
        return 1; // 退出程序
    }

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

    Renew.checkupdae();

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



    // QResource::unregisterResource("RESOURCE.rcc");
    return QApplication::exec();
}

