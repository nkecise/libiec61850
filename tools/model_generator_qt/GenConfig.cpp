/*
 * File: GenConfig.cpp
 */
#include "MainFrame.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainFrame	frame;

	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	frame.setWindowFlags(Qt::WindowMinimizeButtonHint);
	frame.setWindowTitle(QString("GenConfig"));
	frame.show();

	return(app.exec());
}
