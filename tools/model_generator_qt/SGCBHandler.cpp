/*
 * File: SGCBHandler.cpp
 */
#include "MainFrame.h"

bool MainWindow::myActiveSettingGroupChangedHandler(
		void *paras, SettingGroupControlBlock *sgcb, uint8_t newActSG, ClientConnection conn
		)
{
	return(true);
}

bool MainWindow::myEditSettingGroupChangedHandler(
		void *paras, SettingGroupControlBlock *sgcb, uint8_t newEditSG, ClientConnection conn
		)
{
	return(true);
}

void MainWindow::myEditSettingGroupConfirmationHandler(
		void *paras, SettingGroupControlBlock *sgcb, uint8_t editSG
		)
{
}

void MainWindow::installSgcbHandler()
{
	if(model->firstChild == NULL)
		return;
	LogicalDevice *ld = model->firstChild;
	while(ld->sibling != NULL)
	{
		SettingGroupControlBlock *sg = 
			LogicalDevice_getSettingGroupControlBlock(ld);
		if(sg)
		{
			IedServer_setActiveSettingGroupChangedHandler(
					iedServer, sg, myActiveSettingGroupChangedHandler, NULL);
			IedServer_setEditSettingGroupChangedHandler(
					iedServer, sg, myEditSettingGroupChangedHandler, NULL);
			IedServer_setEditSettingGroupConfirmationHandler(
					iedServer, sg, myEditSettingGroupConfirmationHandler, NULL);
		}
		ld = (LogicalDevice *)ld->sibling;
	}
}
