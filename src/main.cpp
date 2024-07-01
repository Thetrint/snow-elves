#include "views/MainWindow.h"

#include "models/ImageProcess.h"
#include "models/WindowManager.h"
#include <QStyleFactory>

#include <iostream>
#include <QFile>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("windowsvita"));

    // 注册资源文件
    if (bool resourceRegistered = QResource::registerResource("RESOURCE.rcc")) {
        std::cout << "Resource file snowelves.rcc registered successfully." << std::endl;
    } else {
        std::cerr << "Failed to register resource file snowelves.qrc!" << std::endl;
        return -1; // 资源文件注册失败，退出程序
    }



    // 读取样式表文件
    if (QFile styleFile(":/styles/style.qss"); styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Opened style.qss file successfully." << std::endl;
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }else {
        std::cerr << "Failed to load style.qss file!" << std::endl;
    }

    MainWindow w;
    w.show();

    QResource::unregisterResource("resources/snowelves.qrc");
    return QApplication::exec();
}
