/*
 * File: DynamicModelGenerator.h
 */
#ifndef LIB_IEC61850_DYNAMIC_MODEL_GENERATOR_H
#define LIB_IEC61850_DYNAMIC_MODEL_GENERATOR_H

#include <string>

int IsScl(const char *fn);
int GenerateDynamicConfig(
		const char *pSclName, const char *pIedName,
		const char *pApName, std::string& ctx
		);

#endif
