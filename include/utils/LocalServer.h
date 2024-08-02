//
// Created by y1726 on 24-8-1.
//

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>

class LocalServer final : public QObject {
    Q_OBJECT

public:
    // 静态方法，返回类的唯一实例
    static LocalServer& getInstance();
    std::map<int, QLocalSocket*> connections; // 存储连接的映射
    void startServer(const QString &serverName);

private slots:
    void handleNewConnection();

private:
    // 构造函数私有化
    explicit LocalServer(QObject *parent = nullptr);

    // 禁止拷贝构造和赋值操作符
    LocalServer(const LocalServer &) = delete;
    LocalServer &operator=(const LocalServer &) = delete;

    QLocalServer server;

};

#endif //LOCALSERVER_H
