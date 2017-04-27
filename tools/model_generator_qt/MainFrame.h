#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include "pugixml.hpp"
#include "SclParser.h"
#include "IedTreeWidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTreeWidget;
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
	void open();
    void save();
    void undo();
    void about();
	void mouseRightButtonReleased(const QPoint& P);
	void showPopupMenu(const QPoint& pos);
	void genCfg();
	void simulate();
private:
	CSCLParser *sclParser;
	QString curFileName;
	map<string, pugi::xml_node> mIED;

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
	void clearIedTreeWidget();
	void createPopupMenuEx();

	QMenu *popMenu;
	QAction *genCfgAct;
	QAction *simAct;

	QListWidget *msgListWidget;
	CIedTreeWidget *iedTreeWidget;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
	QAction *openAct;
    QAction *saveAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
};

#endif
