/********************************************************************************
** Form generated from reading UI file 'RunWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNWINDOW_H
#define UI_RUNWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RunWindow
{
public:
    QHBoxLayout *hboxLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QWidget *widget_3;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *RunWindow)
    {
        if (RunWindow->objectName().isEmpty())
            RunWindow->setObjectName("RunWindow");
        RunWindow->resize(828, 564);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(RunWindow->sizePolicy().hasHeightForWidth());
        RunWindow->setSizePolicy(sizePolicy);
        hboxLayout = new QHBoxLayout(RunWindow);
        hboxLayout->setSpacing(0);
        hboxLayout->setObjectName("hboxLayout");
        hboxLayout->setContentsMargins(5, 5, 0, 0);
        widget = new QWidget(RunWindow);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName("tableWidget");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(tableWidget);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy3);
        gridLayout = new QGridLayout(widget_3);
        gridLayout->setObjectName("gridLayout");
        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(widget_3);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

        pushButton_3 = new QPushButton(widget_3);
        pushButton_3->setObjectName("pushButton_3");

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName("pushButton_4");

        gridLayout->addWidget(pushButton_4, 1, 1, 1, 1);

        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName("pushButton_5");

        gridLayout->addWidget(pushButton_5, 2, 0, 1, 1);

        pushButton_6 = new QPushButton(widget_3);
        pushButton_6->setObjectName("pushButton_6");

        gridLayout->addWidget(pushButton_6, 2, 1, 1, 1);

        pushButton_7 = new QPushButton(widget_3);
        pushButton_7->setObjectName("pushButton_7");

        gridLayout->addWidget(pushButton_7, 3, 0, 1, 2);


        verticalLayout->addWidget(widget_3);


        hboxLayout->addWidget(widget);

        widget_2 = new QWidget(RunWindow);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(2);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy4);
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(widget_2);
        textEdit->setObjectName("textEdit");

        horizontalLayout->addWidget(textEdit);


        hboxLayout->addWidget(widget_2);


        retranslateUi(RunWindow);

        QMetaObject::connectSlotsByName(RunWindow);
    } // setupUi

    void retranslateUi(QWidget *RunWindow)
    {
        RunWindow->setWindowTitle(QCoreApplication::translate("RunWindow", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("RunWindow", "\346\232\202\345\201\234", nullptr));
        pushButton_2->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\346\232\202\345\201\234", nullptr));
        pushButton_3->setText(QCoreApplication::translate("RunWindow", "\346\201\242\345\244\215", nullptr));
        pushButton_4->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\346\201\242\345\244\215", nullptr));
        pushButton_5->setText(QCoreApplication::translate("RunWindow", "\350\247\243\347\273\221", nullptr));
        pushButton_6->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\350\247\243\347\273\221", nullptr));
        pushButton_7->setText(QCoreApplication::translate("RunWindow", "\345\274\200\345\247\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RunWindow: public Ui_RunWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNWINDOW_H
