/*
 * File: Error.cpp
 */
#include "Error.h"

char *err_msg[] = {
	"Operation Successfully",
	"Document has no SCL section defined",
	"Document has no DataTypeTemplates section defined",
	"Document has no Communication section defined",
	"Document has more than one SCL section defined",
	"no subnetworks defined"
};

char *chk_attr[] = {
	"name",
	"id,lnClass",
	"id,cdc",
	"name,bType,fc",
	"id",
	"name,type"
};
