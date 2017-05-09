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
	ERROR_MISSING_TEMPLATE,
	/* Document has no Communication section defined */
	ERROR_MISSING_COMMUNICATION, 
	/* Document contains more than one SCL section */
	ERROR_SCL_REPEATED,
	/* no subnetworks defined */
	ERROR_MISSING_SUBNETWORKS
};

enum SCL_NODE
{
	SCL_NODE_IED = 0,
	SCL_NODE_LNODE_TYPE,
	SCL_NODE_DO_TYPE,
	SCL_NODE_DA_TYPE,
	SCL_NODE_ENUM_TYPE,
	SCL_NODE_DO
};

extern char *err_msg[];
extern char *chk_attr[];

#endif
