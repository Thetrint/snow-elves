//
// Created by y1726 on 24-9-3.
//

#ifndef SHORTCUTKEYLINEEDIT_H
#define SHORTCUTKEYLINEEDIT_H
#include "main.h"

class ShortcutKeyLineEdit final : public QLineEdit {
    Q_OBJECT

public:
    explicit ShortcutKeyLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {};

protected:
    void keyPressEvent(QKeyEvent* event) override;

};
#endif //SHORTCUTKEYLINEEDIT_H
