/*
 * File: $(model_generator_cxx)/AttributeType.cpp
 * Desc: 
 */
#include <string.h>
#include <stdio.h>
#include "AttributeType.h"
#include "SclParser.h"

char *CSCLParser::GetDAType(const pugi::xml_node& xnDA, char *val, int *i)
{
	char *type = (char *)xnDA.attribute("bType").value();
	int nType = -1;

	if(! strcmp(type, "BOOLEAN"))
		nType = IEC61850_BOOLEAN;
	else if(!strcmp(type, "INT8"))
		nType = IEC61850_INT8;
	else if(!strcmp(type, "INT16"))
		nType = IEC61850_INT16;
	else if(!strcmp(type, "INT32"))
		nType = IEC61850_INT32;
	else if(!strcmp(type, "INT64"))
		nType = IEC61850_INT64;
	else if(!strcmp(type, "INT128"))
		nType = IEC61850_INT128;
	else if(!strcmp(type, "INT8U"))
		nType = IEC61850_INT8U;
	else if(!strcmp(type, "INT16U"))
		nType = IEC61850_INT16U;
	else if(!strcmp(type, "INT24U"))
		nType = IEC61850_INT24U;
	else if(!strcmp(type, "INT32U"))
		nType = IEC61850_INT32U;
	else if(!strcmp(type, "FLOAT32"))
		nType = IEC61850_FLOAT32;
	else if(!strcmp(type, "FLOAT64"))
		nType = IEC61850_FLOAT64;
	else if(!strcmp(type, "Enum"))
		nType = IEC61850_ENUMERATED;
	else if(!strcmp(type, "Dbpos"))
		nType = IEC61850_CODEDENUM;
	else if(!strcmp(type, "Check"))
		nType = IEC61850_CHECK;
	else if(!strcmp(type, "Tcmd"))
		nType = IEC61850_CODEDENUM;
	else if(!strcmp(type, "Octet64"))
		nType = IEC61850_OCTET_STRING_64;
	else if(!strcmp(type, "Quality"))
		nType = IEC61850_QUALITY;
	else if(!strcmp(type, "Timestamp"))
		nType = IEC61850_TIMESTAMP;
	else if(!strcmp(type, "VisString32"))
		nType = IEC61850_VISIBLE_STRING_32;
	else if(!strcmp(type, "VisString64"))
		nType = IEC61850_VISIBLE_STRING_64;
	else if(!strcmp(type, "VisString65"))
		nType = IEC61850_VISIBLE_STRING_65;
	else if(!strcmp(type, "VisString129"))
		nType = IEC61850_VISIBLE_STRING_129;
	else if(!strcmp(type, "ObjRef"))
		nType = IEC61850_VISIBLE_STRING_129;
	else if(!strcmp(type, "VisString255"))
		nType = IEC61850_VISIBLE_STRING_255;
	else if(!strcmp(type, "Unicode255"))
		nType = IEC61850_UNICODE_STRING_255;
	else if(!strcmp(type, "OptFlds"))
		nType = IEC61850_GENERIC_BITSTRING;
	else if(!strcmp(type, "TrgOps"))
		nType = IEC61850_GENERIC_BITSTRING;
	else if(!strcmp(type, "EntryID"))
		nType = IEC61850_OCTET_STRING_8;
	else if(!strcmp(type, "EntryTime"))
		nType = IEC61850_ENTRY_TIME;
	else if(!strcmp(type, "PhyComAddr"))
		nType = IEC61850_PHYCOMADDR;
	else if(!strcmp(type, "Struct"))
		nType = IEC61850_CONSTRUCTED;
	sprintf(val, "%d", nType);
	*i = nType;

	return(val);
}

char *CSCLParser::GetDAFC(const pugi::xml_node& xnDA, char *val)
{
	char *fc = (char *)xnDA.attribute("fc").value();
	int nFC = -1;

	if(!strcmp(fc, "ST"))
		nFC = ST;
	if(!strcmp(fc, "MX"))
		nFC = MX;
	if(!strcmp(fc, "SP"))
		nFC = SP;
	if(!strcmp(fc, "SV"))
		nFC = SV;
	if(!strcmp(fc, "CF"))
		nFC = CF;
	if(!strcmp(fc, "DC"))
		nFC = DC;
	if(!strcmp(fc, "SG"))
		nFC = SG;
	if(!strcmp(fc, "SE"))
		nFC = SE;
	if(!strcmp(fc, "SR"))
		nFC = SR;
	if(!strcmp(fc, "OR"))
		nFC = OR;
	if(!strcmp(fc, "BL"))
		nFC = BL;
	if(!strcmp(fc, "EX"))
		nFC = EX;
	if(!strcmp(fc, "CO"))
		nFC = CO;
	if(!strcmp(fc, "ALL"))
		nFC = ALL;
	if(!strcmp(fc, "NONE"))
		nFC = NONE;
	sprintf(val, "%d", nFC);

	return(val);
}

