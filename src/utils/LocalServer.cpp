//
// Created by y1726 on 24-8-1.
//
#include "utils/LocalServer.h"
#include <QLocalSocket>
#include <utility>


// 静态实例的定义
LocalServer& LocalServer::getInstance() {
    static LocalServer instance;
    return instance;
}

// 私有构造函数
LocalServer::LocalServer(QObject *parent) : QObject(parent) {
    connect(&server, &QLocalServer::newConnection, this, &LocalServer::handleNewConnection);
}

void LocalServer::startServer(const QString &serverName) {
    if (!server.listen(serverName)) {
        qCritical() << "Unable to start the server:" << server.errorString();
        return;
    }
    qDebug() << "Server started, listening on" << serverName;
}

void LocalServer::handleNewConnection() {
    if (QLocalSocket *clientSocket = server.nextPendingConnection()) {
        // 读取 ID
        if (clientSocket->waitForReadyRead()) {
            const QByteArray idData = clientSocket->readAll();
            bool conversionOk;
            const int id = idData.trimmed().toInt(&conversionOk);

            if (conversionOk) {
                // 存储连接
                connections[id] = clientSocket;
                qDebug() << "Connection stored with ID:" << id;

                // // 可以发送确认消息或进行其他处理
                // clientSocket->write("ID received: " + QByteArray::number(id));
                // clientSocket->flush();
            } else {
                qWarning() << "Invalid ID received:" << idData;
                clientSocket->disconnectFromServer();
            }
        }
    }
}
