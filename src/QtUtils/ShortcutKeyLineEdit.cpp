//
// Created by y1726 on 24-9-3.
//
#include "QtUtils/ShortcutKeyLineEdit.h"


void ShortcutKeyLineEdit::keyPressEvent(QKeyEvent* event) {
    this->clear();

    // 获取按键代码和修饰符
    int keyCode = event->key();
    const Qt::KeyboardModifiers modifiers = event->modifiers();

    // 验证快捷键：必须包含至少一个修饰符
    if (!(modifiers & (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier))) {
        // 如果没有修饰符，忽略输入
        return;
    }

    // 构建快捷键字符串
    QString keyText;

    // 检查并添加修饰符
    if (modifiers & Qt::ControlModifier) keyText.append("Ctrl+");
    if (modifiers & Qt::AltModifier) keyText.append("Alt+");
    if (modifiers & Qt::ShiftModifier) keyText.append("Shift+");

    // 使用 keyCode 来获取键值
    if ((keyCode >= Qt::Key_A && keyCode <= Qt::Key_Z) || (keyCode >= Qt::Key_0 && keyCode <= Qt::Key_9)) {
        // 字母和数字按键
        keyText.append(QChar(keyCode));
    } else if (keyCode != Qt::Key_Control && keyCode != Qt::Key_Alt && keyCode != Qt::Key_Shift) {
        // 其他按键，使用标准的 QKeySequence 处理，但排除修饰键本身
        keyText.append(QKeySequence(keyCode).toString());
    }


    // 设置文本
    this->setText(keyText);

    // 记录日志
    spdlog::info("Shortcut detected: {} {}", keyCode, keyText.toStdString());
}