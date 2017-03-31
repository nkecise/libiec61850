/*
 * File: $(model_generator_cxx)/AttributeType.h
 */
#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

enum
{
	BOOLEAN = 0, /* int */
    INT8 = 1, /* int8_t */
    INT16 = 2, /* int16_t */
    INT32 = 3, /* int32_t */
    INT64 = 4, /* int64_t */
    INT128 = 5, 
    INT8U = 6, /* uint8_t */
    INT16U = 7, /* uint16_t */
    INT24U = 8, /* uint32_t */
    INT32U = 9, /* uint32_t */
    FLOAT32 = 10, /* float */
    FLOAT64 = 11, /* double */
    ENUMERATED = 12, 
    OCTET_STRING_64 = 13, 
    OCTET_STRING_6 = 14, 
    OCTET_STRING_8 = 15, 
    VISIBLE_STRING_32 = 16, 
    VISIBLE_STRING_64 = 17, 
    VISIBLE_STRING_65 = 18, 
    VISIBLE_STRING_129 = 19, 
    VISIBLE_STRING_255 = 20, 
    UNICODE_STRING_255 = 21, 
    TIMESTAMP = 22, 
    QUALITY = 23, 
    CHECK = 24, 
    CODEDENUM = 25, 
    GENERIC_BITSTRING = 26, 
    CONSTRUCTED = 27, 
    ENTRY_TIME = 28, 
    PHYCOMADDR = 29
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
