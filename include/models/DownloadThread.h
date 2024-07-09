//
// Created by y1726 on 24-7-6.
//

#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H
#include <winsock2.h>
#include <windows.h>
#include <cpr/cpr.h>
#include <QThread>
#include <opencv2/core/cvstd.hpp>

class DownloadThread final : public QThread {
    Q_OBJECT

public:
    explicit DownloadThread(const std::string& name, const int id, const std::string& version, QObject* parent = nullptr)
        : QThread(parent), name(name), id(id), version(version){
        // 可以在这里进行其他初始化工作
    }
    void run() override;

signals:
    void SetProgressBarSignal(int max);
    void UpdateProgressBarSignal(int value);

private:
    std::string name;
    int id;
    std::string version;
};


#endif //DOWNLOADTHREAD_H
