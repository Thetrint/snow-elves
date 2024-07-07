//
// Created by y1726 on 24-7-6.
//

#ifndef RENEWWINDOW_H
#define RENEWWINDOW_H
#include "Ui_RenewWindow.h"
#include "models/DownloadThread.h"

namespace Ui {
    class RenewWindow;
}

class RenewWindow final: public QWidget
{
    Q_OBJECT

public:
    explicit RenewWindow(QWidget *parent = nullptr);
    Ui::RenewWindow ui{};
    void checkupdae();

signals:
    void login();

private slots:
    void UpdateProgressBar(int val) const;
    void SetProgressBar(int max) const;

private:

    static bool compareVersions(const std::string& version1, const std::string& version2);


};

#endif //RENEWWINDOW_H
