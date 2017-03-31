/*
 * File: $(model_generator_cxx)/Utils.cpp
 */
#include "SclParser.h"

char *CSCLParser::Convert2MmsString(char *str, int len)
{
	for(int i = 0; i < len; i++)
	{
		if(str[i] == '.')
			str[i] = '$';
	}

	return(str);
}
