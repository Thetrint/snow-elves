//
// Created by y1726 on 2024/6/18.
//

#include "main.h"
#include "views/MainWindow.h"



#include "views/HomeWindow.h"
#include "views/RunWindow.h"
#include "views/ScriptWindow.h"
#include "views/SettingWindow.h"
#include "utils/signals.h"


MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    eventFilter(new NativeEventFilter(this))

{
    ui.setupUi(this);  // 初始化界面布局和元素

    setMinimumSize(1100, 650);

    setWindowTitle(QString::fromStdString(std::string("时雪") + PROJECT_VERSION));

    home = new HomeWindow(this);
    addPageAndButton("主页", home);

    script = new ScriptWindow(this);
    addPageAndButton("脚本", script);


    run = new RunWindow(this);
    run->setMainWindow(this);
    addPageAndButton("运行", run);

    setting = new SettingWindow(this);
    addPageAndButton("设置", setting);

    // connect(RunWindow)
    buttonGroup.setExclusive(true);

    connect(setting->ui.lineEdit, &QLineEdit::textChanged, this, [&](const QString& textKey) {
        // 初始化修饰符和按键
        UINT modifiers = 0;
        UINT key = 0;

        // 将输入字符串按 '+' 拆分
        QStringList keyParts = textKey.split('+');

        // 遍历每一部分
        for (const QString& part : keyParts) {
            if (part == "Ctrl") {
                modifiers |= MOD_CONTROL;  // Ctrl 键
            } else if (part == "Shift") {
                modifiers |= MOD_SHIFT;    // Shift 键
            } else if (part == "Alt") {
                modifiers |= MOD_ALT;      // Alt 键
            } else if (part.length() == 1 && part[0].isLetterOrNumber()) {
                // 如果是字母或数字，取其大写形式的 ASCII 值
                key = part[0].toUpper().unicode();
            }
        }
        UnregisterHotKey(reinterpret_cast<HWND>(this->winId()), 1);
        // 注册热键
        if (RegisterHotKey(reinterpret_cast<HWND>(this->winId()), 1, modifiers, key)) {
            std::cout << "热键注册成功: " << textKey.toStdString() << std::endl;
        } else {
            std::cout << "热键注册失败: " << textKey.toStdString() << std::endl;
        }
    });

    connect(Signals::instance(), &Signals::writejson, this, &MainWindow::writeWinConfig);


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
                addConfig(selectedFileName.toStdString());
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
        fileList->setSelectionMode(QAbstractItemView::MultiSelection);
        dialogLayout.addWidget(fileList);

        auto* deleteButton = new QPushButton(QObject::tr("删除"));
        dialogLayout.addWidget(deleteButton);

        dialog.setLayout(&dialogLayout);
        dialog.setFixedSize(350, 200);

        connect(deleteButton, &QPushButton::clicked, this, [fileList, configPath, &dialog, this]() {
            QList<QListWidgetItem*> selectedItems = fileList->selectedItems();
            if (selectedItems.isEmpty()) {
                spdlog::error("未选择文件名");
                return;
            }

            QStringList selectedFileNames;
            for (auto* item : selectedItems) {
                selectedFileNames << item->text();
            }

            // 提示用户确认删除
            const QString confirmMessage = QObject::tr("确定要删除以下文件吗?\n%1").arg(selectedFileNames.join("\n"));
            if (const QMessageBox::StandardButton reply = QMessageBox::question(&dialog, QObject::tr("确认删除"),
                confirmMessage, QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::Yes) {

                bool allDeleted = true;
                for (const QString& selectedFileName : selectedFileNames) {
                    const QString filePath = configPath + "/" + selectedFileName + ".json";
                    if (QFile::remove(filePath)) {
                        removeConfig(selectedFileName.toStdString());
                        spdlog::info("配置文件已删除: {}", filePath.toStdString());
                    } else {
                        spdlog::error("无法删除文件: {}", filePath.toStdString());
                        allDeleted = false;
                    }
                }

                if (allDeleted) {
                    dialog.accept();
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
                if (const QMessageBox::StandardButton reply = QMessageBox::question(this, tr("文件已存在"),
                                                                                    tr("文件 %1 已存在，是否覆盖？").arg(
                                                                                        QFileInfo(sourcePath).fileName()),
                                                                                    QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::No) {
                    continue; // 用户选择不覆盖，跳过
                }
            }

            // 复制文件
            if (QFile::copy(sourcePath, destinationPath)) {
                addConfig(QFileInfo(sourcePath).fileName().toStdString());
                spdlog::info("配置文件已导入到：{}", destinationPath.toStdString());
            } else {
                spdlog::error("导入文件失败：{}", destinationPath.toStdString());
            }
        }
    });

    // 读取配置文件
    connect(script->ui.comboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        readUserSettings(text);
    });
    // 创建并安装事件过滤器
    qApp->installNativeEventFilter(eventFilter);


    script->ui.comboBox->addItem("默认配置");
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

// 下拉框添加配置文件

void MainWindow::addConfig(const std::string& config) const {
    const QString qConfig = QString::fromStdString(config);

    // 检查下拉框是否包含该配置
    if (const auto& comboBox = script->ui.comboBox; comboBox->findText(qConfig) == -1) {
        // 如果不包含，则添加进去
        comboBox->addItem(qConfig);
    }
}

void MainWindow::removeConfig(const std::string& config) const {
    const QString qConfig = QString::fromStdString(config);

    // 检查下拉框是否包含该配置
    if (const auto& comboBox = script->ui.comboBox; comboBox->findText(qConfig) != -1) {
        // 如果包含，则删除
        comboBox->removeItem(comboBox->findText(qConfig));
    }
}

QJsonDocument MainWindow::createJsonDocument() const {
    QJsonArray jsonArray;
    for (int i = 0; i < script->ui.listWidget_2->count(); ++i) {
        const QListWidgetItem* item = script->ui.listWidget_2->item(i);
        jsonArray.append(item->text());
    }

    QJsonArray contentArray;
    // 定义文件路径
    const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/System";
    const QString filePath = configPath + "/Chivalry.txt";

    // 尝试打开文件以读取内容
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        contentArray.append("日出日落都浪漫,有风无风都自由。");

    }
    QTextStream in(&file);
    const QString fileContent = in.readAll();
    file.close();

    // 根据换行符分割文件内容

    for (auto contentList = fileContent.split('\n'); const QString &line : contentList) {
        contentArray.append(line);
    }

    QJsonObject root;
    root["执行任务"] = jsonArray;
    root["角色1"] = false;
    root["角色2"] = false;
    root["角色3"] = false;
    root["角色4"] = false;
    root["角色5"] = false;
    root["华山论剑次数"] = script->ui.spinBox->value();
    root["华山论剑秒退"] = script->ui.checkBox->isChecked();
    root["江湖英雄榜次数"] = script->ui.spinBox_2->value();
    root["江湖英雄榜秒退"] = script->ui.checkBox_2->isChecked();
    root["副本模式"] = script->ui.comboBox_3->currentText();
    root["副本人数"] = script->ui.comboBox_2->currentIndex() + 1;
    root["副本喊话内容"] = script->ui.lineEdit->text();
    root["银票礼盒兑换"] = script->ui.checkBox_3->isChecked();
    root["帮派铜钱捐献"] = script->ui.checkBox_4->isChecked();
    root["帮派银两捐献"] = script->ui.checkBox_5->isChecked();
    root["宗门试炼1"] = script->ui.comboBox_4->currentText();
    root["宗门试炼2"] = script->ui.comboBox_5->currentText();
    root["宗门试炼3"] = script->ui.comboBox_6->currentText();
    root["宗门试炼队伍1"] = script->ui.comboBox_7->currentIndex();
    root["宗门试炼队伍2"] = script->ui.comboBox_8->currentIndex();
    root["宗门试炼队伍3"] = script->ui.comboBox_9->currentIndex();
    root["宗门生产"] = script->ui.checkBox_6->isChecked();
    root["宗门生产一键催命"] = script->ui.checkBox_7->isChecked();
    root["宗门生产心情等级"] = script->ui.comboBox_10->currentIndex();

    root["侠缘喊话内容"] = contentArray;
    root["侠缘喊话昵称编号"] = script->ui.lineEdit_2->text();
    root["侠缘喊话次数"] = script->ui.spinBox_3->value();

    root["剑冢次数"] = script->ui.spinBox_5->value();
    root["剑冢随机跳伞"] = script->ui.checkBox_8->isChecked();


    root["技能1"] = script->ui.lineEdit_3->text();
    root["技能2"] = script->ui.lineEdit_4->text();
    root["技能3"] = script->ui.lineEdit_5->text();
    root["技能4"] = script->ui.lineEdit_6->text();
    root["技能5"] = script->ui.lineEdit_7->text();
    root["技能6"] = script->ui.lineEdit_8->text();
    root["技能7"] = script->ui.lineEdit_9->text();
    root["技能8"] = script->ui.lineEdit_10->text();
    root["普攻"] = script->ui.lineEdit_11->text();
    root["绝学"] = script->ui.lineEdit_12->text();
    root["关山"] = script->ui.lineEdit_13->text();
    root["闪避"] = script->ui.lineEdit_14->text();

    root["自定义连招"] = script->ui.textEdit->toPlainText();

    root["自创1"] = script->ui.lineEdit_15->text();
    root["自创2"] = script->ui.lineEdit_16->text();
    root["自创3"] = script->ui.lineEdit_17->text();
    root["自创4"] = script->ui.lineEdit_18->text();
    root["前"] = script->ui.lineEdit_19->text();
    root["后"] = script->ui.lineEdit_20->text();
    root["左"] = script->ui.lineEdit_21->text();
    root["右"] = script->ui.lineEdit_22->text();
    root["背包"] = script->ui.lineEdit_23->text();
    root["帮派"] = script->ui.lineEdit_24->text();
    root["队伍"] = script->ui.lineEdit_25->text();
    root["地图"] = script->ui.lineEdit_26->text();

    root["华山论剑3v3次数"] = script->ui.spinBox_7->value();

    root["江湖行商喊话内容"] = script->ui.lineEdit_27->text();
    root["江湖行商次数"] = script->ui.spinBox_8->value();
    root["江湖行商模式"] = script->ui.comboBox_12->currentText();
    root["江湖行商队长编号"] = script->ui.spinBox_9->value();

    root["采集换线"] = script->ui.spinBox_10->value();
    root["采集次数"] = script->ui.spinBox_11->value();
    root["采集工具购买"] = script->ui.checkBox_10->isChecked();
    root["采集自动吃鸡蛋"] = script->ui.checkBox_9->isChecked();
    root["采集吃鸡蛋数量"] = script->ui.spinBox_12->value();
    root["采集自定义内容"] = script->ui.textEdit_2->toPlainText();
    root["采集物品"] = script->ui.comboBox_14->currentText();
    root["采集模式"] = script->ui.comboBox_13->currentText();

    root["日常悬赏队长编号"] = script->ui.spinBox_13->value();

    root["商品1"] = script->ui.checkBox_12->isChecked();
    root["商品2"] = script->ui.checkBox_13->isChecked();
    root["商品3"] = script->ui.checkBox_14->isChecked();
    root["商品4"] = script->ui.checkBox_15->isChecked();
    root["商品5"] = script->ui.checkBox_16->isChecked();
    root["商品6"] = script->ui.checkBox_17->isChecked();
    root["商品7"] = script->ui.checkBox_18->isChecked();
    root["商品8"] = script->ui.checkBox_19->isChecked();
    root["快速购买"] = script->ui.checkBox_20->isChecked();
    root["优先级1"] = script->ui.comboBox_15->currentIndex() + 1;
    root["优先级2"] = script->ui.comboBox_16->currentIndex() + 1;
    root["优先级3"] = script->ui.comboBox_17->currentIndex() + 1;
    root["优先级4"] = script->ui.comboBox_18->currentIndex() + 1;
    root["优先级5"] = script->ui.comboBox_19->currentIndex() + 1;
    root["优先级6"] = script->ui.comboBox_20->currentIndex() + 1;
    root["优先级7"] = script->ui.comboBox_21->currentIndex() + 1;
    root["优先级8"] = script->ui.comboBox_22->currentIndex() + 1;

    root["摆摊延迟1"] = script->ui.spinBox_14->value();
    root["摆摊延迟2"] = script->ui.spinBox_15->value();
    root["摆摊延迟3"] = script->ui.spinBox_16->value();

    root["商会鸡蛋"] = script->ui.checkBox_11->isChecked();
    root["榫头卯眼"] = script->ui.checkBox_21->isChecked();
    root["锦芳绣残片"] = script->ui.checkBox_22->isChecked();
    root["神厨食材购买"] = script->ui.checkBox_23->isChecked();
    root["神厨食材购买清单"] = script->ui.lineEdit_28->text();
    root["商票上缴"] = script->ui.checkBox_24->isChecked();
    root["生活技能-艾草"] = script->ui.checkBox_25->isChecked();
    root["生活技能-莲子"] = script->ui.checkBox_26->isChecked();


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



void MainWindow::readUserSettings(const QString& filename) const {

    if (filename.toStdString() == "默认配置") {
        script->ui.listWidget_2->clear();
        script->ui.spinBox->setValue(1);
        script->ui.checkBox->setChecked(false);
        script->ui.spinBox_2->setValue(1);
        script->ui.checkBox_2->setChecked(false);
        script->ui.comboBox_3->setCurrentText("带队模式");
        script->ui.comboBox_2->setCurrentIndex(0);
        script->ui.lineEdit->setText("");
        script->ui.checkBox_3->setChecked(false);
        script->ui.checkBox_4->setChecked(false);
        script->ui.checkBox_5->setChecked(false);

        script->ui.comboBox_4->setCurrentText("~");
        script->ui.comboBox_5->setCurrentText("~");
        script->ui.comboBox_6->setCurrentText("~");

        script->ui.comboBox_7->setCurrentIndex(0);
        script->ui.comboBox_8->setCurrentIndex(0);
        script->ui.comboBox_9->setCurrentIndex(0);

        script->ui.checkBox_6->setChecked(false);
        script->ui.checkBox_7->setChecked(false);
        script->ui.comboBox_10->setCurrentIndex(0);

        script->ui.lineEdit_2->setText("");
        script->ui.spinBox_3->setValue(100);


        script->ui.spinBox_5->setValue(1);
        script->ui.checkBox_8->setChecked(false);

        script->ui.lineEdit_3->setText("1");
        script->ui.lineEdit_4->setText("2");
        script->ui.lineEdit_5->setText("3");
        script->ui.lineEdit_6->setText("4");
        script->ui.lineEdit_7->setText("5");
        script->ui.lineEdit_8->setText("6");
        script->ui.lineEdit_9->setText("7");
        script->ui.lineEdit_10->setText("8");
        script->ui.lineEdit_11->setText("9");
        script->ui.lineEdit_12->setText("R");
        script->ui.lineEdit_13->setText("space");
        script->ui.lineEdit_14->setText("shift");

        script->ui.textEdit->setText("点击#技能1#2000 点击#普攻#2000 点击#技能2#2000 点击#技能3#2000 点击#技能4#2000 点击#技能5#2000 点击#技能6#2000 点击#技能7#2000 点击#技能8#2000 点击#绝学#2000");

        script->ui.lineEdit_15->setText("");
        script->ui.lineEdit_16->setText("");
        script->ui.lineEdit_17->setText("");
        script->ui.lineEdit_18->setText("");
        script->ui.lineEdit_19->setText("W");
        script->ui.lineEdit_20->setText("S");
        script->ui.lineEdit_21->setText("A");
        script->ui.lineEdit_22->setText("D");
        script->ui.lineEdit_23->setText("B");
        script->ui.lineEdit_24->setText("O");
        script->ui.lineEdit_25->setText("T");
        script->ui.lineEdit_26->setText("M");

        script->ui.spinBox_7->setValue(1);

        script->ui.lineEdit_27->setText("江湖行商/聚义平冤来人!!!");
        script->ui.spinBox_8->setValue(5);
        script->ui.comboBox_12->setCurrentText("带队模式");
        script->ui.spinBox_9->setValue(1);

        script->ui.spinBox_10->setValue(1);
        script->ui.spinBox_11->setValue(9999);
        script->ui.checkBox_10->setChecked(false);
        script->ui.checkBox_9->setChecked(false);
        script->ui.spinBox_12->setValue(1);
        script->ui.textEdit_2->setText("");
        script->ui.comboBox_14->setCurrentText("野花_江南");
        script->ui.comboBox_13->setCurrentText("默认模式");

        script->ui.checkBox_12->setChecked(false);
        script->ui.checkBox_13->setChecked(false);
        script->ui.checkBox_14->setChecked(false);
        script->ui.checkBox_15->setChecked(false);
        script->ui.checkBox_16->setChecked(false);
        script->ui.checkBox_17->setChecked(false);
        script->ui.checkBox_18->setChecked(false);
        script->ui.checkBox_19->setChecked(false);
        script->ui.checkBox_20->setChecked(false);
        script->ui.comboBox_15->setCurrentIndex(0);
        script->ui.comboBox_16->setCurrentIndex(0);
        script->ui.comboBox_17->setCurrentIndex(0);
        script->ui.comboBox_18->setCurrentIndex(0);
        script->ui.comboBox_19->setCurrentIndex(0);
        script->ui.comboBox_20->setCurrentIndex(0);
        script->ui.comboBox_21->setCurrentIndex(0);
        script->ui.comboBox_22->setCurrentIndex(0);

        script->ui.spinBox_14->setValue(50);
        script->ui.spinBox_15->setValue(10);
        script->ui.spinBox_16->setValue(10);

        script->ui.checkBox_11->setChecked(false);
        script->ui.checkBox_21->setChecked(false);
        script->ui.checkBox_22->setChecked(false);
        script->ui.checkBox_23->setChecked(false);
        script->ui.lineEdit_28->setText("猪肉# 土鸡蛋# 精致面粉#");

        script->ui.checkBox_24->setChecked(false);
        script->ui.checkBox_25->setChecked(false);
        script->ui.checkBox_26->setChecked(false);

        return;
    }
    QJsonDocument settingsDoc;
    if (QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" + filename + ".json"); file.open(QIODevice::ReadOnly)) {
        const QByteArray data = file.readAll();
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
            QString text = value.toString();
            auto items = script->ui.listWidget->findItems(text, Qt::MatchExactly);

            auto *newItem = new QListWidgetItem(*items.first());
            script->ui.listWidget_2->addItem(newItem);
        }

        script->ui.spinBox->setValue(root["华山论剑次数"].toInt());
        script->ui.checkBox->setChecked(root["华山论剑秒退"].toBool());
        script->ui.spinBox_2->setValue(root["江湖英雄榜次数"].toInt());
        script->ui.checkBox_2->setChecked(root["江湖英雄榜秒退"].toBool());
        script->ui.comboBox_3->setCurrentText(root["副本模式"].toString());
        script->ui.comboBox_2->setCurrentIndex(root["副本人数"].toInt() - 1);
        script->ui.lineEdit->setText(root["副本喊话内容"].toString());
        script->ui.checkBox_3->setChecked(root["银票礼盒兑换"].toBool());
        script->ui.checkBox_4->setChecked(root["帮派铜钱捐献"].toBool());
        script->ui.checkBox_5->setChecked(root["帮派银两捐献"].toBool());

        script->ui.comboBox_4->setCurrentText(root["宗门试炼1"].toString());
        script->ui.comboBox_5->setCurrentText(root["宗门试炼2"].toString());
        script->ui.comboBox_6->setCurrentText(root["宗门试炼3"].toString());

        script->ui.comboBox_7->setCurrentIndex(root["宗门试炼队伍1"].toInt());
        script->ui.comboBox_8->setCurrentIndex(root["宗门试炼队伍2"].toInt());
        script->ui.comboBox_9->setCurrentIndex(root["宗门试炼队伍3"].toInt());

        script->ui.checkBox_6->setChecked(root["宗门生产"].toBool());
        script->ui.checkBox_7->setChecked(root["宗门生产一键催命"].toBool());
        script->ui.comboBox_10->setCurrentIndex(root["宗门生产心情等级"].toInt());

        script->ui.lineEdit_2->setText(root["侠缘喊话昵称编号"].toString());
        script->ui.spinBox_3->setValue(root["侠缘喊话次数"].toInt());

        script->ui.spinBox_5->setValue(root["剑冢次数"].toInt());
        script->ui.checkBox_8->setChecked(root["剑冢随机跳伞"].toBool());


        script->ui.lineEdit_3->setText(root["技能1"].toString());
        script->ui.lineEdit_4->setText(root["技能2"].toString());
        script->ui.lineEdit_5->setText(root["技能3"].toString());
        script->ui.lineEdit_6->setText(root["技能4"].toString());
        script->ui.lineEdit_7->setText(root["技能5"].toString());
        script->ui.lineEdit_8->setText(root["技能6"].toString());
        script->ui.lineEdit_9->setText(root["技能7"].toString());
        script->ui.lineEdit_10->setText(root["技能8"].toString());
        script->ui.lineEdit_11->setText(root["普攻"].toString());
        script->ui.lineEdit_12->setText(root["绝学"].toString());
        script->ui.lineEdit_13->setText(root["关山"].toString());
        script->ui.lineEdit_14->setText(root["闪避"].toString());

        script->ui.textEdit->setText(root["自定义连招"].toString());

        script->ui.lineEdit_15->setText(root["自创1"].toString());
        script->ui.lineEdit_16->setText(root["自创2"].toString());
        script->ui.lineEdit_17->setText(root["自创3"].toString());
        script->ui.lineEdit_18->setText(root["自创4"].toString());
        script->ui.lineEdit_19->setText(root["前"].toString());
        script->ui.lineEdit_20->setText(root["后"].toString());
        script->ui.lineEdit_21->setText(root["左"].toString());
        script->ui.lineEdit_22->setText(root["右"].toString());
        script->ui.lineEdit_23->setText(root["背包"].toString());
        script->ui.lineEdit_24->setText(root["帮派"].toString());
        script->ui.lineEdit_25->setText(root["队伍"].toString());
        script->ui.lineEdit_26->setText(root["地图"].toString());

        script->ui.spinBox_7->setValue(root["华山论剑3v3次数"].toInt());

        script->ui.lineEdit_27->setText(root["江湖行商喊话内容"].toString());
        script->ui.spinBox_8->setValue(root["江湖行商次数"].toInt());
        script->ui.comboBox_12->setCurrentText(root["江湖行商模式"].toString());
        script->ui.spinBox_9->setValue(root["江湖行商队长编号"].toInt());

         script->ui.spinBox_10->setValue(root["采集换线"].toInt());
         script->ui.spinBox_11->setValue(root["采集次数"].toInt());
         script->ui.checkBox_10->setChecked(root["采集工具购买"].toBool());
         script->ui.checkBox_9->setChecked(root["采集自动吃鸡蛋"].toBool());
         script->ui.spinBox_12->setValue(root["采集吃鸡蛋数量"].toBool());
         script->ui.textEdit_2->setText(root["采集自定义内容"].toString());
         script->ui.comboBox_14->setCurrentText(root["采集物品"].toString());
         script->ui.comboBox_13->setCurrentText(root["采集模式"].toString());

        script->ui.checkBox_12->setChecked(root["商品1"].toBool());
        script->ui.checkBox_13->setChecked(root["商品2"].toBool());
        script->ui.checkBox_14->setChecked(root["商品3"].toBool());
        script->ui.checkBox_15->setChecked(root["商品4"].toBool());
        script->ui.checkBox_16->setChecked(root["商品5"].toBool());
        script->ui.checkBox_17->setChecked(root["商品6"].toBool());
        script->ui.checkBox_18->setChecked(root["商品7"].toBool());
        script->ui.checkBox_19->setChecked(root["商品8"].toBool());
        script->ui.checkBox_20->setChecked(root["快速购买"].toBool());
        script->ui.comboBox_15->setCurrentIndex(root["优先级1"].toInt() - 1);
        script->ui.comboBox_16->setCurrentIndex(root["优先级2"].toInt() - 1);
        script->ui.comboBox_17->setCurrentIndex(root["优先级3"].toInt() - 1);
        script->ui.comboBox_18->setCurrentIndex(root["优先级4"].toInt() - 1);
        script->ui.comboBox_19->setCurrentIndex(root["优先级5"].toInt() - 1);
        script->ui.comboBox_20->setCurrentIndex(root["优先级6"].toInt() - 1);
        script->ui.comboBox_21->setCurrentIndex(root["优先级7"].toInt() - 1);
        script->ui.comboBox_22->setCurrentIndex(root["优先级8"].toInt() - 1);

        script->ui.spinBox_14->setValue(root["摆摊延迟1"].toInt());
        script->ui.spinBox_15->setValue(root["摆摊延迟2"].toInt());
        script->ui.spinBox_16->setValue(root["摆摊延迟3"].toInt());

        script->ui.checkBox_11->setChecked(root["商会鸡蛋"].toBool());
        script->ui.checkBox_21->setChecked(root["榫头卯眼"].toBool());
        script->ui.checkBox_22->setChecked(root["锦芳绣残片"].toBool());
        script->ui.checkBox_23->setChecked(root["神厨食材购买"].toBool());
        script->ui.lineEdit_28->setText(root["神厨食材购买清单"].toString());

        script->ui.checkBox_24->setChecked(root["商票上缴"].toBool());
        script->ui.checkBox_25->setChecked(root["生活技能-艾草"].toBool());
        script->ui.checkBox_26->setChecked(root["生活技能-莲子"].toBool());


    }
}



void MainWindow::writeSystemSettings() const {

    QJsonObject root;
    root["当前配置"] = script->ui.comboBox->currentText();
    root["全局延迟"] = script->ui.spinBox_4->value();
    root["过图倍率"] = script->ui.spinBox_6->value();
    root["缩放比例"] = script->ui.comboBox_11->currentIndex();
    root["开始快捷键"] = setting->ui.lineEdit->text();
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

    const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    const QStringList filters = {"*.json"};
    const QDir dir(configPath);

    for (QStringList files = dir.entryList(filters, QDir::Files); const QString& s : files) {
        script->ui.comboBox->addItem(QFileInfo(s).completeBaseName());
    }

    // 当前配置文件
    script->ui.comboBox->setCurrentText(root["当前配置"].toString());
    script->ui.spinBox_4->setValue(root["全局延迟"].toInt());
    script->ui.spinBox_6->setValue(root["过图倍率"].toInt());
    script->ui.comboBox_11->setCurrentIndex(root["缩放比例"].toInt());
    setting->ui.lineEdit->setText(root["开始快捷键"].toString());


}
