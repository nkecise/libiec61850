#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include "CentralWidget.h"
#include "pugixml.hpp"
#include "SclParser.h"
#include "IedTreeWidget.h"
#include "hal_socket.h"
#include "iec61850_server.h"
#include "hal_thread.h"
#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"

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

signals:
	void stateChange(const QString& ctx);

private slots:
	void open();
    void save();
    void undo();
    void about();
	void mouseRightButtonReleased(const QPoint& P);
	void showPopupMenu(const QPoint& pos);
	void genCfg();
	void startSim();
	void stopSim();
	void stateHasChanged(const QString& ctx);
private:
	char szEchoStr[256];
	bool running;
	CSCLParser *sclParser;
	QString curFileName;
	QString cfgFileName;
	map<string, pugi::xml_node> mIED;
	IedServer iedServer;
	IedModel *model;

public:
	static MainWindow *pMainWnd;
	static void SclParserHandler(
			const char *file,
			unsigned int line,
			unsigned int offset,
			unsigned int err,
			const char *reason
			);
	static void SocketReadHandler(Socket S, uint8_t *buf, int size);
	static void SocketWriteHandler(Socket S,uint8_t *buf, int size);
	static void ConfigFileParseHandler(
			const char *src, unsigned int src_line,
			unsigned int cfg_line, unsigned int cfg_indendation,
			const char *line_buf, const char *reason);
	static void connectionHandler(
			IedServer self, 
			ClientConnection conn,
			bool connected,
			void *paras
			);
	static bool myActiveSettingGroupChangedHandler(
			void *parameter,
			SettingGroupControlBlock *sgcb,
			uint8_t newActSg,
			ClientConnection connection
			);
	static bool myEditSettingGroupChangedHandler(
			void *parameter,
			SettingGroupControlBlock *sgcb,
			uint8_t newEditSG,
			ClientConnection connection
			);
	static void myEditSettingGroupConfirmationHandler(
			void *parameter,
			SettingGroupControlBlock *sgcb,
			uint8_t editSg
			);
	void installSgcbHandler();
	void DoSim();

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
	QAction *startSimAct;
	QAction *stopSimAct;

	CCentralWidget *centralWidget;
	CIedTreeWidget *iedTreeWidget;
	QTreeWidgetItem *currTreeItem;

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

unsigned int __stdcall GSimThread(void *args);

#endif
