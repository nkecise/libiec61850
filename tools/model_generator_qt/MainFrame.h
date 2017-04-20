/*
 * File:	MainFrame.h
 */
#ifndef	MAIN_FRAME_H
#define	MAIN_FRAME_H

#include <windows.h>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include ".\ui\uic_MainFrame.h"
#include "SclParser.h"

class MainFrame : public QDialog
{
	Q_OBJECT
	public:
		MainFrame(QWidget *parent = NULL);
		~MainFrame();
	public slots:
		void selectSclFile();
		void selectOutFile();
		void generate();
		void currentIndexHasChanged(const QString& text);
	private:
		Ui::MainFrame	ui;
		CSCLParser *sclParser;
		map<string, pugi::xml_node> mIED;
		map<string, map<string, vector<string>>> mSubNet;
};

#endif	//MAIN_FRAME_H
