/*
 * File: SclValidate.cpp
 */
#include <string.h>
#include "Error.h"
#include "SclParser.h"

int CSCLParser::CheckDocSCL()
{
	int cnt = 0;
	pugi::xml_node xnSCL = doc.select_single_node("/SCL").node();

	for( ; xnSCL; xnSCL = xnSCL.next_sibling("SCL"))
		cnt++;
	if(cnt == 0)
		return(ERROR_MISSING_SCL);
	if(cnt > 1)
		return(ERROR_SCL_REPEATED);
	//
	pugi::xml_node xnComm = doc.select_single_node("/SCL/Communication").node();
	if(! xnComm)
		return(ERROR_MISSING_COMMUNICATION);
	pugi::xml_node xnSubNet = xnComm.child("SubNetwork");
	if(! xnSubNet)
		return(ERROR_MISSING_SUBNETWORKS);
	pugi::xml_node xnTemp = doc.select_single_node("/SCL/DataTypeTemplates").node();
	if(! xnTemp)
		return(ERROR_MISSING_TEMPLATE);	

	return(ERROR_OK);
}

int CSCLParser::CheckRequiredAttrs(const pugi::xml_node xnNode, int type, string& str)
{
	char attrs[256], *pos = NULL;
	nRetCode = ERROR_OK;

	strcpy(attrs, chk_attr[type]);
	pos = strtok(attrs, ",");
	while(pos)
	{
		if(! xnNode.attribute(pos))
		{
		}
	}

	return(nRetCode);
}
