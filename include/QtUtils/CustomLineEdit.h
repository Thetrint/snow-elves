//
// Created by y1726 on 24-8-4.
//
#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H
#include "main.h"

class CustomLineEdit final : public QLineEdit {
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget* parent = nullptr)  : QLineEdit(parent) {};

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    static QMap<int, QString> keyDict;
};

#endif //CUSTOMLINEEDIT_H
