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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScriptWindow
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *listWidget_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QSpinBox *spinBox;
    QCheckBox *checkBox;
    QLabel *label_5;
    QSpinBox *spinBox_2;
    QCheckBox *checkBox_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QLineEdit *lineEdit;
    QLabel *label_8;
    QComboBox *comboBox_2;
    QLabel *label_9;
    QComboBox *comboBox_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *ScriptWindow)
    {
        if (ScriptWindow->objectName().isEmpty())
            ScriptWindow->setObjectName("ScriptWindow");
        ScriptWindow->resize(923, 737);
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
        sizePolicy3.setHorizontalStretch(6);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy3);
        page = new QWidget();
        page->setObjectName("page");
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(page_2);
        label_3->setObjectName("label_3");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy4);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        widget_5 = new QWidget(page_2);
        widget_5->setObjectName("widget_5");
        horizontalLayout_2 = new QHBoxLayout(widget_5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(widget_5);
        label_4->setObjectName("label_4");
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(label_4);

        spinBox = new QSpinBox(widget_5);
        spinBox->setObjectName("spinBox");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
        spinBox->setSizePolicy(sizePolicy5);

        horizontalLayout_2->addWidget(spinBox);

        checkBox = new QCheckBox(widget_5);
        checkBox->setObjectName("checkBox");

        horizontalLayout_2->addWidget(checkBox);

        label_5 = new QLabel(widget_5);
        label_5->setObjectName("label_5");

        horizontalLayout_2->addWidget(label_5);

        spinBox_2 = new QSpinBox(widget_5);
        spinBox_2->setObjectName("spinBox_2");

        horizontalLayout_2->addWidget(spinBox_2);

        checkBox_2 = new QCheckBox(widget_5);
        checkBox_2->setObjectName("checkBox_2");

        horizontalLayout_2->addWidget(checkBox_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(widget_5);

        verticalSpacer = new QSpacerItem(20, 393, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        verticalLayout_4 = new QVBoxLayout(page_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_6 = new QLabel(page_3);
        label_6->setObjectName("label_6");
        sizePolicy4.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy4);
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(label_6);

        widget_6 = new QWidget(page_3);
        widget_6->setObjectName("widget_6");
        horizontalLayout_3 = new QHBoxLayout(widget_6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_7 = new QLabel(widget_6);
        label_7->setObjectName("label_7");
        sizePolicy4.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_7);

        lineEdit = new QLineEdit(widget_6);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout_3->addWidget(lineEdit);

        label_8 = new QLabel(widget_6);
        label_8->setObjectName("label_8");
        sizePolicy4.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_8);

        comboBox_2 = new QComboBox(widget_6);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        horizontalLayout_3->addWidget(comboBox_2);

        label_9 = new QLabel(widget_6);
        label_9->setObjectName("label_9");
        sizePolicy4.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_9);

        comboBox_3 = new QComboBox(widget_6);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");

        horizontalLayout_3->addWidget(comboBox_3);

        horizontalSpacer_2 = new QSpacerItem(113, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_4->addWidget(widget_6);

        verticalSpacer_2 = new QSpacerItem(20, 576, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        stackedWidget->addWidget(page_3);

        gridLayout->addWidget(stackedWidget, 1, 2, 1, 1);


        retranslateUi(ScriptWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(ScriptWindow);
    } // setupUi

    void retranslateUi(QWidget *ScriptWindow)
    {
        ScriptWindow->setWindowTitle(QCoreApplication::translate("ScriptWindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("ScriptWindow", "\344\273\273\345\212\241\345\210\227\350\241\250", nullptr));
        pushButton->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\205\245\351\205\215\347\275\256", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ScriptWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ScriptWindow", "\345\210\240\351\231\244\351\205\215\347\275\256", nullptr));
        pushButton_4->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\207\272\351\205\215\347\275\256", nullptr));
        label_2->setText(QCoreApplication::translate("ScriptWindow", "\346\211\247\350\241\214\345\210\227\350\241\250", nullptr));
        label_3->setText(QCoreApplication::translate("ScriptWindow", "\345\237\272\347\241\200\344\273\273\345\212\241\350\256\276\347\275\256", nullptr));
        label_3->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "page", nullptr)));
        label_4->setText(QCoreApplication::translate("ScriptWindow", "\345\215\216\345\261\261\350\256\272\345\211\221\346\254\241\346\225\260:", nullptr));
        label_4->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        checkBox->setText(QCoreApplication::translate("ScriptWindow", "\345\215\216\345\261\261\350\256\272\345\211\221\347\247\222\351\200\200", nullptr));
        label_5->setText(QCoreApplication::translate("ScriptWindow", "\346\261\237\346\271\226\350\213\261\351\233\204\346\246\234\346\254\241\346\225\260", nullptr));
        label_5->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        checkBox_2->setText(QCoreApplication::translate("ScriptWindow", "\346\261\237\346\271\226\350\213\261\351\233\204\346\246\234\347\247\222\351\200\200", nullptr));
        label_6->setText(QCoreApplication::translate("ScriptWindow", "\346\227\245\345\270\270\346\202\254\350\265\217\344\273\273\345\212\241\350\256\276\347\275\256", nullptr));
        label_6->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "page", nullptr)));
        label_7->setText(QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\254\345\226\212\350\257\235\345\206\205\345\256\271:", nullptr));
        label_7->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        label_8->setText(QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\254\345\274\200\345\220\257\344\272\272\346\225\260:", nullptr));
        label_8->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_2->setItemText(0, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2541\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2542\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2543\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(3, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2544\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(4, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2545\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(5, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2546\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(6, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2547\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(7, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2548\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(8, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\2549\344\272\272\345\274\200", nullptr));
        comboBox_2->setItemText(9, QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\25410\344\272\272\345\274\200", nullptr));

        label_9->setText(QCoreApplication::translate("ScriptWindow", "\345\211\257\346\234\254\346\250\241\345\274\217:", nullptr));
        label_9->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_3->setItemText(0, QCoreApplication::translate("ScriptWindow", "\345\270\246\351\230\237\346\250\241\345\274\217", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("ScriptWindow", "\345\233\272\345\256\232\351\230\237\346\250\241\345\274\217", nullptr));

    } // retranslateUi

};

namespace Ui {
    class ScriptWindow: public Ui_ScriptWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTWINDOW_H
