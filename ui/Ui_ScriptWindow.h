/********************************************************************************
** Form generated from reading UI file 'ScriptWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTWINDOW_H
#define UI_SCRIPTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScriptWindow
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidget;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *listWidget_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QWidget *ScriptWindow)
    {
        if (ScriptWindow->objectName().isEmpty())
            ScriptWindow->setObjectName("ScriptWindow");
        ScriptWindow->resize(694, 545);
        gridLayout = new QGridLayout(ScriptWindow);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(5, 5, 5, 0);
        widget = new QWidget(ScriptWindow);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(widget, 0, 0, 1, 2);

        widget_2 = new QWidget(ScriptWindow);
        widget_2->setObjectName("widget_2");
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        comboBox = new QComboBox(widget_2);
        comboBox->setObjectName("comboBox");
        comboBox->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(comboBox);

        pushButton_3 = new QPushButton(widget_2);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName("pushButton_4");

        horizontalLayout->addWidget(pushButton_4);


        gridLayout->addWidget(widget_2, 0, 2, 1, 1);

        widget_4 = new QWidget(ScriptWindow);
        widget_4->setObjectName("widget_4");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(10);
        sizePolicy1.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(widget_4);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(widget_4);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(widget_4);
        listWidget->setObjectName("listWidget");

        verticalLayout->addWidget(listWidget);


        gridLayout->addWidget(widget_4, 1, 0, 1, 1);

        widget_3 = new QWidget(ScriptWindow);
        widget_3->setObjectName("widget_3");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(widget_3);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        listWidget_2 = new QListWidget(widget_3);
        listWidget_2->setObjectName("listWidget_2");

        verticalLayout_2->addWidget(listWidget_2);


        gridLayout->addWidget(widget_3, 1, 1, 1, 1);

        stackedWidget = new QStackedWidget(ScriptWindow);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(5);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy3);
        page = new QWidget();
        page->setObjectName("page");
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 1, 2, 1, 1);


        retranslateUi(ScriptWindow);

        QMetaObject::connectSlotsByName(ScriptWindow);
    } // setupUi

    void retranslateUi(QWidget *ScriptWindow)
    {
        ScriptWindow->setWindowTitle(QCoreApplication::translate("ScriptWindow", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\205\245\351\205\215\347\275\256", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ScriptWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ScriptWindow", "\345\210\240\351\231\244\351\205\215\347\275\256", nullptr));
        pushButton_4->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\207\272\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("ScriptWindow", "\344\273\273\345\212\241\345\210\227\350\241\250", nullptr));
        label_2->setText(QCoreApplication::translate("ScriptWindow", "\346\211\247\350\241\214\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScriptWindow: public Ui_ScriptWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTWINDOW_H
