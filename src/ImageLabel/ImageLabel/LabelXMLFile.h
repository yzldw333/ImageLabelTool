#pragma once
#include "stdafx.h"
#include <list>
#include "TinyXML/tinyxml.h"

using namespace std;
typedef struct LabelXMLFileNode{
	CString value;
	int hasChildren;
	list<struct LabelXMLFileNode>childrenList;
}LabelXMLFileNode;
class CLabelXMLFileIOController
{
public:
	static LabelXMLFileNode GetLabelXMLFileNode(CString fileName);
	static LabelXMLFileNode OnGetXMLFileNode(TiXmlElement *item);
};