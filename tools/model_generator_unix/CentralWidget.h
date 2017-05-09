/*
 * File: CentralWidget.h
 */
#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QWidget>
#include "ui/uic_centralListWidget.h"

class CCentralWidget : public QWidget 
{
	public:
		CCentralWidget(QWidget *parent = NULL);
		~CCentralWidget();
		void appendStateString(const QString& str);
	private:
		Ui::centralWidget ui;
};

#endif
