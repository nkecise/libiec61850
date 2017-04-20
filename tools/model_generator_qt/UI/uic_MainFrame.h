/********************************************************************************
** Form generated from reading UI file 'MainFrame.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIC_MAINFRAME_H
#define UIC_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainFrame
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *sclLineEdit;
    QPushButton *choseSclPushButton;
    QGroupBox *groupBox_2;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *iedComboBox;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *apComboBox;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *outLineEdit;
    QPushButton *choseCfgPushButton;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *generatePushButton;

    void setupUi(QWidget *MainFrame)
    {
        if (MainFrame->objectName().isEmpty())
            MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
        MainFrame->resize(503, 281);
        verticalLayout_2 = new QVBoxLayout(MainFrame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(MainFrame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 441, 25));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sclLineEdit = new QLineEdit(widget);
        sclLineEdit->setObjectName(QString::fromUtf8("sclLineEdit"));
        sclLineEdit->setReadOnly(true);

        horizontalLayout->addWidget(sclLineEdit);

        choseSclPushButton = new QPushButton(widget);
        choseSclPushButton->setObjectName(QString::fromUtf8("choseSclPushButton"));

        horizontalLayout->addWidget(choseSclPushButton);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MainFrame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        widget1 = new QWidget(groupBox_2);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(20, 30, 441, 24));
        horizontalLayout_4 = new QHBoxLayout(widget1);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(widget1);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        iedComboBox = new QComboBox(widget1);
        iedComboBox->setObjectName(QString::fromUtf8("iedComboBox"));
        iedComboBox->setMinimumSize(QSize(160, 0));

        horizontalLayout_2->addWidget(iedComboBox);


        horizontalLayout_4->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        apComboBox = new QComboBox(widget1);
        apComboBox->setObjectName(QString::fromUtf8("apComboBox"));

        horizontalLayout_3->addWidget(apComboBox);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(MainFrame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        layoutWidget_2 = new QWidget(groupBox_3);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(20, 30, 441, 25));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        outLineEdit = new QLineEdit(layoutWidget_2);
        outLineEdit->setObjectName(QString::fromUtf8("outLineEdit"));
        outLineEdit->setReadOnly(true);

        horizontalLayout_5->addWidget(outLineEdit);

        choseCfgPushButton = new QPushButton(layoutWidget_2);
        choseCfgPushButton->setObjectName(QString::fromUtf8("choseCfgPushButton"));

        horizontalLayout_5->addWidget(choseCfgPushButton);


        verticalLayout->addWidget(groupBox_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_2 = new QSpacerItem(388, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        generatePushButton = new QPushButton(MainFrame);
        generatePushButton->setObjectName(QString::fromUtf8("generatePushButton"));

        horizontalLayout_6->addWidget(generatePushButton);


        verticalLayout->addLayout(horizontalLayout_6);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(MainFrame);

        QMetaObject::connectSlotsByName(MainFrame);
    } // setupUi

    void retranslateUi(QWidget *MainFrame)
    {
        MainFrame->setWindowTitle(QApplication::translate("MainFrame", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainFrame", "Step 1. Select SCL File", 0, QApplication::UnicodeUTF8));
        choseSclPushButton->setText(QApplication::translate("MainFrame", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainFrame", "Setp 2. Select IED && AP", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainFrame", "IED:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainFrame", "AccessPoint:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainFrame", "Step 3. Select Output File Name", 0, QApplication::UnicodeUTF8));
        choseCfgPushButton->setText(QApplication::translate("MainFrame", "Browse", 0, QApplication::UnicodeUTF8));
        generatePushButton->setText(QApplication::translate("MainFrame", "Generate!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIC_MAINFRAME_H
