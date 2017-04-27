/*
 * File: GenConfig.cpp
 */
#include <QApplication>
#include "MainFrame.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow frame;

	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	frame.setWindowTitle(QString("GenConfig"));
	frame.show();

	return(app.exec());
}
