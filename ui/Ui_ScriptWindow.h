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
    QWidget *page_4;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_10;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_5;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_11;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_12;
    QComboBox *comboBox_4;
    QLabel *label_13;
    QComboBox *comboBox_5;
    QLabel *label_14;
    QComboBox *comboBox_6;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_16;
    QComboBox *comboBox_7;
    QLabel *label_17;
    QComboBox *comboBox_8;
    QLabel *label_15;
    QComboBox *comboBox_9;
    QWidget *widget_10;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_6;
    QLabel *label_18;
    QComboBox *comboBox_10;
    QCheckBox *checkBox_7;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *listWidget_2;
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

    void setupUi(QWidget *ScriptWindow)
    {
        if (ScriptWindow->objectName().isEmpty())
            ScriptWindow->setObjectName("ScriptWindow");
        ScriptWindow->resize(923, 629);
        gridLayout = new QGridLayout(ScriptWindow);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(5, 5, 5, 0);
        stackedWidget = new QStackedWidget(ScriptWindow);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(6);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
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
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        widget_5 = new QWidget(page_2);
        widget_5->setObjectName("widget_5");
        horizontalLayout_2 = new QHBoxLayout(widget_5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(widget_5);
        label_4->setObjectName("label_4");
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_4);

        spinBox = new QSpinBox(widget_5);
        spinBox->setObjectName("spinBox");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
        spinBox->setSizePolicy(sizePolicy2);

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
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(label_6);

        widget_6 = new QWidget(page_3);
        widget_6->setObjectName("widget_6");
        horizontalLayout_3 = new QHBoxLayout(widget_6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_7 = new QLabel(widget_6);
        label_7->setObjectName("label_7");
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_7);

        lineEdit = new QLineEdit(widget_6);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout_3->addWidget(lineEdit);

        label_8 = new QLabel(widget_6);
        label_8->setObjectName("label_8");
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);

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
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);

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
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        verticalLayout_5 = new QVBoxLayout(page_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_10 = new QLabel(page_4);
        label_10->setObjectName("label_10");
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);
        label_10->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_5->addWidget(label_10);

        widget_7 = new QWidget(page_4);
        widget_7->setObjectName("widget_7");
        horizontalLayout_4 = new QHBoxLayout(widget_7);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        checkBox_3 = new QCheckBox(widget_7);
        checkBox_3->setObjectName("checkBox_3");

        horizontalLayout_4->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(widget_7);
        checkBox_4->setObjectName("checkBox_4");

        horizontalLayout_4->addWidget(checkBox_4);

        checkBox_5 = new QCheckBox(widget_7);
        checkBox_5->setObjectName("checkBox_5");

        horizontalLayout_4->addWidget(checkBox_5);

        horizontalSpacer_3 = new QSpacerItem(565, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_5->addWidget(widget_7);

        verticalSpacer_3 = new QSpacerItem(20, 576, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        stackedWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        verticalLayout_6 = new QVBoxLayout(page_5);
        verticalLayout_6->setObjectName("verticalLayout_6");
        label_11 = new QLabel(page_5);
        label_11->setObjectName("label_11");
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);
        label_11->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(label_11);

        widget_8 = new QWidget(page_5);
        widget_8->setObjectName("widget_8");
        horizontalLayout_5 = new QHBoxLayout(widget_8);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_12 = new QLabel(widget_8);
        label_12->setObjectName("label_12");
        sizePolicy1.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_12);

        comboBox_4 = new QComboBox(widget_8);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");

        horizontalLayout_5->addWidget(comboBox_4);

        label_13 = new QLabel(widget_8);
        label_13->setObjectName("label_13");
        sizePolicy1.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_13);

        comboBox_5 = new QComboBox(widget_8);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");

        horizontalLayout_5->addWidget(comboBox_5);

        label_14 = new QLabel(widget_8);
        label_14->setObjectName("label_14");
        sizePolicy1.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_14);

        comboBox_6 = new QComboBox(widget_8);
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->setObjectName("comboBox_6");

        horizontalLayout_5->addWidget(comboBox_6);


        verticalLayout_6->addWidget(widget_8);

        widget_9 = new QWidget(page_5);
        widget_9->setObjectName("widget_9");
        horizontalLayout_6 = new QHBoxLayout(widget_9);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_16 = new QLabel(widget_9);
        label_16->setObjectName("label_16");
        sizePolicy1.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_16);

        comboBox_7 = new QComboBox(widget_9);
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->setObjectName("comboBox_7");

        horizontalLayout_6->addWidget(comboBox_7);

        label_17 = new QLabel(widget_9);
        label_17->setObjectName("label_17");
        sizePolicy1.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_17);

        comboBox_8 = new QComboBox(widget_9);
        comboBox_8->addItem(QString());
        comboBox_8->addItem(QString());
        comboBox_8->addItem(QString());
        comboBox_8->setObjectName("comboBox_8");

        horizontalLayout_6->addWidget(comboBox_8);

        label_15 = new QLabel(widget_9);
        label_15->setObjectName("label_15");
        sizePolicy1.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_15);

        comboBox_9 = new QComboBox(widget_9);
        comboBox_9->addItem(QString());
        comboBox_9->addItem(QString());
        comboBox_9->addItem(QString());
        comboBox_9->setObjectName("comboBox_9");

        horizontalLayout_6->addWidget(comboBox_9);


        verticalLayout_6->addWidget(widget_9);

        widget_10 = new QWidget(page_5);
        widget_10->setObjectName("widget_10");
        horizontalLayout_7 = new QHBoxLayout(widget_10);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        checkBox_6 = new QCheckBox(widget_10);
        checkBox_6->setObjectName("checkBox_6");

        horizontalLayout_7->addWidget(checkBox_6);

        label_18 = new QLabel(widget_10);
        label_18->setObjectName("label_18");
        sizePolicy1.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(label_18);

        comboBox_10 = new QComboBox(widget_10);
        comboBox_10->addItem(QString());
        comboBox_10->addItem(QString());
        comboBox_10->setObjectName("comboBox_10");

        horizontalLayout_7->addWidget(comboBox_10);

        checkBox_7 = new QCheckBox(widget_10);
        checkBox_7->setObjectName("checkBox_7");

        horizontalLayout_7->addWidget(checkBox_7);

        horizontalSpacer_4 = new QSpacerItem(565, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_6->addWidget(widget_10);

        verticalSpacer_4 = new QSpacerItem(20, 390, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        stackedWidget->addWidget(page_5);

        gridLayout->addWidget(stackedWidget, 1, 2, 1, 1);

        widget_3 = new QWidget(ScriptWindow);
        widget_3->setObjectName("widget_3");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy3);
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

        widget = new QWidget(ScriptWindow);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(1);
        sizePolicy4.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy4);

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
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(10);
        sizePolicy5.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy5);
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


        retranslateUi(ScriptWindow);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(ScriptWindow);
    } // setupUi

    void retranslateUi(QWidget *ScriptWindow)
    {
        ScriptWindow->setWindowTitle(QCoreApplication::translate("ScriptWindow", "Form", nullptr));
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

        label_10->setText(QCoreApplication::translate("ScriptWindow", "\346\257\217\346\227\245\345\205\221\346\215\242\344\273\273\345\212\241\350\256\276\347\275\256", nullptr));
        label_10->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "page", nullptr)));
        checkBox_3->setText(QCoreApplication::translate("ScriptWindow", "\351\223\266\347\245\250\347\244\274\347\233\222", nullptr));
        checkBox_4->setText(QCoreApplication::translate("ScriptWindow", "\345\270\256\346\264\276\351\223\234\351\222\261\346\215\220\347\214\256", nullptr));
        checkBox_5->setText(QCoreApplication::translate("ScriptWindow", "\345\270\256\346\264\276\351\223\266\344\270\244\346\215\220\347\214\256", nullptr));
        label_11->setText(QCoreApplication::translate("ScriptWindow", "\345\256\227\351\227\250\344\273\273\345\212\241\350\256\276\347\275\256", nullptr));
        label_11->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "page", nullptr)));
        label_12->setText(QCoreApplication::translate("ScriptWindow", "\345\256\227\351\227\250\350\257\225\347\202\274:", nullptr));
        label_12->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_4->setItemText(0, QCoreApplication::translate("ScriptWindow", "~", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("ScriptWindow", "\344\272\224\345\206\233\344\271\213\347\202\274", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("ScriptWindow", "\344\270\203\346\230\237\344\271\213\347\202\274", nullptr));
        comboBox_4->setItemText(3, QCoreApplication::translate("ScriptWindow", "\345\205\253\351\227\250\344\271\213\347\202\274", nullptr));

        label_13->setText(QCoreApplication::translate("ScriptWindow", "\345\256\227\351\227\250\350\257\225\347\202\274:", nullptr));
        label_13->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_5->setItemText(0, QCoreApplication::translate("ScriptWindow", "~", nullptr));
        comboBox_5->setItemText(1, QCoreApplication::translate("ScriptWindow", "\344\272\224\345\206\233\344\271\213\347\202\274", nullptr));
        comboBox_5->setItemText(2, QCoreApplication::translate("ScriptWindow", "\344\270\203\346\230\237\344\271\213\347\202\274", nullptr));
        comboBox_5->setItemText(3, QCoreApplication::translate("ScriptWindow", "\345\205\253\351\227\250\344\271\213\347\202\274", nullptr));

        label_14->setText(QCoreApplication::translate("ScriptWindow", "\345\256\227\351\227\250\350\257\225\347\202\274:", nullptr));
        label_14->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_6->setItemText(0, QCoreApplication::translate("ScriptWindow", "~", nullptr));
        comboBox_6->setItemText(1, QCoreApplication::translate("ScriptWindow", "\344\272\224\345\206\233\344\271\213\347\202\274", nullptr));
        comboBox_6->setItemText(2, QCoreApplication::translate("ScriptWindow", "\344\270\203\346\230\237\344\271\213\347\202\274", nullptr));
        comboBox_6->setItemText(3, QCoreApplication::translate("ScriptWindow", "\345\205\253\351\227\250\344\271\213\347\202\274", nullptr));

        label_16->setText(QCoreApplication::translate("ScriptWindow", "\351\230\237\344\274\215:      ", nullptr));
        label_16->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_7->setItemText(0, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\200\351\230\237", nullptr));
        comboBox_7->setItemText(1, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\272\214\351\230\237", nullptr));
        comboBox_7->setItemText(2, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\211\351\230\237", nullptr));

        label_17->setText(QCoreApplication::translate("ScriptWindow", "\351\230\237\344\274\215:      ", nullptr));
        label_17->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_8->setItemText(0, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\200\351\230\237", nullptr));
        comboBox_8->setItemText(1, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\272\214\351\230\237", nullptr));
        comboBox_8->setItemText(2, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\211\351\230\237", nullptr));

        label_15->setText(QCoreApplication::translate("ScriptWindow", "\351\230\237\344\274\215:      ", nullptr));
        label_15->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_9->setItemText(0, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\200\351\230\237", nullptr));
        comboBox_9->setItemText(1, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\272\214\351\230\237", nullptr));
        comboBox_9->setItemText(2, QCoreApplication::translate("ScriptWindow", "\347\254\254\344\270\211\351\230\237", nullptr));

        checkBox_6->setText(QCoreApplication::translate("ScriptWindow", "\345\256\227\351\227\250\347\224\237\344\272\247", nullptr));
        label_18->setText(QCoreApplication::translate("ScriptWindow", "\345\277\203\346\203\205\347\255\211\347\272\247:", nullptr));
        label_18->setProperty("class", QVariant(QCoreApplication::translate("ScriptWindow", "tip", nullptr)));
        comboBox_10->setItemText(0, QCoreApplication::translate("ScriptWindow", "\351\253\230", nullptr));
        comboBox_10->setItemText(1, QCoreApplication::translate("ScriptWindow", "\344\275\216", nullptr));

        checkBox_7->setText(QCoreApplication::translate("ScriptWindow", "\344\270\200\351\224\256\345\202\254\345\221\275", nullptr));
        label_2->setText(QCoreApplication::translate("ScriptWindow", "\346\211\247\350\241\214\345\210\227\350\241\250", nullptr));
        pushButton->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\205\245\351\205\215\347\275\256", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ScriptWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ScriptWindow", "\345\210\240\351\231\244\351\205\215\347\275\256", nullptr));
        pushButton_4->setText(QCoreApplication::translate("ScriptWindow", "\345\257\274\345\207\272\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("ScriptWindow", "\344\273\273\345\212\241\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScriptWindow: public Ui_ScriptWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTWINDOW_H
