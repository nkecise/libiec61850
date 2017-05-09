/*
 * File: IedTreeWidget.h
 */
#ifndef IED_TREE_WIDGET_H
#define IED_TREE_WIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

class CIedTreeWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		CIedTreeWidget(QWidget *parent = NULL);
		~CIedTreeWidget();
	signals:
		void mouseRightButtonRelease(const QPoint& P);
	protected:
		void mouseReleaseEvent(QMouseEvent *e);
};

#endif
