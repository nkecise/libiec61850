/*
 * File: $(model_generator_cxx)/SclParser.cpp
 */
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SclParser.h"
#include "Error.h"

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

int CSCLParser::LoadFile()
{
	int fd = 0;
	char *xml = NULL;
	struct stat st;

	fd = open(sclName.c_str(), O_RDWR);
	if(fd == -1)
	{
		fprintf(stderr, "%s:%d, open '%s', %d\n",
				__FILE__, __LINE__, sclName.c_str(), errno);
		return(errno);
	}
	if(fstat(fd, &st) == -1)	
		return(errno);
	xml = (char *)mmap(NULL, st.st_size + 1, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(xml == MAP_FAILED)
	{
		close(fd);
		return(errno);
	}
	xml[st.st_size] = '\0';
	close(fd);
	//
	doc.load_buffer(xml, st.st_size);
	munmap(xml, st.st_size);
	ParseDataTypeTemplates();

	return(0);
}

int CSCLParser::ParseFile()
{
	pugi::xml_node xnIED;
	char xpath[256];

	if(iedName.size())
	{
		memset(xpath, 0x00, sizeof(xpath));
		sprintf(xpath, "/SCL/IED[@name=\"%s\"]", iedName.c_str());
		xnIED = doc.select_single_node(xpath).node();
	}
	else
		xnIED = doc.select_single_node("/SCL/IED").node();
	if(!xnIED)
		return(ERROR_MISSING_IED);	
	iedName = string(xnIED.attribute("name").value());
	ParseIED(xnIED);

	return(0);
}

void CSCLParser::ParseDataTypeTemplates()
{
	pugi::xml_node xnRoot;

	xnRoot = doc.select_single_node("/SCL/DataTypeTemplates").node();
	pugi::xml_node xnNode = xnRoot.child("LNodeType");
	do
	{
		mLNType.insert(std::make_pair(xnNode.attribute("id").value(), xnNode));
	}while(xnNode = xnNode.next_sibling("LNodeType"));

	xnNode = xnRoot.child("DOType");
	do
	{
		mDOType.insert(std::make_pair(xnNode.attribute("id").value(), xnNode));
	}while(xnNode = xnNode.next_sibling("DOType"));

	xnNode = xnRoot.child("DAType");
	do
	{
		mDAType.insert(std::make_pair(xnNode.attribute("id").value(), xnNode));
	}while(xnNode = xnNode.next_sibling("DAType"));

	xnNode = xnRoot.child("EnumType");
	do
	{
		mEnumType.insert(std::make_pair(xnNode.attribute("id").value(), xnNode));
	}while(xnNode = xnNode.next_sibling("EnumType"));
}

int CSCLParser::ParseIED(const pugi::xml_node& xnIED)
{
	ctx += std::string("MODEL(") 
		+ xnIED.attribute("name").value() 
		+ "){\n";
	//
	pugi::xml_node xnAP;
	if(apName.size())
		xnAP = xnIED.find_child_by_attribute(
				"AccessPoint", apName.c_str());
	else
		xnAP = xnIED.child("AccessPoint");
	if(!xnAP)
		return(ERROR_MISSING_AP);
	apName = xnAP.attribute("name").value();
	//
	xpath = "/SCL/Communication/SubNetwork/ConnectedAP[@iedName='"
		  + iedName + "' and @apName='"
		  + apName + "']";
	xnConnectedAP = doc.select_nodes(xpath.c_str());
	ParseAccessPoint(xnAP);
	ctx += "}";

	printf("%s\n", ctx.c_str());

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
	pugi::xml_node xnSGCB = xnLN.child("SettingControl");
	if(xnSGCB)
		ParseSGCB(xnSGCB);
	/* DO */
	char *lnType = (char *)xnLN.attribute("lnType").value();
	if(mLNType.find(lnType) == mLNType.end())
		return(ERROR_MISSING_LNTYPE);
	pugi::xml_node xnLNType = mLNType.find(lnType)->second;
	for(pugi::xml_node xnDO = xnLNType.child("DO"); xnDO; xnDO = xnDO.next_sibling("DO"))
	{
		ctx += std::string("DO(")
			+ xnDO.attribute("name").value() + " "
			+ string(xnDO.attribute("count") ? xnDO.attribute("count").value() : "0")
			+ "){\n";
		ParseDO(xnDO);
		ctx += "}\n";
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
		ctx += ");\n";	
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
	if(xnLogCtrl.attribute("dataSet"))
		ctx += string(xnLogCtrl.attribute("dataSet").value());
	else
		ctx += "-";
	ctx += " ";
	//
	if(xnLogCtrl.attribute("logName"))
		ctx += string(xnLN.parent().attribute("inst").value()) + "/"
			+ xnLN.attribute("name").value() + "$"
			+ xnLogCtrl.attribute("logName").value();
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
}

void CSCLParser::DumpRptInfo(const pugi::xml_node& xnRptCtrl, const char *index)
{
	char val[2][8];

	ctx += string("RC(") + xnRptCtrl.attribute("name").value() + index + " ";
	if(xnRptCtrl.attribute("rptID"))
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
	if(xnRptCtrl.child("TrgOps"))
		ctx += string(GetRptTrgOpt(xnRptCtrl.child("TrgOps"), val[1])) + " ";
	//
	if(xnRptCtrl.child("OptFields"))
		ctx += string(GetRptOptFld(xnRptCtrl.child("OptFields"), val[2])) + " ";
	if(xnRptCtrl.attribute("bufTime"))
		ctx += string(xnRptCtrl.attribute("bufTime").value()) + " ";
	if(xnRptCtrl.attribute("intgPd"))
		ctx += string(xnRptCtrl.attribute("intgPd").value());
	else
		ctx += "0";
	ctx += ");\n";
}

char *CSCLParser::GetRptTrgOpt(const pugi::xml_node& xnRpt, char *val)
{
	int value = 0;

	if(xnRpt.attribute("dchg"))
		if(!strcmp(xnRpt.attribute("dchg").value(), "true"))
			value += 1;
	if(xnRpt.attribute("qchg"))
		if(!strcmp(xnRpt.attribute("qchg").value(), "true"))
			value += 2;
	if(xnRpt.attribute("dupd"))
		if(!strcmp(xnRpt.attribute("dupd").value(), "true"))
			value += 4;
	if(xnRpt.attribute("period"))
		if(!strcmp(xnRpt.attribute("period").value(), "true"))
			value += 8;
	if(xnRpt.attribute("gi"))
	{
		if(!strcmp(xnRpt.attribute("gi").value(), "true"))
			value += 16;
	}
	else
		value += 16;
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

int CSCLParser::ParseDO(const pugi::xml_node& xnDO)
{
	char *doType = (char *)xnDO.attribute("type").value();
	if(mDOType.find(doType) == mDOType.end())
		return(ERROR_MISSING_DOTYPE);
	pugi::xml_node xnDOType = mDOType.find(doType)->second;
	/* SDO */
	for(pugi::xml_node xnSDO = xnDOType.child("SDO"); xnSDO; 
		xnSDO = xnSDO.next_sibling("SDO"))
	{
		ctx += string("DO(")
			+ xnSDO.attribute("name").value() + " "
			+ string(xnSDO.attribute("count") ? xnSDO.attribute("count").value() : "0")
			+ "){\n";
		ParseDO(xnSDO);
		ctx += "}\n";
	}
	/* DA */
	char fc[16], trgopt[16];
	for(pugi::xml_node xnDA = xnDOType.child("DA"); xnDA; xnDA = xnDA.next_sibling("DA"))
	{
		GetDAFC(xnDA, fc);
		GetDATrigOpt(xnDA, trgopt);
		ParseDA(xnDA, fc, trgopt);
	}

	return(0);
}

int CSCLParser::ParseDA(const pugi::xml_node& xnDA, const char *fc, const char *trgopt)
{
	char *daType = (char *)xnDA.attribute("type").value();
	char *daBType =(char *)xnDA.attribute("bType").value();
	char val[4][64];
	
	if(!strcmp(daBType, "Struct"))
	{/* composite type */
		pugi::xml_node xnDAType;
		if(mDAType.find(daType) == mDAType.end())
			return(ERROR_MISSING_DATYPE);
		xnDAType = mDAType.find(daType)->second;
		ctx += string("DA(")
			+ xnDA.attribute("name").value() + " "
			+ GetDACount(xnDA, val[0]) + " "
			+ GetDAType(xnDA, val[1]) + " "
			+ fc + " "
			+ trgopt + " "
			+ GetDASAddr(xnDA, val[3])
			+ "){\n";
		for(pugi::xml_node xnBDA = xnDAType.child("BDA"); xnBDA;
				xnBDA = xnBDA.next_sibling("BDA"))
		{
			ParseDA(xnBDA, fc, trgopt);
		}
		ctx += "}\n";
	}
	else
	{
		ctx += string("DA(")
			+ xnDA.attribute("name").value() + " "
			+ GetDACount(xnDA, val[0]) + " "
			+ GetDAType(xnDA, val[1]) + " "
			+ fc + " "
			+ trgopt + " "
			+ GetDASAddr(xnDA, val[3])
			+ ");\n";
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

void CSCLParser::ParseSGCB(const pugi::xml_node& xnSGCB)
{
	ctx += std::string("SG(");
	ctx += xnSGCB.attribute("ActSG") ? xnSGCB.attribute("ActSG").value() : "1";
	ctx += " ";
	ctx += xnSGCB.attribute("numOfSGs") ? xnSGCB.attribute("numOfSGs").value() : "1";
	ctx += ")\n";	
}

void CSCLParser::UnloadFile()
{
}

void CSCLParser::GetCfgCtx(std::string& _ctx_)
{
	_ctx_ = ctx;
}

CSCLParser::~CSCLParser()
{
}
