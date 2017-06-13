/*
 * File: $(model_generator_cxx)/SclParser.h
 */
#ifndef SCL_PARSER_H
#define SCL_PARSER_H

#include <string>
#include <map>
#include <stack>
#include <vector>
#include "platform.h"
#include "pugixml.hpp"

using namespace std;

class CSCLParser
{
	public:
		CSCLParser();
		~CSCLParser();
		void SetSCLName(const char *scl);
		void SetAPName(const char *ap);
		void SetIEDName(const char *ied);
		int LoadFileUnix();
		int LoadFileWind();
		int LoadFileEx(const char *ctx, unsigned int cbSize);
		int ParseFile();
		void GetCfgCtx(std::string& ctx);
		void GetIEDList(map<string, pugi::xml_node>& M);
		char *GetSubNetType(
				const char *iedName, 
				const char *apName,
				char *type);
	private:
		stack<pugi::xml_node> stkDOI;
	private: /* api for parsing */
		pugi::xml_document doc;
		pugi::xpath_node_set xnConnectedAP;
		map<string, pugi::xml_node> mIED;
		map<string, pugi::xml_node> mLNType;
		map<string, pugi::xml_node> mDOType;
		map<string, pugi::xml_node> mDAType;
		map<string, pugi::xml_node> mEnumType;
		map<string, map<string, vector<string>>> mSubNet;
		int ParseIEDSection();
		int ParseCommunicationSection();
		int ParseDataTypeTemplates();
		int ParseIED(const pugi::xml_node& xnIED);
		int ParseAccessPoint(const pugi::xml_node& xnAP);
		int ParseLDevice(const pugi::xml_node& xnLDevice);
		int ParseLN(const pugi::xml_node& xnLN, bool bLN0);
		int ParseSGCB(const pugi::xml_node& xnSGCB);
		int ParseDO(const pugi::xml_node& xnDOI, bool bSDO);
		int ParseDA(const pugi::xml_node& xnDO, const char *fc,
				const char *trgopt);
		int ParseDataSet(const pugi::xml_node& xnDataSet);
		int ParseRptCtrl(const pugi::xml_node& xnRptCtrl);
		void DumpRptInfo(const pugi::xml_node& xnRpt, const char *);
		int ParseLogCtrl(const pugi::xml_node& xnLogCtrl,
				const pugi::xml_node& xnLN, bool bLNO);
		int ParseGseCtrl(const pugi::xml_node& xnGseCtrl,
				const pugi::xml_node& xnLN);
		char *GetDACount(const pugi::xml_node& xnDA, char *val);
		char *GetDAType(const pugi::xml_node& xnDA, char *val, int *type);
		char *GetDAVal(string& mval, const pugi::xml_node& xnDA);
		char *GetDAFC(const pugi::xml_node& xnDA, char *val);
		char *GetDATrigOpt(const pugi::xml_node& xnDA, char *val);
		char *GetDASAddr(const pugi::xml_node& xnDA, char *val);
		char *GetRptTrgOpt(const pugi::xml_node& xnNode, char *val);
		char *GetRptOptFld(const pugi::xml_node& xnNode, char *val);
		char *GetEnumOrd(const pugi::xml_node& xnDA, const pugi::xml_node& xnV);
		char *GetLNName(const pugi::xml_node& xnLN, bool bLN0);
	private: /* api for validation */
		int CheckDocSCL();
		int CheckRequiredAttrs(const pugi::xml_node xnNode, int type, string& str);
	private:
		char *Convert2MmsString(char *str, int len);
	private:
		char szEchoStr[256];
		int nRetCode;
		string xpath;
		string sclName;
		string apName;
		string iedName;
		string ctx;
};

#endif
