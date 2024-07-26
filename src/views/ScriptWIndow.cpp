//
// Created by y1726 on 2024/6/29.
//
#include <iostream>
#include <QMessageBox>
#include <QTimer>
#include "views/ScriptWindow.h"


ScriptWindow::ScriptWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    ui.spinBox->setMinimum(1);
    ui.spinBox_2->setMinimum(1);

    QStringList items;
    items
    << "课业任务" << "帮派任务" << "潜神入忆" << "华山论剑" << "华山论剑3v3" << "万象刷赞"
    << "江湖英雄榜" << "日常副本" << "悬赏任务" << "茶馆说书" << "山河器" << "门客设宴"
    << "破阵设宴" << "每日兑换" << "宗门任务";

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

    ui.listWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.listWidget_2->setDragEnabled(true);
    ui.listWidget_2->setAcceptDrops(true);
    ui.listWidget_2->setDropIndicatorShown(true);
    // ui.listWidget_2->setResizeMode(QListView::Adjust);
    ui.listWidget_2->setDefaultDropAction(Qt::MoveAction);
    ui.listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);


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



}


