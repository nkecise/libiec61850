/*
 * File: 61850Handler.cpp
 */
#include <pthread.h>
#include <QMessageBox>
#include <QDir>
#include "MainFrame.h"

Socket_readHandler pSocket_readHandler = MainWindow::SocketReadHandler;
Socket_writeHandler pSocket_writeHandler;
ConfigFileParser_parseHandler pConfigFileParser_parseHandler = MainWindow::ConfigFileParseHandler;
SclParserHandler pSclParserHandler = MainWindow::SclParserHandler;

void MainWindow::SclParserHandler(
		const char *file, /* file of caller */
		unsigned int line, /* line of caller */
		unsigned int offset, /* offset of error occur */
		unsigned int err, /* error code */
		const char *reason /* textural description */
		)
{
	QString str = QString("(")
				+ QString(file) + "," + QString::number(line)
				+ QString("), OFFSET:")
				+ QString::number(offset) + QString(", REASON: ")
				+ QString(reason);
	emit pMainWnd->stateChange(str);
}

void MainWindow::SocketReadHandler(Socket S, uint8_t *buf, int size)
{
}

void MainWindow::SocketWriteHandler(Socket S, uint8_t *buf, int size)
{
}

void MainWindow::ConfigFileParseHandler(
		const char *src_file,
		unsigned int src_line,
		unsigned int cfg_line,
		unsigned int cfg_ind,
		const char *line_buf,
		const char *reason
		)
{
	QString str = QString(src_file)
				+ QString(",")
				+ QString::number(src_line)
				+ QString(" ")
				+ QString::number(cfg_line)
				+ QString(",")
				+ QString(line_buf)
				+ QString(",")
				+ QString(reason);
	emit pMainWnd->stateChange(str);
}

void MainWindow::startSim()
{
	QString apName = currTreeItem->text(0);
	QString iedName= currTreeItem->parent()->text(0);
	cfgFileName = QDir::tempPath() + "/" + iedName + "_" + apName + ".cfg";
	QFile file(cfgFileName);
	if(! file.exists())
	{
		QMessageBox::information(this, tr("Information"),
				tr("Call 'Generate Configuration' first!"));
		return;
	}
	// terminate old simulation process
	if(iedServer)
	{
		if(IedServer_isRunning(iedServer))
		{
			running = false;
			Thread_sleep(1000);
		}
	}
	// start new simulation process
    pthread_attr_t attr;
    pthread_t pthrd;
    pthread_attr_init(&attr);
    pthread_create(&pthrd, &attr, &GSimThread, this);

	return;
}

void MainWindow::stopSim()
{
	running = false;
}

void MainWindow::DoSim()
{
	FileHandle configFile = 
		FileSystem_openFile((char *)cfgFileName.toStdString().c_str(), false);
	if(configFile == NULL)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "FileSystem_openFile '%s' failed!", 
				cfgFileName.toStdString().c_str());
		emit pMainWnd->stateChange(QString(szEchoStr));
		return;
	}
	//
	model = ConfigFileParser_createModelFromConfigFile(configFile);
	if(model == NULL)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "ConfigFileParser_createModelFromConfigFile Failed");
		emit pMainWnd->stateChange(QString(szEchoStr));
		return;
	}
	//
	iedServer = IedServer_create(model);
	IedServer_setConnectionIndicationHandler(
			iedServer, 
			(IedConnectionIndicationHandler)connectionHandler,
			NULL);
    installSgcbHandler();
    installFileServiceHandler();
	IedServer_start(iedServer, 102);
	if(! IedServer_isRunning(iedServer))
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "IedServer start failed.");
		IedServer_destroy(iedServer);
		emit pMainWnd->stateChange(QString(szEchoStr));
		return;
	}
	int i = 1;
	running = true;
	emit pMainWnd->stateChange(QString("Server is RUNNING"));
	while(running)
	{
        Thread_sleep(15000);
        //writeRequest(i++);
	}
	IedServer_stop(iedServer);
	IedServer_destroy(iedServer);
	IedModel_destroy(model);
	iedServer = NULL;
	model = NULL;
	emit pMainWnd->stateChange(QString("Server is Destroyed"));
}

void MainWindow::writeRequest(int i)
{
	char szObjRef[256];

    strcpy(szObjRef, "PRS702LD0/STMP1.TmpSv01.mag.f");
    DataAttribute *datAttr1 = (DataAttribute *)
		IedModel_getModelNodeByObjectReference(model, szObjRef);
    DataAttribute *datAttr2 = (DataAttribute *)
            IedModel_getModelNodeByObjectReference(model, "PRS702LD0/STMP4.TmpSv10.mag.f");
    DataAttribute *datAttr3 = (DataAttribute *)
            IedModel_getModelNodeByObjectReference(model, "PRS702LD0/SCLI2.RPowerSv06.mag.f");
    if(!datAttr1 || !datAttr2 || !datAttr3)
        return;
    MmsValue *val1 = MmsValue_newFloat(static_cast<float>(rand())/static_cast<float>(RAND_MAX));
    MmsValue *val2 = MmsValue_newFloat(static_cast<float>(rand())/static_cast<float>(RAND_MAX));
    MmsValue *val3 = MmsValue_newFloat(static_cast<float>(rand())/static_cast<float>(RAND_MAX));

	IedServer_lockDataModel(iedServer);
    IedServer_updateAttributeValue(iedServer, datAttr1, val1);
    IedServer_updateAttributeValue(iedServer, datAttr2, val2);
    IedServer_updateAttributeValue(iedServer, datAttr3, val3);
	IedServer_unlockDataModel(iedServer);
    MmsValue_delete(val1);
    MmsValue_delete(val2);
    MmsValue_delete(val3);
}

void MainWindow::connectionHandler(
		IedServer self,
		ClientConnection conn,
		bool connected,
		void *paras
		)
{
	char szPeerAddr[128];
	char szEchoStr[128];

	strcpy(szPeerAddr, ClientConnection_getPeerAddress(conn));
	memset(szEchoStr, 0x00, sizeof(szEchoStr));
	if(connected)
		sprintf(szEchoStr, "Connected: %s", szPeerAddr);
	else
		sprintf(szEchoStr, "Disconnected: %s", szPeerAddr);
	emit pMainWnd->stateChange(QString(szEchoStr));
}

void *GSimThread(void *args)
{
	MainWindow *thiz = (MainWindow *)args;
	thiz->DoSim();

	return(0);
}
