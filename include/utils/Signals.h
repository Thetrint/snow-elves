#ifndef SIGNALS_H
#define SIGNALS_H

#include <QObject>

class Signals final : public QObject {
    Q_OBJECT

public:
    // 获取单例实例
    static Signals* instance();

    void emitWriteJson(int id);

signals:
    // 定义公共信号
    void writejson(int id);
    void itemSelected(const QString &itemName);

private:
    // 私有构造函数以防止直接实例化
    explicit Signals(QObject *parent = nullptr);
    // 静态指针用于存储单例实例
    static Signals* _instance;
};

#endif // SIGNALS_H
