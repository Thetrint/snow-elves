//
// Created by y1726 on 24-8-1.
//

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>
#include <qwindowdefs_win.h>

class LocalServer final : public QObject {
    Q_OBJECT

public:
    // 静态方法，返回类的唯一实例
    static LocalServer& instance();
    void sendMessage(const int id, const QString &message);
    void startServer(const QString &serverName);
    QLocalSocket *getConnect(int id);

    void setConnect(int id, QLocalSocket *socket);

    void removeConnect(int id);

signals:
    // 自定义信号，用于传递新的连接
    void newClientConnected(QLocalSocket *clientSocket);
    void Start(const int& id, const HWND& hwnd);

private slots:
    void NewConnection();
    void readData();

private:
    // 构造函数私有化
    explicit LocalServer(QObject *parent = nullptr);
    std::map<int, QLocalSocket*> connections; // 存储连接的映射
    // 禁止拷贝构造和赋值操作符
    LocalServer(const LocalServer &) = delete;
    LocalServer &operator=(const LocalServer &) = delete;

    QLocalServer server;

};

#endif //LOCALSERVER_H
