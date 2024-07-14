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
#include <QButtonGroup>
#include <QAbstractNativeEventFilter>
#include <QObject>
#include <utils/signals.h>

namespace Ui {
    class MainWindow;
}

class NativeEventFilter final : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT

public:
    explicit NativeEventFilter(QObject *parent = nullptr) : QObject(parent){}

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
        if (eventType == "windows_generic_MSG") {
            if (const MSG *msg = static_cast<MSG *>(message); msg->message == WM_HOTKEY) {
                std::cout << "成功了吧~~" << std::endl;
                std::cout << msg->wParam << std::endl;
                if (msg->wParam == 1) {
                    emit Signals::instance()->Start();
                }

                return true;
            }
        }
        return false;
    }

};


class MainWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


    std::string username{};

     // ~MainWindow();
private slots:
    void writewinconfig(const int id) const;
//     void switchToPage(int index) const;

private:

    Ui::MainWindow ui{};

    NativeEventFilter *eventFilter;

    //窗口页面
    HomeWindow *home;
    ScriptWindow *script;
    RunWindow *run;

    QButtonGroup buttonGroup{ui.menu};
    void addPageAndButton(const QString &buttonText, QWidget *page);
    int getSpacerIndex(const QSpacerItem *spacer) const;



};

#endif // MAINWINDOW_H

