/*
 * File: IedTreeWidget.cpp
 */
#include <QMessageBox>
#include "IedTreeWidget.h"

CIedTreeWidget::CIedTreeWidget(QWidget *parent)
{
}

void CIedTreeWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::RightButton)
	{
		emit mouseRightButtonRelease(e->pos());
	}
	QTreeWidget::mouseReleaseEvent(e);
}

CIedTreeWidget::~CIedTreeWidget()
{
}
