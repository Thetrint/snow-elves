//
// Created by y1726 on 2024/6/18.
//

#include "main.h"
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

    setMinimumSize(1100, 650);



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

    //保存配置文件
    connect(script->ui.pushButton_2, &QPushButton::clicked, this, [&](){
        const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        const QStringList filters = {"*.json"};
        const QDir dir(configPath);
        QStringList files = dir.entryList(filters, QDir::Files);

        // 去掉文件名的后缀
        QStringList baseNames;
        for (const QString& file : files) {
            baseNames << QFileInfo(file).completeBaseName();
        }

        // 创建一个选择文件的对话框
        QDialog dialog;
        dialog.setWindowTitle(QObject::tr("选择或输入配置文件名"));

        // 设置对话框的样式
        dialog.setStyleSheet(R"(
            QDialog {
                background-color: #f0f0f0;
                border: 1px solid #d3d3d3;
                border-radius: 8px;
            }
            QLineEdit {
                border: 1px solid #c3c3c3;
                border-radius: 4px;
                padding: 5px;
            }
            QListWidget {
                border: 1px solid #c3c3c3;
                border-radius: 4px;
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 4px;
                cursor: pointer;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
        )");

        QVBoxLayout dialogLayout(&dialog);

        auto* titleLabel = new QLabel(QObject::tr("请选择或输入配置文件名:"));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
        dialogLayout.addWidget(titleLabel);

        auto* inputField = new QLineEdit;
        inputField->setPlaceholderText(QObject::tr("输入文件名"));
        dialogLayout.addWidget(inputField);

        auto* fileList = new QListWidget;
        fileList->addItems(baseNames);
        dialogLayout.addWidget(fileList);

        auto* selectButton = new QPushButton(QObject::tr("选择"));
        dialogLayout.addWidget(selectButton);

        dialog.setLayout(&dialogLayout);
        dialog.setFixedSize(350, 300);

        QObject::connect(selectButton, &QPushButton::clicked, this, [inputField, fileList, configPath, &dialog, this]() {
            QString selectedFileName;
            if (inputField->text().isEmpty()) {
                selectedFileName = fileList->currentItem() ? fileList->currentItem()->text() : "";
            } else {
                selectedFileName = inputField->text().trimmed();
            }

            if (selectedFileName.isEmpty()) {
                spdlog::error("未选择或输入文件名");
                return;
            }

            QString filePath = configPath + "/" + selectedFileName;
            if (!filePath.endsWith(".json")) {
                filePath += ".json";
            }

            const QJsonDocument settingsDoc = createJsonDocument();
            if (QFile file(filePath); file.open(QIODevice::WriteOnly)) {
                file.write(settingsDoc.toJson());
                file.close();
                spdlog::info("配置文件已保存: {}", filePath.toStdString());
            } else {
                spdlog::error("无法打开文件进行写入: {}", filePath.toStdString());
            }

            dialog.accept();
        });

        dialog.exec();
    });

    // 删除配置文件
    connect(script->ui.pushButton_3, &QPushButton::clicked, this, [&](){
        const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        const QStringList filters = {"*.json"};
        const QDir dir(configPath);
        QStringList files = dir.entryList(filters, QDir::Files);

        // 去掉文件名的后缀
        QStringList baseNames;
        for (const QString& file : files) {
            baseNames << QFileInfo(file).completeBaseName();
        }

        // 创建一个选择文件的对话框
        QDialog dialog;
        dialog.setWindowTitle(QObject::tr("输入或选择要删除的配置文件"));

        // 设置对话框的样式
        dialog.setStyleSheet(R"(
            QDialog {
                background-color: #f0f0f0;
                border: 1px solid #d3d3d3;
                border-radius: 8px;
            }
            QComboBox {
                border: 1px solid #c3c3c3;
                border-radius: 4px;
                padding: 5px;
            }
            QPushButton {
                background-color: #f44336;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 4px;
                cursor: pointer;
            }
            QPushButton:hover {
                background-color: #e53935;
            }
        )");

        QVBoxLayout dialogLayout(&dialog);

        auto* fileList = new QListWidget;
        fileList->addItems(baseNames);
        dialogLayout.addWidget(fileList);

        auto* deleteButton = new QPushButton(QObject::tr("删除"));
        dialogLayout.addWidget(deleteButton);

        dialog.setLayout(&dialogLayout);
        dialog.setFixedSize(350, 200);

        QObject::connect(deleteButton, &QPushButton::clicked, this, [fileList, configPath, &dialog, this]() {
            const QString selectedFileName = fileList->currentItem() ? fileList->currentItem()->text() : " ";

            if (selectedFileName.isEmpty()) {
                spdlog::error("未选择文件名");
                return;
            }

            const QString filePath = configPath + "/" + selectedFileName + ".json";

            // 提示用户确认删除

            if (const QMessageBox::StandardButton reply = QMessageBox::question(&dialog, QObject::tr("确认删除"),
                QObject::tr("确定要删除文件: %1 吗?").arg(selectedFileName),
                QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::Yes) {
                if (QFile::remove(filePath)) {
                    spdlog::info("配置文件已删除: {}", filePath.toStdString());
                    dialog.accept();
                } else {
                    spdlog::error("无法删除文件: {}", filePath.toStdString());
                }
            }
        });

        dialog.exec();
    });

    // 导出配置文件
    connect(script->ui.pushButton_4, &QPushButton::clicked, this, [&](){
        // 获取应用配置路径下的 JSON 文件列表
        const QStringList filters = {"*.json"};
        const QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
        QStringList files = dir.entryList(filters, QDir::Files);

        // 去掉文件名的后缀
        QStringList baseNames;
        for (const QString& file : files) {
            baseNames << QFileInfo(file).completeBaseName();
        }

        // 创建一个选择文件的对话框
        auto* fileList = new QListWidget;
        fileList->addItems(baseNames);
        fileList->setSelectionMode(QAbstractItemView::MultiSelection); // 支持多选
        QDialog dialog(this);
        dialog.setWindowTitle(tr("选择要复制的配置文件"));
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setFixedSize(300, 200);
        dialog.setStyleSheet(R"(
            QDialog {
                background-color: #f0f0f0;
                border: 1px solid #d3d3d3;
                border-radius: 8px;
            }
            QListWidget {
                border: 1px solid #c3c3c3;
                border-radius: 4px;
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 4px;
                cursor: pointer;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
        )");

        auto* layout = new QVBoxLayout;
        layout->addWidget(fileList);
        dialog.setLayout(layout);

        auto* selectButton = new QPushButton(tr("选择"));
        layout->addWidget(selectButton);

        connect(selectButton, &QPushButton::clicked, &dialog, [&]() {
            if (fileList->selectedItems().isEmpty()) {
                return;
            }
            dialog.accept();
        });

        if (dialog.exec() == QDialog::Accepted) {
            // 选择保存目标目录
            const QString destinationDir = QFileDialog::getExistingDirectory(this, tr("选择保存位置"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
            if (destinationDir.isEmpty()) {
                return; // 用户取消选择
            }

            // 遍历选中的文件并复制
            for (QListWidgetItem* item : fileList->selectedItems()) {
                QString selectedFileName = item->text();
                QString sourcePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" + selectedFileName + ".json";
                QString destinationPath = destinationDir + "/" + selectedFileName + ".json";

                // 检查目标文件是否已存在
                if (QFile::exists(destinationPath)) {
                    if (QMessageBox::StandardButton reply = QMessageBox::question(&dialog, tr("文件已存在"),
                        tr("文件 %1 已存在，是否覆盖？").arg(selectedFileName),
                        QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::No) {
                        continue; // 用户选择不覆盖，跳过
                    }
                }

                // 复制文件
                if (QFile::copy(sourcePath, destinationPath)) {
                    spdlog::info("配置文件已导出到：{}", destinationPath.toStdString());
                } else {
                    spdlog::error("文件导出失败：{}", destinationPath.toStdString());
                }
            }
        }
    });

    // 导入配置文件
    connect(script->ui.pushButton, &QPushButton::clicked, this, [&](){
        // 打开文件选择对话框选择多个源文件
        QStringList sourcePaths = QFileDialog::getOpenFileNames(this, tr("选择要导入的配置文件"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("JSON 文件 (*.json)"));
        if (sourcePaths.isEmpty()) {
            return; // 用户取消选择
        }

        // 指定目标路径（之前导出的路径）
        const QString targetDirectory = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

        // 复制每个文件
        for (const QString& sourcePath : sourcePaths) {
            QString destinationPath = targetDirectory + "/" + QFileInfo(sourcePath).fileName();

            // 检查目标文件是否已存在
            if (QFile::exists(destinationPath)) {
                if (QMessageBox::StandardButton reply = QMessageBox::question(this, tr("文件已存在"),
                                                                              tr("文件 %1 已存在，是否覆盖？").arg(
                                                                                  QFileInfo(sourcePath).fileName()),
                                                                              QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::No) {
                    continue; // 用户选择不覆盖，跳过
                }
            }

            // 复制文件
            if (QFile::copy(sourcePath, destinationPath)) {
                spdlog::info("配置文件已导入到：{}", destinationPath.toStdString());
            } else {
                spdlog::error("导入文件失败：{}", destinationPath.toStdString());
            }
        }
    });
    // 创建并安装事件过滤器
    qApp->installNativeEventFilter(eventFilter);


    readSystemSettings();
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
    root["角色1"] = false;
    root["角色2"] = false;
    root["角色3"] = false;
    root["角色4"] = false;
    root["角色5"] = false;
    root["华山论剑次数"] = script->ui.spinBox->value();
    root["华山论剑秒退"] = script->ui.checkBox->checkState();
    root["江湖英雄榜次数"] = script->ui.spinBox_2->value();
    root["江湖英雄榜秒退"] = script->ui.checkBox_2->checkState();
    root["副本模式"] = script->ui.comboBox_3->currentText();
    root["副本人数"] = script->ui.comboBox_2->currentIndex() + 1;
    root["副本喊话内容"] = script->ui.lineEdit->text();
    root["银票礼盒兑换"] = script->ui.checkBox_3->checkState();


    return QJsonDocument(root);
}


void MainWindow::writeWinConfig(const int id) const {
    const QJsonDocument settingsDoc = createJsonDocument();

    // 打开文件以写入 JSON 字符串
    if (QFile file(QDir::tempPath() + "/ElvesConfig_" + QString::number(id) + ".json");
        file.open(QIODevice::WriteOnly)) {
        file.write(settingsDoc.toJson());
        file.close();
        spdlog::info("配置文件已保存: {}", id);
    } else {
        spdlog::error("无法打开文件进行写入: {}", id);
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

    }
}



void MainWindow::writeSystemSettings() {

    QJsonObject root;
    root["当前配置"] = "";
    const auto settingsDoc =  QJsonDocument(root);


    const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/System";

    // 检查并创建路径中缺少的目录
    if (const QDir dir; !dir.exists(configPath)) {
        if (!dir.mkpath(configPath)) {
            spdlog::error("无法创建配置目录");
            return;
        }
    }

    const QString filePath = configPath + "/SystemConfig.json";

    if (QFile file(filePath); file.open(QIODevice::WriteOnly)) {
        file.write(settingsDoc.toJson());
        file.close();
    } else {
        spdlog::error("无法打开系统配置文件进行写入");
    }
}

void MainWindow::readSystemSettings() const {
    const QString Path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/System";
    const QString filePath = Path + "/SystemConfig.json";

    QFile file(filePath);
    if (!file.exists()) {
        spdlog::error("系统配置文件不存在: {}", filePath.toStdString());
        return ; // 返回空的 JSON 对象
    }

    if (!file.open(QIODevice::ReadOnly)) {
        spdlog::error("无法打开系统配置文件进行读取: {}", filePath.toStdString());
        return ; // 返回空的 JSON 对象
    }
    const QByteArray fileData = file.readAll();
    file.close();

    const QJsonDocument settingsDoc = QJsonDocument::fromJson(fileData);
    if (settingsDoc.isNull() || !settingsDoc.isObject()) {
        spdlog::error("系统配置文件解析失败");
        return ; // 返回空的 JSON 对象
    }
    QJsonObject root = settingsDoc.object();

    script->ui.comboBox->addItem(root["当前配置"].toString());

    const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    const QStringList filters = {"*.json"};
    const QDir dir(configPath);

    for (QStringList files = dir.entryList(filters, QDir::Files); const QString& s : files) {
        script->ui.comboBox->addItem(QFileInfo(s).completeBaseName());
    }


}
