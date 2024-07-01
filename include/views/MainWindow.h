//
// Created by y1726 on 2024/6/18.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Ui_MainWindow.h"
#include "views/MainWindow.h"
#include "views/HomeWindow.h"
#include "views/ScriptWindow.h"
#include "views/RunWindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow final: public QWidget
{
    // Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

     // ~MainWindow();
private slots:
    void writewinconfig(int id) const;
//     void switchToPage(int index) const;

private:
    //窗口页面
    HomeWindow *home;
    ScriptWindow *script;
    RunWindow *run;
    // 菜单布局
    QVBoxLayout *menu_layout;
    QSpacerItem *spacer;
    void addPageAndButton(const QString &buttonText, QWidget *page);
    int getSpacerIndex(const QSpacerItem *spacer) const;



    Ui::MainWindow ui{};
};

#endif // MAINWINDOW_H

