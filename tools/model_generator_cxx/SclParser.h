/*
 * File: $(model_generator_cxx)/SclParser.h
 */
#ifndef SCL_PARSER_H
#define SCL_PARSER_H

#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
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
		int LoadFile();
		int ParseFile();
		void UnloadFile();
		void GetCfgCtx(std::string& ctx);
	private:
		pugi::xml_document doc;
		pugi::xpath_node_set xnConnectedAP;
		std::map<string, pugi::xml_node> mLNType;
		std::map<string, pugi::xml_node> mDOType;
		std::map<string, pugi::xml_node> mDAType;
		std::map<string, pugi::xml_node> mEnumType;
		void ParseDataTypeTemplates();
		int ParseIED(const pugi::xml_node& xnIED);
		int ParseAccessPoint(const pugi::xml_node& xnAP);
		int ParseLDevice(const pugi::xml_node& xnLDevice);
		int ParseLN(const pugi::xml_node& xnLN, bool bLN0);
		void ParseSGCB(const pugi::xml_node& xnSGCB);
		int ParseDO(const pugi::xml_node& xnDOI);
		int ParseDA(const pugi::xml_node& xnDO, const char *fc,
				const char *trgopt);
		int ParseDataSet(const pugi::xml_node& xnDataSet);
		int ParseRptCtrl(const pugi::xml_node& xnRptCtrl);
		void DumpRptInfo(const pugi::xml_node& xnRpt, const char *);
		int ParseLogCtrl(const pugi::xml_node& xnLogCtrl,
				const pugi::xml_node& xnLN);
		int ParseGseCtrl(const pugi::xml_node& xnGseCtrl,
				const pugi::xml_node& xnLN);
		char *GetDACount(const pugi::xml_node& xnDA, char *val);
		char *GetDAType(const pugi::xml_node& xnDA, char *val);
		char *GetDAFC(const pugi::xml_node& xnDA, char *val);
		char *GetDATrigOpt(const pugi::xml_node& xnDA, char *val);
		char *GetDASAddr(const pugi::xml_node& xnDA, char *val);
		char *GetRptTrgOpt(const pugi::xml_node& xnNode, char *val);
		char *GetRptOptFld(const pugi::xml_node& xnNode, char *val);
	private:
		char *Convert2MmsString(char *str, int len);
	private:
		string xpath;
		string sclName;
		string apName;
		string iedName;
		string ctx;
};

#endif
