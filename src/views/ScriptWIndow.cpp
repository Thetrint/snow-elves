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

    QStringList items;
    items << "课业任务" << "帮派任务" << "潜神入忆" << "华山论剑";

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
    ui.listWidget_2->setDefaultDropAction(Qt::MoveAction);
    ui.listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //添加信号
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, [&]() {
        for (QListWidgetItem *item : ui.listWidget->selectedItems()) {
            bool alreadyExists = false;
            QString currentItemText = item->text();

            // 检查在 ui.listWidget_2 中是否已经存在相同的项
            for (int i = 0; i < ui.listWidget_2->count(); ++i) {
                if (QListWidgetItem *existingItem = ui.listWidget_2->item(i); existingItem->text() == currentItemText) {
                    alreadyExists = true;
                    break;
                }
            }

            // 如果不存在，则克隆选中项，并添加到 ui.listWidget_2
            if (!alreadyExists) {
                auto *newItem = new QListWidgetItem(*item);
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
        }
    });

    // 连接 listWidget2 的双击信号到删除槽函数
    connect(ui.listWidget_2, &QListWidget::itemDoubleClicked, [&]() {
        for (QListWidgetItem *item : ui.listWidget_2->selectedItems()) {
            // 克隆选中项，然后添加到另一个 QListWidget
            delete ui.listWidget_2->takeItem(ui.listWidget_2->row(item));
        }

    });



}


