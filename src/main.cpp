
#include "main.h"
#include "views/MainWindow.h"

#include "models/ImageProcess.h"
#include "models/WindowManager.h"
#include "views/LoginWindow.h"
#include "views/RenewWindow.h"
#include <QStyleFactory>


#include <iostream>
#include <QFile>
#include <QResource>
#include <utils/signals.h>


int main(int argc, char *argv[])
{
    //获取屏幕缩放
    WindowManager::GetFactor();

    QApplication app(argc, argv);
    // QApplication::setStyle(QStyleFactory::create("windowsvita"));


    // 注册资源文件
    if (QResource::registerResource("RESOURCE.rcc")) {
        std::cout << "Resource file snowelves.rcc registered successfully." << std::endl;
    } else {
        std::cerr << "Failed to register resource file snowelves.qrc!" << std::endl;
        return -1; // 资源文件注册失败，退出程序
    }


    // 读取样式表文件
    if (QFile styleFile(":/styles/style.qss"); styleFile.open(QFile::ReadOnly)) {
        std::cout << "Opened style.qss file successfully." << std::endl;
        const QString styleSheet = QLatin1String(styleFile.readAll());

        app.setStyleSheet(styleSheet); // 把文件内容传参

        styleFile.close();
    } else {
        std::cerr << "Failed to load style.qss file!" << std::endl;
    }

    MainWindow w;
    RenewWindow Renew;
    LoginWindow Login;



    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(&Renew, &RenewWindow::login, [&]() {
        Login.show();
    });

    Renew.checkupdae();

    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(&Login, &LoginWindow::loginSuccess, [&]() {
        w.show();
    });

    // 连接 loginSuccess 信号到主窗口的 show 槽
    QObject::connect(Signals::instance(), &Signals::Update, [&](const std::string& name, const std::string& version) {
        std::cout << version << std::endl;
        const std::string command = "start /B Update.bat " + name + " " + version;
        std::system(command.c_str());

        w.close();
        Login.close();
        Renew.close();

    });



    // QResource::unregisterResource("RESOURCE.rcc");
    return QApplication::exec();
}

