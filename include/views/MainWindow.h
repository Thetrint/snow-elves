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

#include "views/SettingWindow.h"

namespace Ui {
    class MainWindow;
}


class MainWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QJsonDocument createJsonDocument() const;

    std::string username{};

     // ~MainWindow();
private slots:
    void writeWinConfig(const int id) const;

    void readUserSettings(const QString& filename) const;

    //     void switchToPage(int index) const;

private:

    Ui::MainWindow ui{};

    NativeEventFilter *eventFilter;
    void writeUserSettings() const;

    void writeSystemSettings() const;

    void readSystemSettings() const;

    void exportConfig();

    void importConfig();

    //窗口页面
    HomeWindow *home;
    ScriptWindow *script;
    RunWindow *run;
    SettingWindow *setting;

    QButtonGroup buttonGroup{ui.menu};
    void addPageAndButton(const QString &buttonText, QWidget *page);
    int getSpacerIndex(const QSpacerItem *spacer) const;

    void addConfig(const std::string &config) const;

    void removeConfig(const std::string &config) const;

protected:
    void closeEvent(QCloseEvent *event) override {
        if (const QMessageBox::StandardButton reply =
            QMessageBox::question(this, "关闭窗口", "你确定要关闭窗口吗？", QMessageBox::Yes | QMessageBox::No); reply == QMessageBox::Yes) {
            writeSystemSettings();
            emit Signals::instance()->Close();
            event->accept();  // 接受关闭事件
        } else {
            event->ignore();  // 忽略关闭事件
        }
    }

};

#endif // MAINWINDOW_H

