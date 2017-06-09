/*
 * File: FileSvcHandler.cpp
 */
#include "MainFrame.h"

MmsError MainWindow::myMmsFileAccessHandler(
		void *parameter,
		MmsServerConnection connection,
		MmsFileServiceType type,
		const char *localFileName,
		const char *otherFileName
		)
{
	return(MMS_ERROR_NONE);
}

void MainWindow::installFileServiceHandler()
{
	MmsServer mmsServer = IedServer_getMmsServer(iedServer);

    MmsServer_setFilestoreBasepath(mmsServer, "./");
	MmsServer_installFileAccessHandler(mmsServer, myMmsFileAccessHandler, NULL);
}
