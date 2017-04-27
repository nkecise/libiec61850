/*
 * File: MainFrame.cpp
 */
#include <QtGui>
#include <QCursor>
#include "MainFrame.h"

MainWindow::MainWindow()
{
	sclParser = new CSCLParser();
	msgListWidget = new QListWidget();
    setCentralWidget(msgListWidget);

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
	curFileName.replace("/", "\\");
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
	clearIedTreeWidget();
	for(map<string, pugi::xml_node>::iterator it = mIED.begin();
			it != mIED.end(); it++)
	{
		pugi::xml_node xnIED = it->second;
		QTreeWidgetItem *root = new QTreeWidgetItem();
		root->setText(0, xnIED.attribute("name").value());
		iedTreeWidget->addTopLevelItem(root);
		for(pugi::xml_node xnAP = xnIED.child("AccessPoint"); xnAP;
				xnAP = xnAP.next_sibling("AccessPoint"))
		{
			QTreeWidgetItem *child = new QTreeWidgetItem();
			child->setText(0, xnAP.attribute("name").value());
			root->addChild(child);
		}
	}
}

void MainWindow::genCfg()
{
	QMessageBox::information(this, NULL, "Generate Configuration");
}

void MainWindow::simulate()
{
	QMessageBox::information(this, NULL, "Simulation");
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

	simAct = new QAction("Simulate", this);
	connect(simAct, SIGNAL(triggered()), this, SLOT(simulate()));
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
	popMenu->addAction(simAct);
}

void MainWindow::showPopupMenu(const QPoint& pos)
{
	QTreeWidgetItem *item = (QTreeWidgetItem *)iedTreeWidget->itemAt(pos);
	if(item == NULL)
		return;
	QTreeWidgetItem *root = item->parent();
	if(root == NULL)
		return;
	//
	popMenu->clear();
	popMenu->addAction(genCfgAct);
	popMenu->addAction(simAct);
	popMenu->exec(QCursor::pos());
}

MainWindow::~MainWindow()
{
	delete sclParser;
}
