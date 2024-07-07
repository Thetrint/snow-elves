//
// Created by y1726 on 24-7-6.
//

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include "Ui_LoginWindow.h"

namespace Ui {
    class LoginWindow;
}

class LoginWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);


signals:
    void loginSuccess();

private slots:
    void onLoginClicked();

private:
    Ui::LoginWindow ui{};
};

#endif //LOGINWINDOW_H
