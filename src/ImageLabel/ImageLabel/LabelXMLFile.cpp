#include "stdafx.h"
#include "LabelXMLFile.h"
#include "common.hpp"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include  <afxpriv.h>
LabelXMLFileNode CLabelXMLFileIOController::GetLabelXMLFileNode(CString fileName)
{
	USES_CONVERSION;
	char * xmlPath = T2A(fileName);
	TiXmlDocument doc(xmlPath);
	LabelXMLFileNode node;
	bool bSuccess = doc.LoadFile(TIXML_ENCODING_UTF8);
	if (!bSuccess) {
		TRACE("Could not load test file %s. Error='%s'. Exiting.\n", xmlPath, doc.ErrorDesc());
		exit(1);
	}

	TiXmlElement* root = doc.RootElement();
	if (root != NULL)
	{
		node = OnGetXMLFileNode(root);
	}
	return node;
}
LabelXMLFileNode CLabelXMLFileIOController::OnGetXMLFileNode(TiXmlElement *item)
{
	LabelXMLFileNode node;
	node.hasChildren = 0;
	node.value = UTF8_To_UTF16(item->Attribute("name"));
	if (node.value == "")
		node.value = UTF8_To_UTF16(item->GetText());
	item = item->FirstChildElement();
	while (item != NULL)
	{
		node.hasChildren = 1;
		LabelXMLFileNode newNode = OnGetXMLFileNode(item);
		node.childrenList.push_back(newNode);
		item = item->NextSiblingElement();
	}
	return node;
}