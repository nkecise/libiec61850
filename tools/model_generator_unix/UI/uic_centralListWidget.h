/********************************************************************************
** Form generated from reading UI file 'centralListWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIC_CENTRALLISTWIDGET_H
#define UIC_CENTRALLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_centralWidget
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *centralWidget)
    {
        if (centralWidget->objectName().isEmpty())
            centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->resize(400, 300);
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(11);
        centralWidget->setFont(font);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Fixedsys"));
        listWidget->setFont(font1);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setAlternatingRowColors(true);
        listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(listWidget);


        retranslateUi(centralWidget);

        QMetaObject::connectSlotsByName(centralWidget);
    } // setupUi

    void retranslateUi(QWidget *centralWidget)
    {
        centralWidget->setWindowTitle(QApplication::translate("centralWidget", "CentralListWidget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class centralWidget: public Ui_centralWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIC_CENTRALLISTWIDGET_H
