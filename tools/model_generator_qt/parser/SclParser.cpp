/*
 * File: $(model_generator_cxx)/SclParser.cpp
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SclParser.h"
#include "Error.h"
#include "AttributeType.h"

CSCLParser::CSCLParser()
{
}

void CSCLParser::SetSCLName(const char *scl)
{
	sclName = std::string(scl);
}

void CSCLParser::SetAPName(const char *ap)
{
	apName = std::string(ap);
}

void CSCLParser::SetIEDName(const char *ied)
{
	iedName = std::string(ied);
}

#ifdef __linux__
int CSCLParser::LoadFileUnix()
{
	int fd = 0;
	char *xml = NULL;
	struct stat st;

	fd = open(sclName.c_str(), O_RDWR);
	if(fd == -1)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "open '%s' failed.\n", sclName.c_str());
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, errno, szEchoStr);
		return(errno);
	}
	if(fstat(fd, &st) == -1)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "fstat FAILED. ");
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, errno, szEchoStr);
		return(errno);
	xml = (char *)mmap(NULL, st.st_size + 1, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(xml == MAP_FAILED)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "mmap FAILED.");
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, errno, szEchoStr);
		close(fd);
		return(errno);
	}
	xml[st.st_size] = '\0';
	close(fd);
	//
	doc.load_buffer(xml, st.st_size);
	munmap(xml, st.st_size);
	//
	if(nRetCode = CheckDocSCL())
	{
		switch(nRetCode)
		{
			case ERROR_MISSING_SCL:
				sprintf(szEchoStr, "Document has no SCL section defined");
				break;
			case ERROR_SCL_REPEATED:
				sprintf(szEchoStr, "Document has more than one SCL section found"); 
				break;
			case ERROR_MISSING_TEMPLATE:
				sprintf(szEchoStr, "Document has no DataTypeTemplates seciton defined");
				break;
			case ERROR_MISSING_COMMUNICATION:
				sprintf(szEchoStr, "Document has no Communication section defined");
				break;
			case ERROR_MISSING_SUBNETWORKS:
				sprintf(szEchoStr, "No SubNetworks defined under Communication section");
				break;
		}
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		return(nRetCode);
	}
	if(nRetCode = ParseDataTypeTemplates())
		return(nRetCode);
	if(nRetCode = ParseIEDSection())
		return(nRetCode);
	if(nRetCode = ParseCommunicationSection())
		return(nRetCode);

	return(ERROR_OK);
}
#elif _WIN32
int CSCLParser::LoadFileWind()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	//
	hFile = CreateFile(
			sclName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
			);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nRetCode = GetLastError();
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "CreateFile Failed: %s", sclName.c_str());
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		return(nRetCode);
	}
	//
	DWORD nFileSizeLow, nFileSizeHigh;
	HANDLE hFileMap = NULL;
	LPCSTR pXml = NULL;
	nFileSizeLow = GetFileSize(hFile, &nFileSizeHigh);
	hFileMap = CreateFileMapping(
			hFile, NULL, PAGE_READONLY, 0, nFileSizeLow, NULL
			);
	if(hFileMap == NULL)
	{
		nRetCode = GetLastError();
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "CreateFileMapping Failed: %s", sclName.c_str());
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		CloseHandle(hFile);
		return(nRetCode);
	}
	pXml = (LPSTR)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, nFileSizeLow);
	if(!pXml)
	{
		nRetCode = GetLastError();
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, "MapViewOfFile Failed: %s", sclName.c_str());
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		return(nRetCode);
	}
	//
	doc.load_buffer(pXml, nFileSizeLow);
	if(nRetCode = CheckDocSCL())
	{
		switch(nRetCode)
		{
			case ERROR_MISSING_SCL:
				sprintf(szEchoStr, "Document has no SCL section defined");
				break;
			case ERROR_SCL_REPEATED:
				sprintf(szEchoStr, "Document has more than one SCL section found"); 
				break;
			case ERROR_MISSING_TEMPLATE:
				sprintf(szEchoStr, "Document has no DataTypeTemplates seciton defined");
				break;
			case ERROR_MISSING_COMMUNICATION:
				sprintf(szEchoStr, "Document has no Communication section defined");
				break;
			case ERROR_MISSING_SUBNETWORKS:
				sprintf(szEchoStr, "No SubNetworks defined under Communication section");
				break;
		}
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		return(nRetCode);
	}
	if(nRetCode = ParseDataTypeTemplates())
		return(nRetCode);
	if(nRetCode = ParseIEDSection())
		return(nRetCode);
	if(nRetCode = ParseCommunicationSection())
		return(nRetCode);
	//
	CloseHandle(hFile);
	CloseHandle(hFileMap);
	UnmapViewOfFile(pXml);

	return(0);
}
#endif

int CSCLParser::LoadFileEx(const char *ctx, unsigned int cbSize)
{
	doc.load_buffer(ctx, cbSize);
	//
	if(nRetCode = CheckDocSCL())
	{
		switch(nRetCode)
		{
			case ERROR_MISSING_SCL:
				sprintf(szEchoStr, "Document has no SCL section defined");
				break;
			case ERROR_SCL_REPEATED:
				sprintf(szEchoStr, "Document has more than one SCL section found"); 
				break;
			case ERROR_MISSING_TEMPLATE:
				sprintf(szEchoStr, "Document has no DataTypeTemplates seciton defined");
				break;
			case ERROR_MISSING_COMMUNICATION:
				sprintf(szEchoStr, "Document has no Communication section defined");
				break;
			case ERROR_MISSING_SUBNETWORKS:
				sprintf(szEchoStr, "No SubNetworks defined under Communication section");
				break;
		}
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, nRetCode, szEchoStr);
		return(nRetCode);
	}
	if(nRetCode = ParseDataTypeTemplates())
		return(nRetCode);
	if(nRetCode = ParseIEDSection())
		return(nRetCode);
	if(nRetCode = ParseCommunicationSection())
		return(nRetCode);

	return(0);
}

int CSCLParser::ParseFile()
{
	pugi::xml_node xnIED;
	char xpath[256];

	ctx.clear();

	if(iedName.size())
	{
		memset(xpath, 0x00, sizeof(xpath));
		sprintf(xpath, "/SCL/IED[@name='%s']", iedName.c_str());
		xnIED = doc.select_single_node(xpath).node();
	}
	else
		xnIED = doc.select_single_node("/SCL/IED").node();
	if(!xnIED)
	{
		memset(szEchoStr, 0x00, sizeof(szEchoStr));
		sprintf(szEchoStr, 
				"The specified IED '%s' is non-exist in the SCL file",
				iedName.c_str()
				);
		if(pSclParserHandler)
			pSclParserHandler(__FILE__, __LINE__, 0, ERROR_MISSING_IED, szEchoStr);
		return(ERROR_MISSING_IED);
	}
	iedName = string(xnIED.attribute("name").value());
	ParseIED(xnIED);

	return(0);
}

int CSCLParser::ParseDataTypeTemplates()
{
	pugi::xml_node xnRoot;
	const char *val = NULL;
	nRetCode = ERROR_OK;

	mLNType.clear();
	mDOType.clear();
	mDAType.clear();
	mEnumType.clear();

	xnRoot = doc.select_single_node("/SCL/DataTypeTemplates").node();
	pugi::xml_node xnNode = xnRoot.child("LNodeType");
	do
	{
		if(xnNode.attribute("id"))
		{
			val = xnNode.attribute("id").value();
			mLNType.insert(std::make_pair(val, xnNode));
		}
		else
		{
		}
	}while(xnNode = xnNode.next_sibling("LNodeType"));
	//
	xnNode = xnRoot.child("DOType");
	do
	{
		if(xnNode.attribute("id"))
		{
			val = xnNode.attribute("id").value();
			mDOType.insert(std::make_pair(val, xnNode));
		}
		else
		{
		}
	}while(xnNode = xnNode.next_sibling("DOType"));
	//
	xnNode = xnRoot.child("DAType");
	do
	{
		if(xnNode.attribute("id"))
		{
			val = xnNode.attribute("id").value();
			mDAType.insert(std::make_pair(val, xnNode));
		}
		else
		{
		}
	}while(xnNode = xnNode.next_sibling("DAType"));
	//
	xnNode = xnRoot.child("EnumType");
	do
	{
		if(xnNode.attribute("id"))
		{
			val = xnNode.attribute("id").value();
			mEnumType.insert(std::make_pair(val, xnNode));
		}
		else
		{
		}
	}while(xnNode = xnNode.next_sibling("EnumType"));

EXIT:

	return(nRetCode);
}

int CSCLParser::ParseIED(const pugi::xml_node& xnIED)
{
	ctx += std::string("MODEL(") 
		+ xnIED.attribute("name").value() 
		+ "){\n";
	//
	pugi::xml_node xnAP;
	bool b = false;
	if(apName.size())
	{
		for(xnAP = xnIED.child("AccessPoint"); xnAP;
				xnAP = xnAP.next_sibling())
		{
			printf("%s\n", xnAP.attribute("name").value());
			if(!strcmp(xnAP.attribute("name").value(), apName.c_str()))
			{
				b = true;
				break;
			}
		}
	}
	else
		xnAP = xnIED.child("AccessPoint");
	if(!xnAP || !b)
		return(ERROR_MISSING_AP);
	apName = xnAP.attribute("name").value();
	//
	xpath = "/SCL/Communication/SubNetwork/ConnectedAP[@iedName='"
		  + iedName + "' and @apName='"
		  + apName + "']";
	xnConnectedAP = doc.select_nodes(xpath.c_str());
	ParseAccessPoint(xnAP);
	ctx += "}";

	return(0);
}

int CSCLParser::ParseAccessPoint(const pugi::xml_node& xnAP)
{
	pugi::xml_node xnLDevice = xnAP.child("Server").child("LDevice");

	for(; xnLDevice; xnLDevice = xnLDevice.next_sibling("LDevice"))
	{
		ctx += std::string("LD(") + xnLDevice.attribute("inst").value() + "){\n";
		ParseLDevice(xnLDevice);
		ctx += "}\n";
	}

	return(0);
}

int CSCLParser::ParseLDevice(const pugi::xml_node& xnLDevice)
{
	pugi::xml_node xnLN0 = xnLDevice.child("LN0");
	if(xnLN0)
	{
		ctx += std::string("LN(LLN0){\n");
		ParseLN(xnLN0, true);
		ctx += "}\n";
	}
	//
	pugi::xml_node xnLN = xnLDevice.child("LN");
	for(; xnLN; xnLN = xnLN.next_sibling("LN"))
	{
		ctx += std::string("LN(") 
			+ xnLN.attribute("prefix").value()
			+ xnLN.attribute("lnClass").value() 
			+ xnLN.attribute("inst").value()
			+ "){\n";
		ParseLN(xnLN, false);
		ctx += "}\n";
	}

	return(0);
}

int CSCLParser::ParseLN(const pugi::xml_node& xnLN, bool bLN0)
{
	bool pushed = false;
	pugi::xml_node xnSGCB = xnLN.child("SettingControl");
	if(xnSGCB)
		ParseSGCB(xnSGCB);
	/* DO */
	char *lnType = (char *)xnLN.attribute("lnType").value();
	if(mLNType.find(lnType) == mLNType.end())
		return(ERROR_MISSING_LNTYPE);
	pugi::xml_node xnLNType = mLNType.find(lnType)->second;
	for(pugi::xml_node xnDO = xnLNType.child("DO"); 
			xnDO; xnDO = xnDO.next_sibling("DO"))
	{
		pugi::xml_node xnDOI = xnLN.find_child_by_attribute(
				"DOI", "name", xnDO.attribute("name").value());
		if(xnDOI)
		{
			stkDOI.push(xnDOI);
			pushed = true;
		}
		else
			pushed = false;
		//
		ctx += std::string("DO(")
			+ xnDO.attribute("name").value() + " "
			+ string(xnDO.attribute("count") ? xnDO.attribute("count").value() : "0")
			+ "){\n";
		ParseDO(xnDO, false);
		ctx += "}\n";
		//
		if(pushed)
		{
			stkDOI.pop();
		}
	}
	/* DataSet */
	for(pugi::xml_node xnDataSet = xnLN.child("DataSet"); xnDataSet;
			xnDataSet = xnDataSet.next_sibling("DataSet"))
	{
		ctx += string("DS(")
			+ xnDataSet.attribute("name").value()
			+ "){\n";
		ParseDataSet(xnDataSet);
		ctx += "}\n";
	}
	/* ReportControlBlock */
	for(pugi::xml_node xnRptCtrl = xnLN.child("ReportControl"); xnRptCtrl;
			xnRptCtrl = xnRptCtrl.next_sibling("ReportControl"))
	{
		ParseRptCtrl(xnRptCtrl);
	}
	/* LogControlBlock */
	for(pugi::xml_node xnLogCtrl = xnLN.child("LogControl"); xnLogCtrl;
			xnLogCtrl = xnLogCtrl.next_sibling("LogControl"))
	{
		ParseLogCtrl(xnLogCtrl, xnLN);
	}
	/* GSEControlBlock */
	for(pugi::xml_node xnGseCtrl = xnLN.child("GSEControl"); xnGseCtrl;
			xnGseCtrl = xnGseCtrl.next_sibling("GSEControl"))
	{
		ParseGseCtrl(xnGseCtrl, xnLN);
	}

	return(0);
}

int CSCLParser::ParseDO(const pugi::xml_node& xnDO, bool bSDO)
{
	bool pushed = false;
	char *doType = (char *)xnDO.attribute("type").value();
	if(mDOType.find(doType) == mDOType.end())
		return(ERROR_MISSING_DOTYPE);
	pugi::xml_node xnDOType = mDOType.find(doType)->second;

	for(pugi::xml_node xnSDO = xnDOType.child("SDO"); xnSDO; 
		xnSDO = xnSDO.next_sibling("SDO"))
	{
		if(! stkDOI.empty())
		{
			pugi::xml_node xnParent = stkDOI.top();
			pugi::xml_node xnChild = xnParent.find_child_by_attribute(
					"SDI", "name", xnSDO.attribute("name").value());
			if(xnChild)
			{
				stkDOI.push(xnChild);
				pushed = true;
			}
			else
				pushed = false;
		}
		//
		ctx += string("DO(")
			+ xnSDO.attribute("name").value() + " "
			+ string(xnSDO.attribute("count") ? xnSDO.attribute("count").value() : "0")
			+ "){\n";
		//
		ParseDO(xnSDO, true);
		ctx += "}\n";
		//
		if(pushed)
		{
			stkDOI.pop();
		}
	}
	/* DA */
	char fc[16], trgopt[16];
	for(pugi::xml_node xnDA = xnDOType.child("DA"); xnDA; xnDA = xnDA.next_sibling("DA"))
	{
		GetDAFC(xnDA, fc);
		GetDATrigOpt(xnDA, trgopt);
		ParseDA(xnDA, fc, trgopt);
		if(! strcmp(fc, "6")) /* SG == 6, SE == 7 */
			ParseDA(xnDA, "7", trgopt);
	}

	return(0);
}

int CSCLParser::ParseDA(
		const pugi::xml_node& xnDA, 
		const char *fc, 
		const char *trgopt
		)
{
	char *daType = (char *)xnDA.attribute("type").value();
	char *daBType =(char *)xnDA.attribute("bType").value();
	char val[4][64];
	string daVal;
	bool pushed = false;
	int type = 0;
	
	if(!strcmp(daBType, "Struct"))
	{/* composite type */
		pugi::xml_node xnDAType;
		if(mDAType.find(daType) == mDAType.end())
			return(ERROR_MISSING_DATYPE);
		xnDAType = mDAType.find(daType)->second;
		ctx += string("DA(")
			+ xnDA.attribute("name").value() + " "
			+ GetDACount(xnDA, val[0]) + " "
			+ GetDAType(xnDA, val[1], &type) + " "
			+ fc + " "
			+ trgopt + " "
			+ GetDASAddr(xnDA, val[3])
			+ "){\n";
		//
		if(! stkDOI.empty())
		{
			pugi::xml_node xnParent = stkDOI.top();
			if(xnParent)
			{
				pugi::xml_node xnChild = xnParent.find_child_by_attribute(
						"SDI", "name", xnDA.attribute("name").value());
				if(xnChild)
				{
					stkDOI.push(xnChild);
					pushed = true;
					//fprintf(stderr, "PUSH: %s\n", xnDA.attribute("name").value());
				}
				else
					pushed = false;
			}
		}
		//
		for(pugi::xml_node xnBDA = xnDAType.child("BDA"); xnBDA;
				xnBDA = xnBDA.next_sibling("BDA"))
		{
			ParseDA(xnBDA, fc, trgopt);
		}
		ctx += "}\n";
		//
		if(pushed)
		{
			/*
			pugi::xml_node xnT = stkDOI.top();
			fprintf(stderr, "POP: %s\n", 
					xnT.attribute("name").value());
					*/
			stkDOI.pop();
		}
	}
	else
	{
		ctx += string("DA(")
			+ xnDA.attribute("name").value() + " "
			+ GetDACount(xnDA, val[0]) + " "
			+ GetDAType(xnDA, val[1], &type) + " "
			+ fc + " "
			+ trgopt + " "
			+ GetDASAddr(xnDA, val[3])
			+ ")";
		if(GetDAVal(daVal, xnDA))
			ctx += daVal;
		ctx += ";\n";
	}

	return(0);
}

char *CSCLParser::GetDAVal(string& mval, const pugi::xml_node& xnDA)
{
	char ctype[64], t[64];
	int ntype = -1;
	pugi::xml_node xnDAI, xnVal;

	/* default for Val in DataTypeTemplates */
	xnVal = xnDA.child("Val");
	/* DA was instantitated */
	if(! stkDOI.empty())
	{
		pugi::xml_node xnParent = stkDOI.top();
		if(xnParent)
		{
			xnDAI = xnParent.find_child_by_attribute(
					"DAI", "name", xnDA.attribute("name").value());
			if(xnDAI)
				xnVal = xnDAI.child("Val");
		}
	}
	if(xnVal)
	{
		GetDAType(xnDA, ctype, &ntype);
		switch(ntype)
		{
			case IEC61850_ENUMERATED:
				mval += string("=") + GetEnumOrd(xnDA, xnVal);
				break;
			case IEC61850_INT8:
			case IEC61850_INT16:
			case IEC61850_INT32:
			case IEC61850_INT64:
				mval += string("=") + xnVal.text().get();
				break;
			case IEC61850_INT8U:
			case IEC61850_INT16U:
			case IEC61850_INT24U:
			case IEC61850_INT32U:
				mval += string("=") + xnVal.text().get();
				break;
			case IEC61850_FLOAT32:
			case IEC61850_FLOAT64:
				sprintf(t, "%f", atof(xnVal.text().get()));
				mval += string("=") + t;
				break;
			case IEC61850_UNICODE_STRING_255:
			case IEC61850_VISIBLE_STRING_32:
			case IEC61850_VISIBLE_STRING_64:
			case IEC61850_VISIBLE_STRING_129:
			case IEC61850_VISIBLE_STRING_255:
			case IEC61850_VISIBLE_STRING_65:
				mval += string("=\"") + xnVal.text().get()
						+ string("\"");
				break;
		}
	}

	return((char *)mval.c_str());
}

char *CSCLParser::GetEnumOrd(
		const pugi::xml_node& xnDA, const pugi::xml_node& xnV)
{
	const char *v = xnV.text().get();
	if(!v)
		return("-1");
	//
	pugi::xml_node xnEnumType;
	if(mEnumType.find(xnDA.attribute("name").value()) != mEnumType.end())
		xnEnumType = mEnumType.find(xnDA.attribute("name").value())->second;
	else
	{
		sprintf(szEchoStr, "%s,%d: Missing EnumType: %s\n", 
			__FILE__, __LINE__, xnDA.attribute("name").value());
		return("-1");
	}
	pugi::xml_node xnEnumVal;
	bool fnd = false;
	for(xnEnumVal = xnEnumType.child("EnumVal"); xnEnumVal;
			xnEnumVal = xnEnumVal.next_sibling("EnumVal"))
	{
		if((!strcmp(xnEnumVal.text().get(), v)) 
				|| !strcmp(v, xnEnumVal.attribute("ord").value()))
		{
			fnd = true;
			break;
		}
	}
	if(fnd)
		return((char *)xnEnumVal.attribute("ord").value());
	else
		return("-1");
}

int CSCLParser::ParseGseCtrl(const pugi::xml_node& xnGseCtrl,
		const pugi::xml_node& xnLN)
{
	char t[32], s[32];
	char *gocb = (char *)xnGseCtrl.attribute("name").value();
	char *ldins= (char *)xnLN.parent().attribute("inst").value();
	//	
	xpath = "/SCL/Communication/SubNetwork/ConnectedAP[@iedName='"
		  + iedName + "' and @apName='" + apName + "']"
		  + "/GSE[@cbName='" + gocb + "' and @ldInst='"
		  + ldins + "']";
	pugi::xml_node xnConnedAP = doc.select_single_node(xpath.c_str()).node();
	//
	ctx += "GC("
		+ string(xnGseCtrl.attribute("name").value()) + " "
		+ string(xnGseCtrl.attribute("appID").value()) + " "
		+ string(xnGseCtrl.attribute("datSet").value()) + " "
		+ string(xnGseCtrl.attribute("confRev").value())+ " "
		+ "0 ";
	//
	pugi::xml_node xnMinTime = xnConnedAP.child("MinTime");
	if(xnMinTime)
		ctx += string(xnMinTime.text().get());
	else
		ctx += "-1";
	ctx += " ";
	pugi::xml_node xnMaxTime = xnConnedAP.child("MaxTime");
	if(xnMaxTime)
		ctx += string(xnMaxTime.text().get());
	else
		ctx += "-1";
	ctx += " ";
	//
	pugi::xml_node xnAddress = xnConnedAP.child("Address");
	if(! xnAddress)
		ctx += "){\n}\n";
	else
	{
		ctx += "){\n";
		ctx += "PA(";
		pugi::xml_node xnNode = xnAddress.find_child_by_attribute("P", "type", "VLAN-PRIORITY");
		if(xnNode)
			ctx += string(xnNode.text().get()) + " ";
		xnNode = xnAddress.find_child_by_attribute("P", "type", "VLAN-ID");
		if(xnNode)
		{
			sprintf(t, "%d", strtol(xnNode.text().get(), NULL, 16));
			ctx += string(t) + " ";
		}
		xnNode = xnAddress.find_child_by_attribute("P", "type", "APPID");
		if(xnNode)
		{
			sprintf(t, "%d", strtol(xnNode.text().get(), NULL, 16));
			ctx += string(t) + " ";
		}
		xnNode = xnAddress.find_child_by_attribute("P", "type", "MAC-Address");
		if(xnNode)
		{
			memset(s, 0x00, sizeof(s));
			memset(t, 0x00, sizeof(t));
			strcpy(t, xnNode.text().get());
			for(int i = 0, j = 0; i < strlen(t); i++)
			{
				if(t[i] == '-')
					continue;
				s[j++] = t[i];
			}
			ctx += s;
		}
		ctx += ");\n";
		ctx += "}\n";
	}

	return(0);
}

int CSCLParser::ParseLogCtrl(const pugi::xml_node& xnLogCtrl,
		const pugi::xml_node& xnLN)
{
	ctx += "LC(";
	ctx += string(xnLogCtrl.attribute("name").value()) + " ";
	if(xnLogCtrl.attribute("datSet"))
		ctx += string(xnLogCtrl.attribute("datSet").value());
	else
		ctx += "-";
	ctx += " ";
	/*
	if(xnLogCtrl.attribute("logName"))
		ctx += string(xnLN.parent().attribute("inst").value()) + "/"
			+ xnLN.attribute("lnClass").value() + "$"
			+ xnLogCtrl.attribute("logName").value();
			*/
	if(xnLogCtrl.attribute("logName"))
		ctx += string(xnLN.parent().attribute("inst").value()) + "/"
			+ string(xnLN.parent().attribute("inst").value());
	else
		ctx += "-";
	ctx += " ";
	//
	char val[8];
	ctx += string(GetRptTrgOpt(xnLogCtrl, val)) + " ";
	//
	if(xnLogCtrl.attribute("intgPd"))
		ctx += string(xnLogCtrl.attribute("intgPd").value());
	else
		ctx += "0";
	ctx += " ";
	/* note: default value to logEna is true */
	if(xnLogCtrl.attribute("logEna"))
	{
		if(!strcmp(xnLogCtrl.attribute("logEna").value(), "true"))
			ctx += "1";
		else
			ctx += "0";
	}
	else
		ctx += "1";
	ctx += " ";
	//
	if(xnLogCtrl.attribute("reasonCode"))
	{
		if(!strcmp(xnLogCtrl.attribute("reasonCode").value(), "true"))
			ctx += "1);";
		else
			ctx += "0);";
	}
	else
		ctx += "1);";
	ctx += "\n";

	return(0);
}

int CSCLParser::ParseRptCtrl(const pugi::xml_node& xnRptCtrl)
{
	char *indexed = (char *)xnRptCtrl.attribute("indexed").value();
	int max = 1;

	pugi::xml_node xnRptEnabled = xnRptCtrl.child("RptEnabled");
	if(xnRptEnabled)
	{
		if(xnRptEnabled.attribute("max"))
			max = atoi(xnRptEnabled.attribute("max").value());
	}
	/* note: default value for indexed is true */
	if(!strlen(indexed) || !strcmp(indexed, "true"))
	{
		char t[8];
		for(int i = 1; i <= max; i++)
		{
			sprintf(t, "%02d", i);
			DumpRptInfo(xnRptCtrl, t);
		}
	}
	else
		DumpRptInfo(xnRptCtrl, "");

	return(0);
}

void CSCLParser::DumpRptInfo(const pugi::xml_node& xnRptCtrl, const char *index)
{
	char val[2][8];

	// name
	ctx += string("RC(") + xnRptCtrl.attribute("name").value() + index + " ";
	// rptid
	if(xnRptCtrl.attribute("rptID") &&
			strlen(xnRptCtrl.attribute("rptID").value()))
		ctx += xnRptCtrl.attribute("rptID").value();
	else
		ctx += "-";
	ctx += " ";
	//
	if(xnRptCtrl.attribute("buffered"))
		if(!strcmp(xnRptCtrl.attribute("buffered").value(), "true"))
			ctx += "1 ";
		else
			ctx += "0 ";
	else
		ctx += "0 ";
	//
	if(xnRptCtrl.attribute("datSet"))
		ctx += xnRptCtrl.attribute("datSet").value();
	else
		ctx += "-";
	ctx += " ";
	//
	ctx += string(xnRptCtrl.attribute("confRev").value()) + " ";
	ctx += string(GetRptTrgOpt(xnRptCtrl, val[1])) + " ";
	//
	if(xnRptCtrl.child("OptFields"))
		ctx += string(GetRptOptFld(xnRptCtrl.child("OptFields"), val[2])) + " ";
	if(xnRptCtrl.attribute("bufTime"))
		ctx += string(xnRptCtrl.attribute("bufTime").value());
	else
		ctx += "0";
	ctx += " ";
	if(xnRptCtrl.attribute("intgPd"))
		ctx += string(xnRptCtrl.attribute("intgPd").value());
	else
		ctx += "0";
	ctx += ");\n";
}

char *CSCLParser::GetRptTrgOpt(const pugi::xml_node& xnRpt, char *val)
{
	int value = 0;
	pugi::xml_node xnTrgOpt = xnRpt.child("TrgOps");
	if(xnTrgOpt)
	{
		if(xnTrgOpt.attribute("dchg"))
			if(!strcmp(xnTrgOpt.attribute("dchg").value(), "true"))
				value += 1;
		if(xnTrgOpt.attribute("qchg"))
			if(!strcmp(xnTrgOpt.attribute("qchg").value(), "true"))
				value += 2;
		if(xnTrgOpt.attribute("dupd"))
			if(!strcmp(xnTrgOpt.attribute("dupd").value(), "true"))
				value += 4;
		if(xnTrgOpt.attribute("period"))
			if(!strcmp(xnTrgOpt.attribute("period").value(), "true"))
				value += 8;
		if(xnTrgOpt.attribute("gi"))
		{
			if(!strcmp(xnTrgOpt.attribute("gi").value(), "true"))
				value += 16;
		}
		else
			value += 16;
	}
	else
		value = 16;
	sprintf(val, "%d", value);

	return(val);
}

char *CSCLParser::GetRptOptFld(const pugi::xml_node& xnRpt, char *val)
{
	int value = 0;

	if(xnRpt.attribute("seqNum"))
		if(!strcmp(xnRpt.attribute("seqNum").value(), "true"))
			value += 1;
	if(xnRpt.attribute("timeStamp"))
		if(!strcmp(xnRpt.attribute("timeStamp").value(), "true"))
			value += 2;
	if(xnRpt.attribute("reasonCode"))
		if(!strcmp(xnRpt.attribute("reasonCode").value(), "true"))
			value += 4;
	if(xnRpt.attribute("dataSet"))
		if(!strcmp(xnRpt.attribute("dataSet").value(), "true"))
			value += 8;
	if(xnRpt.attribute("dataRef"))
		if(!strcmp(xnRpt.attribute("dataRef").value(), "true"))
			value += 16;
	/* note: default valuue is 'true' */
	if(xnRpt.attribute("bufOvfl"))
	{
		if(!strcmp(xnRpt.attribute("bufOvfl").value(), "true"))
			value += 32;
	}
	else
		value += 32;
	if(xnRpt.attribute("entryID"))
		if(!strcmp(xnRpt.attribute("entryID").value(), "true"))
			value += 64;
	if(xnRpt.attribute("configRef"))
		if(!strcmp(xnRpt.attribute("configRef").value(), "true"))
			value += 128;
	sprintf(val, "%d", value);

	return(val);
}

int CSCLParser::ParseDataSet(const pugi::xml_node& xnDataSet)
{
	string lnref = "";
	string ldInst = "";
	char val[2][64];
	char *parentLDInst = (char *)xnDataSet.parent().parent().attribute("inst").value();

	for(pugi::xml_node xnFCDA = xnDataSet.child("FCDA"); xnFCDA;
			xnFCDA = xnFCDA.next_sibling("FCDA"))
	{
		memset(val[0], 0x00, sizeof(val[0]));
		memset(val[1], 0x00, sizeof(val[1]));
		lnref.clear();
		ldInst.clear();
		lnref = 
			string(xnFCDA.attribute("prefix") ? xnFCDA.attribute("prefix").value() : "")
		  + xnFCDA.attribute("lnClass").value()
		  + string(xnFCDA.attribute("lnInst") ? xnFCDA.attribute("lnInst").value() : "");
		//
		pugi::xml_node xnLDevice = xnFCDA.parent().parent();
		const char *childLDInst = xnFCDA.attribute("ldInst").value();
		if(strcmp(parentLDInst, childLDInst))
			ldInst = childLDInst;
		//
		if(xnFCDA.attribute("doName"))
			strncpy(val[0], xnFCDA.attribute("doName").value(), sizeof(val[0])-1);
		if(xnFCDA.attribute("daName"))
			strncpy(val[1], xnFCDA.attribute("daName").value(), sizeof(val[1])-1);
		ctx += string("DE(")
			+ string(ldInst.size() ? ldInst + "/" : "")
			+ lnref + "$"
			+ xnFCDA.attribute("fc").value();
		if(strlen(val[0]))
			ctx += "$" + string(Convert2MmsString(val[0], strlen(val[0])));
		if(strlen(val[1]))
			ctx += string("$") + Convert2MmsString(val[1], strlen(val[1])) + ");\n";
		else
			ctx += ");\n";
	}

	return(0);
}

char *CSCLParser::GetDACount(const pugi::xml_node& xnDA, char *val)
{
	strcpy(val, xnDA.attribute("count") ? xnDA.attribute("count").value() : "0");
	return(val);
}

char *CSCLParser::GetDATrigOpt(const pugi::xml_node& xnDA, char *val)
{
	int value = 0;

	if(xnDA.attribute("dchg"))
		if(!strcmp(xnDA.attribute("dchg").value(), "true"))
			value += 1;
	if(xnDA.attribute("qchg"))
		if(!strcmp(xnDA.attribute("qchg").value(), "true"))
			value += 2;
	if(xnDA.attribute("dupd"))
		if(!strcmp(xnDA.attribute("dupd").value(), "true"))
			value += 4;
	if(xnDA.attribute("period"))
		if(!strcmp(xnDA.attribute("period").value(), "true"))
			value += 8;
	if(xnDA.attribute("gi"))
		if(!strcmp(xnDA.attribute("gi").value(), "true"))
			value += 16;
	sprintf(val, "%d", value);

	return(val);
}

char *CSCLParser::GetDASAddr(const pugi::xml_node& xnDA, char *val)
{
	strcpy(val, "0");
	return(val);
}

int CSCLParser::ParseSGCB(const pugi::xml_node& xnSGCB)
{
	ctx += std::string("SG(");
	ctx += xnSGCB.attribute("ActSG") ? xnSGCB.attribute("ActSG").value() : "1";
	ctx += " ";
	ctx += xnSGCB.attribute("numOfSGs") ? xnSGCB.attribute("numOfSGs").value() : "1";
	ctx += ")\n";	

	return(ERROR_OK);
}

void CSCLParser::GetCfgCtx(std::string& _ctx_)
{
	_ctx_ = ctx;
}

int CSCLParser::ParseIEDSection()
{
	pugi::xml_node xnRoot = doc.select_single_node("/SCL").node();
	if(! xnRoot)
		return(ERROR_MISSING_SCL);
	mIED.clear();
	for(pugi::xml_node xnIED = xnRoot.child("IED"); xnIED;
		xnIED = xnIED.next_sibling("IED"))
	{
		mIED.insert(make_pair(xnIED.attribute("name").value(), xnIED));
	}
}

int CSCLParser::ParseCommunicationSection()
{
	pugi::xml_node xnRoot = doc.select_single_node("/SCL/Communication").node();
	if(!xnRoot)
		return(ERROR_MISSING_COMMUNICATION);
	mSubNet.clear();
	for(pugi::xml_node xnSubNet = xnRoot.child("SubNetwork"); xnSubNet;
			xnSubNet = xnSubNet.next_sibling("SubNetwork"))
	{
		char *subName = (char *)xnSubNet.attribute("name").value();
		for(pugi::xml_node xnConnAP = xnSubNet.child("ConnectedAP"); xnConnAP;
				xnConnAP = xnConnAP.next_sibling("ConnectedAP"))
		{
			char *iedName = (char *)xnConnAP.attribute("iedName").value();
			char *apName = (char *)xnConnAP.attribute("apName").value();
			map<string, map<string, vector<string>>>::iterator it;
			if(mSubNet.find(iedName) == mSubNet.end())
			{
				vector<string> V;
				map<string, vector<string>> M;
				V.push_back(apName);
				M.insert(make_pair(subName, V));
				mSubNet.insert(make_pair(iedName, M));
			}
			else
			{
				it = mSubNet.find(iedName);
				//
				if((it->second).find(subName) == (it->second).end())
				{
					vector<string> V;
					V.push_back(apName);
					(it->second).insert(make_pair(subName, V));
				}
				else
					((it->second).find(subName)->second).push_back(apName);
			}
		}
	}
}

void CSCLParser::GetIEDList(map<string, pugi::xml_node>& M)
{
	M = mIED;
}

char *CSCLParser::GetSubNetType(
		const char *iedName,
		const char *apName,
		char *type
		)
{
	char szXPath[256];

	sprintf(szXPath, 
			"/SCL/Communication/SubNetwork/ConnectedAP"
			"[@iedName='%s' and @apName='%s']",
			iedName, apName);
	pugi::xml_node xnNode = doc.select_single_node(szXPath).node();
	if(!xnNode)
		return("-");
	pugi::xml_node xnSubNet = xnNode.parent();
	strcpy(type, xnSubNet.attribute("type").value());

	return(type);
}

CSCLParser::~CSCLParser()
{
	mIED.clear();
	mLNType.clear();
	mDOType.clear();
	mDAType.clear();
	mEnumType.clear();
	mSubNet.clear();
}
