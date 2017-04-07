/*
 *  server_example_config_file.c
 *
 *  This example shows how to use dynamic server data model with a configuration file.
 *
 *  - How to open and parse the model configuration file
 *  - How to access data attributes by object reference strings
 *  - How to access data attributes by short addresses
 *
 *  Note: If building with cmake the vmd-filestore folder containing the configuration file
 *  (model.cfg) has to be copied to the folder where the example is executed!
 *  The configuration file can be created from the SCL(ICD) file with the Java tool genconfig.jar
 *  that is included in the source distribution of libiec61580.
 *
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"

static int running = 0;

void sigint_handler(int signalId)
{
	running = 0;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    /* open configuration file */
	CHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	PathRemoveFileSpec(szFileName);
	strcat(szFileName, "\\BYE550M.cfg");
    FileHandle configFile = FileSystem_openFile(szFileName, false);

    if (configFile == NULL) {
        printf("Error opening config file!\n");
        return 1;
    }

    /* parse the configuration file and create the data model */
    IedModel* model = ConfigFileParser_createModelFromConfigFile(configFile);

    FileSystem_closeFile(configFile);

    if (model == NULL) {
        printf("Error parsing config file!\n");
        return 1;
    }

	IedServer iedServer = IedServer_create(model);

	IedServer_start(iedServer, tcpPort);

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}

	running = 1;

	while (running) {

		Thread_sleep(100);
	}

	IedServer_stop(iedServer);

	IedServer_destroy(iedServer);

	IedModel_destroy(model);
} /* main() */
