//
// Created by y1726 on 2024/6/18.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "main.h"
#include "Ui_MainWindow.h"
#include "views/MainWindow.h"
#include "views/HomeWindow.h"
#include "views/ScriptWindow.h"
#include "views/RunWindow.h"
#include "utils/Utilities.h"
#include <utils/signals.h>

namespace Ui {
    class MainWindow;
}



class MainWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


    std::string username{};

     // ~MainWindow();
private slots:
    void writeWinConfig(const int id) const;

    void readUserSettings() const;

    //     void switchToPage(int index) const;

private:

    Ui::MainWindow ui{};

    NativeEventFilter *eventFilter;
    void writeUserSettings() const;

    void exportConfig();

    void importConfig();

    //窗口页面
    HomeWindow *home;
    ScriptWindow *script;
    RunWindow *run;

    QButtonGroup buttonGroup{ui.menu};
    void addPageAndButton(const QString &buttonText, QWidget *page);
    int getSpacerIndex(const QSpacerItem *spacer) const;

    QJsonDocument createJsonDocument() const;
};

#endif // MAINWINDOW_H

