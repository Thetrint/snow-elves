/********************************************************************************
** Form generated from reading UI file 'RenewWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENEWWINDOW_H
#define UI_RENEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RenewWindow
{
public:
    QProgressBar *progressBar;

    void setupUi(QWidget *RenewWindow)
    {
        if (RenewWindow->objectName().isEmpty())
            RenewWindow->setObjectName("RenewWindow");
        RenewWindow->resize(400, 300);
        progressBar = new QProgressBar(RenewWindow);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(50, 150, 118, 23));
        progressBar->setValue(24);

        retranslateUi(RenewWindow);

        QMetaObject::connectSlotsByName(RenewWindow);
    } // setupUi

    void retranslateUi(QWidget *RenewWindow)
    {
        RenewWindow->setWindowTitle(QCoreApplication::translate("RenewWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenewWindow: public Ui_RenewWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENEWWINDOW_H
