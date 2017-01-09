// 根据xml文件读取标签信息
#include "stdafx.h"
#include "CLabelLoading.h"
#include "TinyXML/tinystr.h"
#include "TinyXML/tinyxml.h"
#include <string>
#include <list>
using namespace std;
CString UTF8_To_UTF16(const char* source)
{
	unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, source, -1, NULL, NULL);
	if (len == 0)
		return CString(_T(""));
	wchar_t *buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, source, -1, buffer, len);

	CString dest(buffer);
	delete[] buffer;
	return dest;
}
void GetLabels(const char * xmlPath,Label** outLabels,int &groupNum)
{
	TiXmlDocument doc(xmlPath);
	bool bSuccess = doc.LoadFile(TIXML_ENCODING_UTF8);
	if (!bSuccess) {
		TRACE("Could not load test file %s. Error='%s'. Exiting.\n", xmlPath, doc.ErrorDesc());
		exit(1);
	}
	
	TiXmlElement* root = doc.RootElement();
	TiXmlElement* item = root->FirstChildElement("group");
	int groupLen = 0;

	/************************************************************************/
	/*                       获得Group数量                                  */
	/************************************************************************/
	while (item!=NULL)
	{
		groupLen++;
		item = item->NextSiblingElement("group");
	}


	/************************************************************************/
	/*                       遍历XML存入对应Label数据结构                   */
	/************************************************************************/
	Label* labels = new Label[groupLen];
	item = root->FirstChildElement("group");
	int i = 0;
	while (item != NULL)
	{
		labels[i].name = UTF8_To_UTF16(item->Attribute("name"));
		TiXmlElement* cateItem = item->FirstChildElement("class");
		while (cateItem!= NULL)
		{
			Category category;
			category.name = UTF8_To_UTF16(cateItem->Attribute("name"));
			TiXmlElement * node = cateItem->FirstChildElement("name");
			while (node != NULL)
			{
				category.strList.push_back(UTF8_To_UTF16(node->GetText()));
				node = node->NextSiblingElement("name");
			}
			labels[i].categoryList.push_back(category);
			cateItem = cateItem->NextSiblingElement("class");
		}
		item = item->NextSiblingElement("group");
		i++;
	}
	*outLabels = labels;
	groupNum = groupLen;
}
void EraseLabel(Label* label)
{
	list<Category>::iterator it;
	for (it = label->categoryList.begin(); it != label->categoryList.end(); it++)
	{
		(*it).strList.clear();
	}
	label->categoryList.clear();
}