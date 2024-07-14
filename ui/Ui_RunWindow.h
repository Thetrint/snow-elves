/********************************************************************************
** Form generated from reading UI file 'RunWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNWINDOW_H
#define UI_RUNWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
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
    QGridLayout *gridLayout_3;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QWidget *widget_3;
    QGridLayout *gridLayout;
    QPushButton *pushButton_7;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_8;

    void setupUi(QWidget *RunWindow)
    {
        if (RunWindow->objectName().isEmpty())
            RunWindow->setObjectName("RunWindow");
        RunWindow->resize(828, 655);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(RunWindow->sizePolicy().hasHeightForWidth());
        RunWindow->setSizePolicy(sizePolicy);
        RunWindow->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(RunWindow);
        gridLayout_3->setObjectName("gridLayout_3");
        widget_2 = new QWidget(RunWindow);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(widget_2);
        textEdit->setObjectName("textEdit");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(4);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy2);
        textEdit->setReadOnly(true);

        gridLayout_2->addWidget(textEdit, 0, 0, 1, 1);


        gridLayout_3->addWidget(widget_2, 0, 1, 3, 1);

        widget = new QWidget(RunWindow);
        widget->setObjectName("widget");
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(249, 0));
        widget->setMaximumSize(QSize(249, 16777215));
        widget->setSizeIncrement(QSize(0, 0));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName("tableWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy3);
        tableWidget->setMinimumSize(QSize(248, 260));
        tableWidget->setMaximumSize(QSize(248, 260));

        verticalLayout->addWidget(tableWidget);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(8);
        sizePolicy4.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy4);
        gridLayout = new QGridLayout(widget_3);
        gridLayout->setObjectName("gridLayout");
        pushButton_7 = new QPushButton(widget_3);
        pushButton_7->setObjectName("pushButton_7");

        gridLayout->addWidget(pushButton_7, 4, 0, 1, 2);

        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName("pushButton_4");

        gridLayout->addWidget(pushButton_4, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(widget_3);
        pushButton_3->setObjectName("pushButton_3");

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName("pushButton_5");

        gridLayout->addWidget(pushButton_5, 2, 0, 1, 1);

        pushButton_6 = new QPushButton(widget_3);
        pushButton_6->setObjectName("pushButton_6");

        gridLayout->addWidget(pushButton_6, 2, 1, 1, 1);

        pushButton_2 = new QPushButton(widget_3);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName("pushButton");
        pushButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_8 = new QPushButton(widget_3);
        pushButton_8->setObjectName("pushButton_8");

        gridLayout->addWidget(pushButton_8, 3, 0, 1, 2);


        verticalLayout->addWidget(widget_3);


        gridLayout_3->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(RunWindow);

        QMetaObject::connectSlotsByName(RunWindow);
    } // setupUi

    void retranslateUi(QWidget *RunWindow)
    {
        RunWindow->setWindowTitle(QCoreApplication::translate("RunWindow", "Form", nullptr));
        pushButton_7->setText(QCoreApplication::translate("RunWindow", "\345\274\200\345\247\213", nullptr));
        pushButton_4->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\346\201\242\345\244\215", nullptr));
        pushButton_3->setText(QCoreApplication::translate("RunWindow", "\346\201\242\345\244\215", nullptr));
        pushButton_5->setText(QCoreApplication::translate("RunWindow", "\350\247\243\347\273\221", nullptr));
        pushButton_6->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\350\247\243\347\273\221", nullptr));
        pushButton_2->setText(QCoreApplication::translate("RunWindow", "\345\205\250\351\203\250\346\232\202\345\201\234", nullptr));
        pushButton->setText(QCoreApplication::translate("RunWindow", "\346\232\202\345\201\234", nullptr));
        pushButton_8->setText(QCoreApplication::translate("RunWindow", "\346\210\252\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RunWindow: public Ui_RunWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNWINDOW_H
