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
		nType = BOOLEAN;
	else if(!strcmp(type, "INT8"))
		nType = INT8;
	else if(!strcmp(type, "INT16"))
		nType = INT16;
	else if(!strcmp(type, "INT32"))
		nType = INT32;
	else if(!strcmp(type, "INT64"))
		nType = INT64;
	else if(!strcmp(type, "INT128"))
		nType = INT128;
	else if(!strcmp(type, "INT8U"))
		nType = INT8U;
	else if(!strcmp(type, "INT16U"))
		nType = INT16U;
	else if(!strcmp(type, "INT24U"))
		nType = INT24U;
	else if(!strcmp(type, "INT32U"))
		nType = INT32U;
	else if(!strcmp(type, "FLOAT32"))
		nType = FLOAT32;
	else if(!strcmp(type, "FLOAT64"))
		nType = FLOAT64;
	else if(!strcmp(type, "Enum"))
		nType = ENUMERATED;
	else if(!strcmp(type, "Dbpos"))
		nType = CODEDENUM;
	else if(!strcmp(type, "Check"))
		nType = CHECK;
	else if(!strcmp(type, "Tcmd"))
		nType = CODEDENUM;
	else if(!strcmp(type, "Octet64"))
		nType = OCTET_STRING_64;
	else if(!strcmp(type, "Quality"))
		nType = QUALITY;
	else if(!strcmp(type, "Timestamp"))
		nType = TIMESTAMP;
	else if(!strcmp(type, "VisString32"))
		nType = VISIBLE_STRING_32;
	else if(!strcmp(type, "VisString64"))
		nType = VISIBLE_STRING_64;
	else if(!strcmp(type, "VisString65"))
		nType = VISIBLE_STRING_65;
	else if(!strcmp(type, "VisString129"))
		nType = VISIBLE_STRING_129;
	else if(!strcmp(type, "ObjRef"))
		nType = VISIBLE_STRING_129;
	else if(!strcmp(type, "VisString255"))
		nType = VISIBLE_STRING_255;
	else if(!strcmp(type, "Unicode255"))
		nType = UNICODE_STRING_255;
	else if(!strcmp(type, "OptFlds"))
		nType = GENERIC_BITSTRING;
	else if(!strcmp(type, "TrgOps"))
		nType = GENERIC_BITSTRING;
	else if(!strcmp(type, "EntryID"))
		nType = OCTET_STRING_8;
	else if(!strcmp(type, "EntryTime"))
		nType = ENTRY_TIME;
	else if(!strcmp(type, "PhyComAddr"))
		nType = PHYCOMADDR;
	else if(!strcmp(type, "Struct"))
		nType = CONSTRUCTED;
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

