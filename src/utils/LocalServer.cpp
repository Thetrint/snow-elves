//
// Created by y1726 on 24-8-1.
//
#include "utils/LocalServer.h"
#include <QLocalSocket>
#include <QVariant>
#include <utility>
#include <spdlog/spdlog.h>
#include <utils/Signals.h>


// 私有构造函数
LocalServer::LocalServer(QObject *parent) : QObject(parent) {
    connect(&server, &QLocalServer::newConnection, this, &LocalServer::NewConnection);
}


// 静态实例的定义
LocalServer& LocalServer::instance() {
    static LocalServer instance;
    return instance;
}

// 启动本地服务
void LocalServer::startServer(const QString &serverName) {
    if (!server.listen(serverName)) {
        spdlog::info("Unable to start the server:");
        return;
    }
    spdlog::info("本地服务启动 监听开始");

}

QLocalSocket* LocalServer::getConnect(const int id) {
    // 检查 id 是否在 connections 映射中
    auto it = connections.find(id);
    if (it != connections.end()) {
        return it->second;  // 返回对应的 QLocalSocket*
    } else {
        return nullptr;  // 如果未找到对应的连接，返回 nullptr
    }
}

void LocalServer::setConnect(const int id, QLocalSocket* socket) {
    connections[id] = socket;  // 将 id 和 socket 存入映射
    socket->setProperty("id", QVariant(id));
    spdlog::info(std::format("连接已设置: id ={}", id));

}

void LocalServer::removeConnect(const int id) {
    if (const auto it = connections.find(id); it != connections.end()) {
        // 删除映射中的连接
        connections.erase(it);
        spdlog::info(std::format("连接已删除: id ={}", id));
    } else {
        spdlog::info(std::format("未找到对应的连接: id ={}", id));

    }
}

// 发送消息的函数实现
void LocalServer::sendMessage(const int id, const QString &message) {
    // 先获取连接

    if (QLocalSocket* socket = getConnect(id); socket && socket->state() == QLocalSocket::ConnectedState) {
        // 将 QString 转换为 QByteArray，并发送数据
        const QByteArray data = message.toUtf8();
        socket->write(data);
        socket->flush();  // 确保数据立刻发送
    } else {
        spdlog::info(std::format("连接不可用或未连接: id ={}", id));
    }
}

void LocalServer::NewConnection() {
    if (QLocalSocket *clientSocket = server.nextPendingConnection()) {
        spdlog::info("游戏窗口接入完成");

        // 连接 readyRead 信号以便读取数据
        connect(clientSocket, &QLocalSocket::readyRead, this, &LocalServer::readData);

        // 连接 disconnected 信号以便处理客户端断开连接
        connect(clientSocket, &QLocalSocket::disconnected, this, [this, clientSocket]() {
            int clientId = clientSocket->property("id").toInt();
            spdlog::info("客户端断开连接: {}", clientId);
            removeConnect(clientId); // 移除连接
        });

        // 通过信号将新连接传递出去
        emit newClientConnected(clientSocket);
    }
}

void LocalServer::readData() {
    if (auto *clientSocket = qobject_cast<QLocalSocket*>(sender())) {
        int clientId = clientSocket->property("id").toInt();
        const QByteArray data = clientSocket->readAll();
        const QString receivedData = QString::fromUtf8(data);
        spdlog::info("收到 窗口:{} 的数据: {}",clientId, receivedData.toStdString());

        QStringList parts = receivedData.split('#');

        if (parts[0] == "id_hwnd") {
            QStringList subParts = parts[1].split(':');
            emit Start(subParts[0].toInt(), reinterpret_cast<HWND>(subParts[1].toULongLong()));

        }
    }
}

