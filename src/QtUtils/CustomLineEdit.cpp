//
// Created by y1726 on 24-8-4.
//
#include "QtUtils/CustomLineEdit.h"

QMap<int, QString> CustomLineEdit::keyDict = {
    {Qt::Key_0, "0"}, {Qt::Key_1, "1"}, {Qt::Key_2, "2"},
    {Qt::Key_3, "3"}, {Qt::Key_4, "4"}, {Qt::Key_5, "5"},
    {Qt::Key_6, "6"}, {Qt::Key_7, "7"}, {Qt::Key_8, "8"},
    {Qt::Key_9, "9"}, {Qt::Key_A, "A"}, {Qt::Key_B, "B"},
    {Qt::Key_C, "C"}, {Qt::Key_D, "D"}, {Qt::Key_E, "E"},
    {Qt::Key_F, "F"}, {Qt::Key_G, "G"}, {Qt::Key_H, "H"},
    {Qt::Key_I, "I"}, {Qt::Key_J, "J"}, {Qt::Key_K, "K"},
    {Qt::Key_L, "L"}, {Qt::Key_M, "M"}, {Qt::Key_N, "N"},
    {Qt::Key_O, "O"}, {Qt::Key_P, "P"}, {Qt::Key_Q, "Q"},
    {Qt::Key_R, "R"}, {Qt::Key_S, "S"}, {Qt::Key_T, "T"},
    {Qt::Key_U, "U"}, {Qt::Key_V, "V"}, {Qt::Key_W, "W"},
    {Qt::Key_X, "X"}, {Qt::Key_Y, "Y"}, {Qt::Key_Z, "Z"},
    {Qt::Key_Shift, "Shift"}, {Qt::Key_Return, "Enter"},
    {Qt::Key_Control, "Ctrl"}, {Qt::Key_Escape, "ESC"},
    {Qt::Key_Space, "Space"}
};


void CustomLineEdit::keyPressEvent(QKeyEvent* event) {
    // 清除当前文本
    this->clear();

    // 获取按键代码
    const int keyCode = event->key();

    // 查找按键对应的文本，找不到则为空字符串
    QString keyText = keyDict.value(keyCode, QString());

    // 如果按下的是小键盘的按键，前面加上 "Num"
    if (event->modifiers() & Qt::KeypadModifier && !keyText.isEmpty()) {
        keyText.prepend("Num");
    }

    // 设置文本
    if (!keyText.isEmpty()) {
        this->setText(keyText);
    }
}
