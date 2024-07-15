//
// Created by y1726 on 2024/6/18.
//


#include "views/MainWindow.h"
#include "views/HomeWindow.h"
#include "views/RunWindow.h"
#include "views/ScriptWindow.h"
#include "utils/signals.h"


MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    eventFilter(new NativeEventFilter(this))

{
    ui.setupUi(this);  // 初始化界面布局和元素

    setMinimumSize(961, 652);



    home = new HomeWindow(this);
    addPageAndButton("主页", home);

    script = new ScriptWindow(this);
    addPageAndButton("脚本", script);


    run = new RunWindow(this);
    addPageAndButton("运行", run);

    // connect(RunWindow)
    buttonGroup.setExclusive(true);

    connect(Signals::instance(), &Signals::writejson, this, &MainWindow::writeWinConfig);

    // 注册热键 (示例：Ctrl+Shift+1)
    if (RegisterHotKey(reinterpret_cast<HWND>(this->winId()), 1, MOD_SHIFT, 'Q')) {
        std::cout << "热键注册成功" << std::endl;
    } else {
        std::cout << "热键注册失败" << std::endl;
    }

    // 创建并安装事件过滤器
    qApp->installNativeEventFilter(eventFilter);
    // writeUserSettings();
    // exportConfig();

    // writewinconfig();
    // switchToPage(1);

}


//容器添加界面
void MainWindow::addPageAndButton(const QString &buttonText, QWidget *page) {

    const int index = getSpacerIndex(ui.verticalSpacer);

    // 创建一个QPushButton并将其添加到ui.widget的布局中
    auto *button = new QPushButton(buttonText, this);
    button->setCheckable(true);
    ui.verticalLayout->insertWidget(index, button);

    // 将按钮的点击信号连接到槽函数，切换到相应的页面
    connect(button, &QPushButton::clicked, this, [this, page]() {
        ui.stackedWidget->setCurrentWidget(page);
    });

    buttonGroup.addButton(button);
    ui.stackedWidget->addWidget(page);
}

//获取弹簧索引
int MainWindow::getSpacerIndex(const QSpacerItem *spacer) const {
    for (int i = 0; i < ui.verticalLayout->count(); ++i) {
        if (ui.verticalLayout->itemAt(i)->spacerItem() == spacer) {
            return i;
        }
    }
    return -1; // 如果没有找到弹簧，则返回-1
}

QJsonDocument MainWindow::createJsonDocument() const {
    QJsonArray jsonArray;
    for (int i = 0; i < script->ui.listWidget_2->count(); ++i) {
        const QListWidgetItem* item = script->ui.listWidget_2->item(i);
        jsonArray.append(item->text());
    }

    QJsonObject root;
    root["执行任务"] = jsonArray;
    root["keyu"] = 30;
    root["city"] = "New York";

    return QJsonDocument(root);
}


void MainWindow::writeWinConfig(const int id) const {
    const QJsonDocument settingsDoc = createJsonDocument();

    // 打开文件以写入 JSON 字符串
    if (QFile file(QDir::tempPath() + "/ElvesConfig_" + QString::number(id) + ".json"); file.
        open(QIODevice::WriteOnly)) {
        file.write(settingsDoc.toJson());
        file.close();
    }


}



void MainWindow::readUserSettings() const {
    QJsonDocument settingsDoc;
    if (QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.json"); file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
         settingsDoc = QJsonDocument::fromJson(data);
    }

    if (!settingsDoc.isNull()) {
        QJsonObject root = settingsDoc.object();

        // 读取执行任务
        QJsonArray executeTasks = root["执行任务"].toArray();
        // 更新界面或处理数据
        script->ui.listWidget_2->clear();
        for (const auto& value : executeTasks) {
            auto *item = new QListWidgetItem(value.toString());
            item->setSizeHint(QSize(script->ui.listWidget_2->height(), 30));
            item->setTextAlignment(Qt::AlignCenter);
            script->ui.listWidget_2->addItem(item);
        }

        // 读取其他配置
        int keyu = root["keyu"].toInt();
        QString city = root["city"].toString();



        qDebug() << "Keyu:" << keyu;
        qDebug() << "City:" << city;
    }
}



void MainWindow::writeUserSettings() const {

    const QJsonDocument settingsDoc = createJsonDocument();

    if (QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.json"); file.open(
        QIODevice::WriteOnly)) {
        file.write(settingsDoc.toJson());
        file.close();
        }
}


void MainWindow::exportConfig() {
    // 获取应用配置路径下的 JSON 文件列表
    QStringList filters = {"*.json"};
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    QStringList files = dir.entryList(filters, QDir::Files);

    // 创建一个选择文件的对话框
    auto* fileList = new QListWidget;
    fileList->addItems(files);
    QDialog dialog(this);
    dialog.setWindowTitle(tr("选择要复制的配置文件"));
    dialog.setLayout(new QVBoxLayout);
    dialog.layout()->addWidget(fileList);
    dialog.setWindowModality(Qt::ApplicationModal);

    auto* selectButton = new QPushButton(tr("选择"));
    dialog.layout()->addWidget(selectButton);
    dialog.setFixedSize(300, 200);

    connect(selectButton, &QPushButton::clicked, &dialog, [&]() {
        if (fileList->currentItem()) {
            dialog.accept();
        }
    });

    if (dialog.exec() == QDialog::Accepted) {
        QString sourcePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" + fileList->currentItem()->text();

        // 选择保存目标目录
        QString destinationDir = QFileDialog::getExistingDirectory(this, tr("选择保存位置"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        if (destinationDir.isEmpty()) {
            return; // 用户取消选择
        }

        // 复制文件
        QString destinationPath = destinationDir + "/" + fileList->currentItem()->text();
        if (QFile::copy(sourcePath, destinationPath)) {
            qDebug() << "配置文件已导出到：" << destinationPath;
        } else {

        }
    }
}


void MainWindow::importConfig() {
    // 打开文件选择对话框选择多个源文件
    QStringList sourcePaths = QFileDialog::getOpenFileNames(this, tr("选择要导入的配置文件"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("JSON 文件 (*.json)"));
    if (sourcePaths.isEmpty()) {
        return; // 用户取消选择
    }

    // 指定目标路径（之前导出的路径）
    QString targetDirectory = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    // 复制每个文件
    for (const QString& sourcePath : sourcePaths) {
        QString destinationPath = targetDirectory + "/" + QFileInfo(sourcePath).fileName();

        if (QFile::copy(sourcePath, destinationPath)) {
            qDebug() << "配置文件已导入到：" << destinationPath;
        } else {

        }
    }
}