#ifndef CLABELLOADING_H
#define CLABELLOADING_H
#include <list>
using namespace std;
/********标签类别：如动作、球员****/
typedef struct Category
{
	CString name;
	list<CString> strList;
}Category;




/********标签组：如足球、篮球******/
typedef struct Group
{
	CString name;
	list<Category> categoryList;
	int count;
}Label;
void GetLabels(const char * xmlPath, Label** outLabels, int &groupNum);
void EraseLabel(Label* label);

#endif