/*
 * File: ModelHandler.cpp
 */
#include "MainFrame.h"

const char *DAType[] = {
	"BOOLEAN", "INT8", "INT16", "INT32", "INT64", "INT128",
	"INT8U", "INT16U", "INT24U", "INT32U", "FLOAT32", "FLOAT64",
	"ENUMERATED", "OCTET_STRING_64", "OCTET_STRING6", "OCTET_STRING_8",
	"VISIBLE_STRING_32", "VISIBLE_STRING_64", "VISIBLE_STRING_65", "VISIBLE_STRING_129",
	"VISIBLE_STRING_255", "UNICODE_STRING_255", "TIMESTAMP", "QUALITY", "CHECK",
	"CODEDENUM", "GENERIC_BITSTRING", "CONSTRUCTED", "ENTRY_TIME", "PHYCOMADDR"
};

const char *FC[] = {
	"ST", "MX", "SP", "SV", "CF", "DC", "SG", "SE", "SR", "OR", "BL",
	"EX", "CO", "US", "MS", "RP", "BR", "LG"
};

void MainWindow::enumServerModel()
{
	if(model->firstChild == NULL)
		return;
	fmod = fopen("model.txt", "w");
	LogicalDevice *ld = model->firstChild;
	while(ld)
	{
		enumLD(ld);
		ld = (LogicalDevice *)ld->sibling;
	};
	fclose(fmod);

	return;
}

void MainWindow::enumLD(LogicalDevice *ld)
{
	fprintf(fmod, "%s\n", ld->name);
	//
	ModelNode *node = ld->firstChild;
	while(node)
	{
		ModelNodeType type = node->modelType;
		if(type != LogicalNodeModelType)
			continue;
		enumLN(node);
		node = node->sibling;
	};

	return;
}

void MainWindow::enumLN(ModelNode *parent)
{
	fprintf(fmod, "\t%s\n", parent->name);
	//
	char prefix[128];
	ModelNode *node = parent->firstChild;

	while(node)
	{
		ModelNodeType type = node->modelType;
		strcpy(prefix, "\t\t");
		enumDO(node, prefix);
		node = node->sibling;
	}

	return;
}

void MainWindow::enumDO(ModelNode *parent, char *prefix)
{
	fprintf(fmod, "%s%s\n", prefix, parent->name);
	//
	ModelNode *node = parent->firstChild;

	while(node)
	{
		ModelNodeType type = node->modelType;
		if(type == DataObjectModelType)
		{
			strcat(prefix, "\t");
			enumDO(node, prefix);
			strcpy(prefix, "\t\t");
		}
		else if(type == DataAttributeModelType)
		{
			strcpy(prefix, "\t\t\t");
			enumDA(node, prefix);
		}
		node = node->sibling;
	};

	return;
}

void MainWindow::enumDA(ModelNode *parent, char *prefix)
{
	char mypre[128];
	DataAttribute *da = (DataAttribute *)parent;
	fprintf(fmod, "%s%s,%s,%s\n", prefix, da->name, FC[da->fc], DAType[da->type]);
	//
	ModelNode *node = parent->firstChild;

	while(node)
	{
		strcpy(mypre, prefix);
		strcat(prefix, "\t");
		enumDA(node, prefix);
		strcpy(prefix, mypre);
		node = node->sibling;
	}

	return;
}
