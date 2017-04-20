/*
 * File:	MainFrame.cpp
 */
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <map>
#include "MainFrame.h"

using namespace std;

MainFrame::MainFrame(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
	sclParser = new CSCLParser();
	//
	connect(ui.choseSclPushButton, SIGNAL(clicked()), this,
			SLOT(selectSclFile()));
	connect(ui.choseCfgPushButton, SIGNAL(clicked()), this,
			SLOT(selectOutFile()));
	connect(ui.generatePushButton, SIGNAL(clicked()), this,
			SLOT(generate()));
	connect(ui.iedComboBox, SIGNAL(currentIndexChanged(const QString&)),
			this, SLOT(currentIndexHasChanged(const QString&)));
	setWindowTitle(tr("GenConfig"));
}

void MainFrame::selectSclFile()
{
	QString curFileName = QFileDialog::getOpenFileName(
			this, tr("Select SCL"), NULL,
			"SCL Files(*.scd *.icd *.cid);;"
			"All Files(*.*)"
			);
	if(curFileName.isEmpty())
		return;
	curFileName.replace("/", "\\");
	(ui.sclLineEdit)->setText(curFileName);
	//
	sclParser->SetSCLName(curFileName.toStdString().c_str());
#ifdef _WIN32
	if(sclParser->LoadFileWind())
#elif __linux__
	if(sclParser->LoadFileUnix())
#endif
	{
		QMessageBox::critical(this, tr("Error"), tr("Failed to load SCL File"));
		return;
	}
	// initialize IED ComboBox
	sclParser->GetIEDList(mIED);
	for(map<string, pugi::xml_node>::iterator it = mIED.begin();
			it != mIED.end(); it++)
	{
		(ui.iedComboBox)->addItem(it->second.attribute("name").value());
	}
}

void MainFrame::selectOutFile()
{
	char szFileName[256];
#ifdef _WIN32
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	PathRemoveFileSpec(szFileName);
#elif __linux__
#endif
	strcat(szFileName, "\\");
	strcat(szFileName, (ui.iedComboBox)->currentText().toStdString().c_str());
	QString curFile = QFileDialog::getSaveFileName(
			this, tr("Output File"), szFileName, "config files(*.cfg);;");
	if(! curFile.isEmpty())
	{
		curFile.replace("/", "\\");
		(ui.outLineEdit)->setText(curFile);
	}
}

void MainFrame::generate()
{
	if((ui.sclLineEdit)->text().isEmpty()
	  || (ui.outLineEdit)->text().isEmpty())
	{
		QMessageBox::critical(this, tr("ERROR"), 
				tr("Select SCL OR Output File First!"));
		return;
	}
	//
	string ctx;
	sclParser->SetAPName((ui.apComboBox)->currentText().toStdString().c_str());
	sclParser->SetIEDName((ui.iedComboBox)->currentText().toStdString().c_str());
	sclParser->ParseFile();
	sclParser->GetCfgCtx(ctx);
	//
	FILE *fp = fopen((ui.outLineEdit)->text().toStdString().c_str(), "w");
	if(fp)
	{
		fwrite(ctx.c_str(), 1, ctx.size(), fp);
		fclose(fp);
	}
	QString echoStr = QString("Configuration was written to '")
					+ (ui.outLineEdit)->text()
					+ "'";
	QMessageBox::information(this, tr("Information"), echoStr);
}

void MainFrame::currentIndexHasChanged(const QString& text)
{
	pugi::xml_node xnIED;

	(ui.apComboBox)->clear();
	if(mIED.find(text.toStdString()) == mIED.end())
		return;
	xnIED = mIED.find(text.toStdString())->second;
	for(pugi::xml_node xnAP = xnIED.child("AccessPoint"); xnAP;
			xnAP = xnAP.next_sibling("AccessPoint"))
	{
		(ui.apComboBox)->addItem(xnAP.attribute("name").value());
	}
}

MainFrame::~MainFrame()
{
	delete sclParser;
}
