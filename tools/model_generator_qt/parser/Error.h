/*
 * File: $(model_generator_cxx)/Error.h
 */
#ifndef ERROR_H
#define ERROR_H

#define ERROR_BASE	65535
#define ERROR_MISSING_IED		ERROR_BASE-1
#define ERROR_MISSING_AP		ERROR_BASE-2
#define ERROR_MISSING_LNTYPE	ERROR_BASE-3
#define ERROR_MISSING_DOTYPE	ERROR_BASE-4
#define ERROR_MISSING_DATYPE	ERROR_BASE-5
#define ERROR_LOAD_FILE			ERROR_BASE-6
#define ERROR_PARSE_FILE		ERROR_BASE-7
#define ERROR_OPEN_FILE			ERROR_BASE-8

enum SCL_ERROR
{
	ERROR_OK = 0,
	/* Document has no SCL section defined */
	ERROR_MISSING_SCL = 32768,
	/* Document has no DataTypeTemplates section defined */
	ERROR_MISSING_DATA_TYPE_TEMPLATES,
	/* Document has no Communication section defined */
	ERROR_MISSING_COMMUNICATION, 
	/* Document contains more than one SCL section */
	ERROR_SCL_REPEATED,
	/* no subnetworks defined */
	ERROR_MISSING_SUBNETWORKS,
	/* ConnectedAP is missing required attribute 'ied' */
	ERROR_CONNECTEDAP_ATTR_MISSING_IED,
	/* ConnectedAP is missing required attribute 'apName' */
	ERROR_CONNECTEDAP_ATTR_MISSING_APNAME,
	/* GSE is missing required attribute 'ldInst' */
	ERROR_GSE_ATTR_MISSING_LDINST,
	/* GSE is missing required attribute 'cbName' */
	ERROR_GSE_ATTR_MISSING_CBNAME,
	/* GSE is missing Address definition */
	ERROR_GSE_MISSING_ADDRESS,
	/* VLAN-ID is out of range, 0x0FFF */
	ERROR_VLANID_OUT_OF_RANGE,
	/* APPID out of range */
	ERROR_APPID_OUT_OF_RANGE,
	/* MAC Address is malformed */
	ERROR_MAC_MALFORMED,
	/* SMV is missing required attribute 'ldInst' */
	ERROR_SMV_ATTR_MISSING_LDINST,
	/* SMV is missing required attribute 'cbName' */
	ERROR_SMV_ATTR_MISSING_CBNAME,
	/* SMV is missing Address definition */
	ERROR_SMV_MISSING_ADDRESS,
	/* SubNetwork is missing attribute 'name' */
	ERROR_SUBNETWORK_ATTR_MISSING_NAME,
	/* AcessPoint is missing required attribute 'name' */
	ERROR_ACCESSPOINT_ATTR_MISSING_NAME,
	/* AccessPoint has no Server defined */
	ERROR_ACCESSPOINT_MISSING_SERVER,
	/* No DataTypeTemplates section found in SCL file */
	ERROR_TEMPLATE_MISSING_TEMPLATE,
	/* DataTypeTemplates contains multiple LNodeType elements */
	ERROR_TEMPLATE_LNODETYPE_REPEATED,
	/* DOType contains multiple DAType elemets */
	ERROR_TEMPLATE_DATYPE_REPEATED,
	/* DataTypeTemplates contains multiple EnumType elements */
	ERROR_TEMPLATE_ENUM_TYPE_REPEATED,
	/* Missing type definition for enumerated data attribute */
	ERROR_TEMPLATE_MISSING_TYPE_DEFINITION,
	/* Wrong type definition for enumerate data attribute */
	ERROR_TEMPLATE_WRONG_TYPE_DEFINITION,
	/* Missing type definition for constructed data attribute */
	ERROR_TEMPLATE_MISSING_CONSTRUCTED_DEFINITION,
	/* Wrong type definition for constructed data attribute */
	ERROR_TEMPLATE_WRONG_CONSTRUCTED_DEFINITION,
	/* DO definition is missing */
	ERROR_TEMPLATE_MISSING_DOTYPE,
	/* DA definition is missing */
	ERROR_TEMPLATE_MISSING_DATYPE,
	/* DO is repeated */
	ERROR_TEMPLATE_DOTYPE_REPEATED,
	/* LNodeType is missing required attribute 'lnClass' */
	ERROR_TEMPLATE_MISSING_LNCLASS,
	/* cdc is missing */
	ERROR_TEMPLATE_MISSING_CDC,
	/* *
	 * 1. DO type contains multiple DA elements 
	 * 2. DO type contains multiple DA & SDO elments which has the same name 
	 * */
	ERROR_TEMPLATE_DOTYPE_ELEMENT_REPEATED,
	/* DOType is missing required attribute 'type' */
	ERROR_TEMPLATE_DOTYPE_ATTR_MISSING_TYPE,
	/* DOType is missing required attribute 'name' */
	ERROR_TEMPLATE_DOTYPE_ATTR_MISSING_NAME,
	/* id is missing of LNodeType, DOType, DAType & EnumType */
	ERROR_TEMPLATE_MISSING_ID,
	/* DAType is missing required attribute 'name' */
	ERROR_TEMPLATE_DATYPE_ATTR_MISSING_NAME,
	/* DAType is missing required attribute 'bType' */
	ERROR_TEMPLATE_DATYPE_ATTR_MISSING_BTYPE,
	/* DA type definition contains more than one BDA elements */
	ERROR_TEMPLATE_DATYPE_BDA_REPEATED,
	/* DataSet missing required attribute 'name' */
	ERROR_DS_ATTR_MISSING_NAME,
	/* Unsupported GSEControl type */
	ERROR_GSECTRL_UNSUPPORTED_TYPE,
	/* no AccessPoint defined in IED */
	ERROR_IED_MISSING_ACCESSPOINT,
	/* LogControl is missing required attribute 'name' */
	ERROR_LOGCTRL_ATTR_MISSING_NAME,
	/* LogControl is missing required attribute 'logName' */
	ERROR_LOGCTRL_ATTR_MISSING_LOGNAME,
	/* LDevice missing required attribute 'inst' */
	ERROR_LD_ATTR_MISSING_INST,
	/* LDevice misses LN0 */
	ERROR_LD_MSSING_LN0,
	/* Logical node is missing required attribute 'lnClass' */
	ERROR_LN_ATTR_MISSING_LNCLASS,
	/* Logical node is missing required attribute 'lnType' */
	ERROR_LN_ATTR_MISSING_LNTYPE,
	/* Logical node is missing required attribute 'inst' */
	ERROR_LN_ATTR_MISSING_INST,
	/* LN other than LN0 is not allowed to contain SettingControl */
	ERROR_LN_SG_IS_NOT_ALLOWED,
	/* LN contains more than one SettingControl */
	ERROR_LN_SG_REPEATED,
	/* ReportControlBlock is missing required attribute 'confRev' */
	ERROR_RPTCTRL_ATTR_MISSING_CONFREF,
	/* RptEnabled.max != 1 is not allowed when indexed='false' */
	ERROR_RPTCTRL_ATTR_MAX_INVALID_VALUE,
	/* No logical devices defined for AccessPoint */
	ERROR_SERVER_MISSING_ACCESSPOINT,
	/* Invalid value for BOOLEAN */
	ERROR_BOOLEAN_INVALID_VALUE

};

extern char *err_msg[];

#endif
