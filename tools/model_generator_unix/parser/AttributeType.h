/*
 * File: $(model_generator_cxx)/AttributeType.h
 */
#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

enum
{
	IEC61850_BOOLEAN = 0, /* int */
    IEC61850_INT8 = 1, /* int8_t */
    IEC61850_INT16 = 2, /* int16_t */
    IEC61850_INT32 = 3, /* int32_t */
    IEC61850_INT64 = 4, /* int64_t */
    IEC61850_INT128 = 5, 
    IEC61850_INT8U = 6, /* uint8_t */
    IEC61850_INT16U = 7, /* uint16_t */
    IEC61850_INT24U = 8, /* uint32_t */
    IEC61850_INT32U = 9, /* uint32_t */
    IEC61850_FLOAT32 = 10, /* float */
    IEC61850_FLOAT64 = 11, /* double */
    IEC61850_ENUMERATED = 12, 
    IEC61850_OCTET_STRING_64 = 13, 
    IEC61850_OCTET_STRING_6 = 14, 
    IEC61850_OCTET_STRING_8 = 15, 
    IEC61850_VISIBLE_STRING_32 = 16, 
    IEC61850_VISIBLE_STRING_64 = 17, 
    IEC61850_VISIBLE_STRING_65 = 18, 
    IEC61850_VISIBLE_STRING_129 = 19, 
    IEC61850_VISIBLE_STRING_255 = 20, 
    IEC61850_UNICODE_STRING_255 = 21, 
    IEC61850_TIMESTAMP = 22, 
    IEC61850_QUALITY = 23, 
    IEC61850_CHECK = 24, 
    IEC61850_CODEDENUM = 25, 
    IEC61850_GENERIC_BITSTRING = 26, 
    IEC61850_CONSTRUCTED = 27, 
    IEC61850_ENTRY_TIME = 28, 
    IEC61850_PHYCOMADDR = 29
};

enum
{
	ST = (0), /** Status information */
	MX = (1), /** Measurands - analogue values */
	SP = (2), /** Setpoint */
	SV = (3), /** Substitution */
	CF = (4), /** Configuration */
	DC = (5), /** Description */
	SG = (6), /** Setting group */
	SE = (7), /** Setting group editable */
	SR = (8), /** Service response / Service tracking */
	OR = (9), /** Operate received */
	BL = (10), /** Blocking */
	EX = (11), /** Extended definition */
	CO = (12), /** Control */
	ALL = (99), /** All FCs */
	NONE = (-1) /** not specified */
};

#endif
