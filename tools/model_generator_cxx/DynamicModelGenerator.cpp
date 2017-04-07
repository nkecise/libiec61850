/*
 * File: DynamicModelGenerator.cpp
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DynamicModelGenerator.h"
#include "SclParser.h"

int GenerateDynamicConfig(
		const char *pSclName, const char *pIedName,
		const char *pApName, std::string& ctx)
{
	CSCLParser *pSCLParser = new CSCLParser();

	pSCLParser->SetSCLName(pSclName);
	pSCLParser->SetAPName(pApName);
	pSCLParser->SetIEDName(pIedName);
	if(pSCLParser->LoadFile())
		return(ERROR_LOAD_FILE);
	if(pSCLParser->ParseFile())
		return(ERROR_PARSE_FILE);
	pSCLParser->UnloadFile();
	pSCLParser->GetCfgCtx(ctx);

	delete pSCLParser;

	return(0);
}

int IsScl(const char *fn)
{
	char szFileName[256];

	strcpy(szFileName, fn);
	if(strcasestr(szFileName, "ICD")
		|| strcasestr(szFileName, "CID")
		|| strcasestr(szFileName, "SCD"))
		return(1);
	else
		return(0);
}


int main(int argc, char *argv[])
{
	char szSclName[256], szIedName[64];
	char szApName[16], szOutName[256];
	
	if(argc < 2)
	{
		fprintf(stderr, "Usage: DynamicModelGenerator <ICD File> [-ied <ied-name>] [-ap <access-point-name>] [<output filename]\n");
		return(1);
	}
	strcpy(szSclName, argv[1]);
	if(!strlen(szSclName) || !IsScl(szSclName))
	{
		fprintf(stderr, "Usage: DynamicModelGenerator <ICD File> [-ied <ied-name>] [-ap <access-point-name>] [<output filename]\n");
		return(1);
	}
	//
	memset(szIedName, 0x00, sizeof(szIedName));
	memset(szApName, 0x00, sizeof(szApName));
	memset(szOutName, 0x00, sizeof(szOutName));
	for(int i = 2; i < argc; i+=2)
	{
		if(! strcasecmp(argv[i], "-ied"))
		{
			if(argv[i+1] != NULL)
				strncpy(szIedName, argv[i+1], sizeof(szIedName)-1);
		}
		if(! strcasecmp(argv[i], "-ap"))
		{
			if(argv[i+1] != NULL)
				strncpy(szApName, argv[i+1], sizeof(szApName)-1);
		}
		if(! strcasecmp(argv[i], "output"))
		{
			if(argv[i+1] != NULL)
				strncpy(szOutName, argv[i+1], sizeof(szOutName)-1);
		}
	}
	//
	std::string ctx;
	GenerateDynamicConfig(szSclName, szIedName, szApName, ctx);

	return(0);
}
