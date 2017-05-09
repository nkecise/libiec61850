/*
 * File: MainFrame.cpp
 */
#include <QtGui>
#include <QCursor>
#include "MainFrame.h"

MainWindow *MainWindow::pMainWnd = NULL;

MainWindow::MainWindow()
{
	running = false;
	pMainWnd = this;
	iedServer = NULL;
	model = NULL;
	sclParser = new CSCLParser();
	centralWidget = new CCentralWidget(this);
    setCentralWidget(centralWidget);

    createActions();
    createMenus();
    //createToolBars();
    createStatusBar();
    createDockWindows();
	createPopupMenuEx();

    setWindowTitle(tr("GenConfig"));
    setUnifiedTitleAndToolBarOnMac(true);
	showMaximized();

	connect(iedTreeWidget,
			SIGNAL(mouseRightButtonRelease(const QPoint&)),
			this, 
			SLOT(mouseRightButtonReleased(const QPoint &))
			);
	connect(iedTreeWidget, 
			SIGNAL(customContextMenuRequested(const QPoint &)), 
			this, SLOT(showPopupMenu(const QPoint &))
			);
	connect(this, SIGNAL(stateChange(const QString&)), this,
			SLOT(stateHasChanged(const QString&)));
}

void MainWindow::stateHasChanged(const QString& ctx)
{
	QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString str = time + "\t" + ctx;
	centralWidget->appendStateString(str);
}

void MainWindow::mouseRightButtonReleased(const QPoint& P)
{
	QTreeWidgetItem *item = (QTreeWidgetItem *)iedTreeWidget->itemAt(P);
	QTreeWidgetItem *root = item->parent();
	if(root == NULL)
		return;
}

void MainWindow::open()
{
	curFileName = QFileDialog::getOpenFileName(
			this, tr("Select SCL"), NULL,
			"SCL Files(*.scd *.icd *.cid);;"
			"All Files(*.*)"
			);
	if(curFileName.isEmpty())
		return;
	//
	QFile file(curFileName);
	if(! file.open(QIODevice::ReadOnly))
	{
		QString str = QString("Cannot open file '") + curFileName + QString("'");
		emit stateChange(str);
		return;
	}
	//
	sclParser->SetSCLName(curFileName.toUtf8().data());
	sclParser->LoadFileEx(file.readAll().data(), file.size());
	// initialize IED ComboBox
	sclParser->GetIEDList(mIED);
	clearIedTreeWidget();
	for(map<string, pugi::xml_node>::iterator it = mIED.begin();
			it != mIED.end(); it++)
	{
		pugi::xml_node xnIED = it->second;
		QString ctx = QString(xnIED.attribute("name").value())
					+ "-"
					+ QString::fromUtf8(xnIED.attribute("desc").value());
		QTreeWidgetItem *root = new QTreeWidgetItem();
		root->setText(0, xnIED.attribute("name").value());
		iedTreeWidget->addTopLevelItem(root);
		for(pugi::xml_node xnAP = xnIED.child("AccessPoint"); xnAP;
				xnAP = xnAP.next_sibling("AccessPoint"))
		{
			QTreeWidgetItem *child = new QTreeWidgetItem();
			ctx = QString(xnAP.attribute("name").value())
				+ "("
				+ sclParser->GetSubNetType(
						xnIED.attribute("name").value(),
						xnAP.attribute("name").value(),
						szEchoStr)
				+ ")";
			child->setText(0, xnAP.attribute("name").value());
			root->addChild(child);
		}
	}
}

void MainWindow::genCfg()
{
	QString apName = currTreeItem->text(0);
	QString iedName= currTreeItem->parent()->text(0);
	cfgFileName = 
		QDir::tempPath() + "/" + iedName + "_" + apName + ".cfg";
	//
	std::string ctx;
	sclParser->SetAPName(apName.toStdString().c_str());
	sclParser->SetIEDName(iedName.toStdString().c_str());
	sclParser->ParseFile();	
	sclParser->GetCfgCtx(ctx);
	//
	QFile file(cfgFileName);
	if(file.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		file.write(ctx.c_str(), ctx.size());
		file.close();
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "Configuration was written to: '%s'",
				cfgFileName.toStdString().c_str());
		emit stateChange(QString(szEchoStr));
	}
	else
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "open '%s' failed", cfgFileName.toStdString().c_str());
		QMessageBox::critical(this, tr("ERROR"), szEchoStr);	
	}
}

void MainWindow::save()
{
}

void MainWindow::undo()
{
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open..."));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	genCfgAct = new QAction(tr("Generate Configuration"), this);
	connect(genCfgAct, SIGNAL(triggered()), this, SLOT(genCfg()));

	startSimAct = new QAction("Start Simulation", this);
	connect(startSimAct, SIGNAL(triggered()), this, SLOT(startSim()));

	stopSimAct = new QAction("Stop Simulation", this);
	connect(stopSimAct, SIGNAL(triggered()), this, SLOT(stopSim()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
	/*
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
	*/
    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
	/*
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
	*/
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("IED"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    iedTreeWidget = new CIedTreeWidget(dock);
	iedTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    dock->setWidget(iedTreeWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::clearIedTreeWidget()
{
	for(int i = iedTreeWidget->topLevelItemCount()-1; i >= 0; i--)
		iedTreeWidget->takeTopLevelItem(i);
}

void MainWindow::createPopupMenuEx()
{
	popMenu = new QMenu(iedTreeWidget);
	popMenu->addAction(genCfgAct);
	popMenu->addAction(startSimAct);
	popMenu->addAction(stopSimAct);
}

void MainWindow::showPopupMenu(const QPoint& pos)
{
	currTreeItem = (QTreeWidgetItem *)iedTreeWidget->itemAt(pos);
	if(currTreeItem == NULL)
		return;
	QTreeWidgetItem *root = currTreeItem->parent();
	if(root == NULL)
		return;
	popMenu->clear();
	popMenu->addAction(genCfgAct);
	popMenu->addAction(startSimAct);
	popMenu->exec(QCursor::pos());
}

MainWindow::~MainWindow()
{
	delete sclParser;
}
