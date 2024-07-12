//
// Created by y1726 on 24-7-6.
//
#include "views/LoginWindow.h"

#include <iostream>
#include <QFile>
#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent):
    QWidget(parent)

{
    ui.setupUi(this);  // 初始化界面布局和元素

    connect(ui.pushButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}


void LoginWindow::onLoginClicked() {
    // 在这里进行用户验证

    if (bool isAuthenticated = true) {
        emit loginSuccess();
        close(); // 关闭登录窗口并返回 Accepted
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials.");
    }

};