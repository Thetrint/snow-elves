//
// Created by y1726 on 2024/6/29.
//
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include "views/ScriptWindow.h"
#include "QtUtils/CustomLineEdit.h"
#include "main.h"

ScriptWindow::ScriptWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    ui.spinBox->setMinimum(1);
    ui.spinBox_2->setMinimum(1);
    ui.spinBox_3->setMinimum(1);
    ui.spinBox_3->setMaximum(200);

    ui.spinBox_4->setMinimum(1000);
    ui.spinBox_4->setSingleStep(100);
    ui.spinBox_4->setMaximum(10000);

    ui.spinBox_6->setMinimum(5);
    ui.spinBox_6->setMaximum(30);


    // 输入框重构



    QStringList items;
    items
    << "全局设置" << "#####"
    << "课业任务" << "帮派任务" << "潜神入忆" << "华山论剑" << "华山论剑3v3" << "万象刷赞"
    << "江湖英雄榜" << "日常副本" << "悬赏任务" << "茶馆说书" << "山河器" << "门客设宴"
    << "破阵设宴" << "每日兑换" << "宗门任务" << "侠缘喊话" << "生死剑冢" << "宅邸打卡";

    foreach (const QString &text, items) {
        auto *item = new QListWidgetItem(text);
        item->setTextAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(ui.listWidget->height(), 30));
        ui.listWidget->addItem(item);
    }

    // 设置QListWidget的属性以启用拖放功能
    // ui.listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // ui.listWidget->setDragEnabled(true);
    // ui.listWidget->setAcceptDrops(true);
    // ui.listWidget->setDropIndicatorShown(true);
    // ui.listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui.listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui.listWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.listWidget_2->setDragEnabled(true);
    ui.listWidget_2->setAcceptDrops(true);
    ui.listWidget_2->setDropIndicatorShown(true);
    // ui.listWidget_2->setResizeMode(QListView::Adjust);
    ui.listWidget_2->setDefaultDropAction(Qt::MoveAction);
    ui.listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui.listWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui.spinBox_4, &QSpinBox::valueChanged, [&](const int value) {
        std::cout << DELAY << std::endl;
        DELAY = value;
        std::cout << DELAY << std::endl;
    });

    connect(ui.spinBox_6, &QSpinBox::valueChanged, [&](const int value) {
        std::cout << DELAY_MAGNIFICATION << std::endl;
        DELAY_MAGNIFICATION = value;
        std::cout << DELAY_MAGNIFICATION << std::endl;
    });

    // 任务导航
    connect(ui.listWidget, &QListWidget::itemClicked, [&](const QListWidgetItem *new_item) {
        const QString itemText = new_item->text();
        const std::string itemTextStd = itemText.toStdString(); // 转换为 std::string

        // 从字典中查找对应的值
        if (const auto it = TaskMap.find(itemTextStd); it != TaskMap.end()) {
            ui.stackedWidget->setCurrentIndex(it->second);

        }
    });

    connect(ui.listWidget_2, &QListWidget::itemClicked, [&](const QListWidgetItem *new_item) {
        const QString itemText = new_item->text();
        const std::string itemTextStd = itemText.toStdString(); // 转换为 std::string

        // 从字典中查找对应的值
        if (const auto it = TaskMap.find(itemTextStd); it != TaskMap.end()) {
            ui.stackedWidget->setCurrentIndex(it->second);

        }
    });

    //添加信号
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, [&](const QListWidgetItem *new_item) {
        bool alreadyExists = false;
        const QString currentItemText = new_item->text();

        // 检查在 ui.listWidget_2 中是否已经存在相同的项
        for (int i = 0; i < ui.listWidget_2->count(); ++i) {
            if (ui.listWidget_2->item(i)->text() == currentItemText) {
              alreadyExists = true;
          }
        }

        // 如果不存在，则克隆选中项，并添加到 ui.listWidget_2
        if (!alreadyExists) {
            auto *newItem = new QListWidgetItem(*new_item);
            ui.listWidget_2->addItem(newItem);
        }else {
            // 如果存在相同项，则显示提示消息框
            auto *msgBox = new QMessageBox();
            msgBox->setWindowTitle("提示");
            msgBox->setText("该项已经存在！");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setAttribute(Qt::WA_DeleteOnClose); // 自动删除
            msgBox->show();

            // 设置定时器，在一段时间后关闭消息框
            QTimer::singleShot(2000, msgBox, &QMessageBox::close);
        }

    });

    // 连接 listWidget2 的双击信号到删除槽函数
    connect(ui.listWidget_2, &QListWidget::itemDoubleClicked, [&](const QListWidgetItem *new_item) {
        delete ui.listWidget_2->takeItem(ui.listWidget_2->row(new_item));
    });

    // 侠缘喊话内容设置
    connect(ui.pushButton_5, &QPushButton::clicked, [&]() {
        // 定义文件路径
        const QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/System";
        const QString filePath = configPath + "/Chivalry.txt";

        // 尝试打开文件，如果文件不存在则创建
        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            // 如果无法打开文件，直接返回
            return;
        }

        QTextStream stream(&file);

        // 如果文件为空，则写入默认内容
        if (file.size() == 0) {
            stream << "日出日落都浪漫,有风无风都自由。\n"; // 写入默认内容
            file.flush(); // 确保数据被写入文件
        }
        // 关闭文件后重新打开以读取内容
        file.close();

        // 以只读模式重新打开文件
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "无法重新打开文件以读取:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        const QString fileContent = in.readAll();
        file.close();

        // 创建并展示对话框
        auto *dialog = new QDialog(this);
        dialog->setWindowTitle(tr("编辑文件"));

        auto *layout = new QVBoxLayout(dialog);
        const auto textEdit = new QTextEdit(dialog);
        textEdit->setText(fileContent);
        layout->addWidget(textEdit);

        auto *saveButton = new QPushButton(tr("保存"), dialog);
        connect(saveButton, &QPushButton::clicked, [=]() {
            // 获取文本编辑器中的内容
            QString text = textEdit->toPlainText().trimmed();

            // 如果文本内容为空（即没有具体内容，都是空格），则写入默认内容
            if (text.isEmpty()) {
                text = "日出日落都浪漫,有风无风都自由。\n";
            }

            // 保存文本内容到文件
            QFile file1(filePath);
            if (!file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::warning(dialog, tr("错误"), tr("无法保存文件: ") + file1.errorString());
                return;
            }

            QTextStream out(&file1);
            out << text;
            file1.close();
            dialog->accept();
        });
        layout->addWidget(saveButton);

        dialog->setLayout(layout);
        dialog->exec();
    });



}


