/*
 * File: CentralWidget.cpp
 */
#include "CentralWidget.h"

CCentralWidget::CCentralWidget(QWidget *parent)
{
	ui.setupUi(this);
}

void CCentralWidget::appendStateString(const QString& str)
{
	(ui.listWidget)->addItem(str);
}

CCentralWidget::~CCentralWidget()
{
}

