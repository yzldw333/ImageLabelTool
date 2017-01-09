
// EventController.h
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "LabelXMLFile.h"

// CImageLabelApp: 
// 有关此类的实现，请参阅 ImageLabel.cpp
//

class CEventController
{

public:
	
	static CEventController* GetInstance();
	int OnImportLabelFile(LabelXMLFileNode &label);
	CString OnImportImageDirectory();
	BOOL OnAddLabel(CString fileName, CString domain, CString type, CString labelName, CRect drawrect, CRect winRect, CRect picRect,int depth, CString shot,COLORREF color, BOOL bTruncated, BOOL bDifficult, CString pose);
	void OnDeleteImage();
	void OnDelLabel(CString fileName, int index);
	void OnModifyLabel(CString fileName, int index, CString domain, CString type, CString labelName, CString shot, BOOL bTruncated, BOOL bDifficult, CString pose);
	int OnExportVOCXML();
	int OnExportNIMIDetectionXML();
	
private:
	CEventController(){}
	static CEventController* _instance;
	class Garbor{
	public:
		Garbor(){
			printf("Garbor constructed!");
		}
		~Garbor(){
			printf("Garbor destruct!");
			if (CEventController::_instance)
				delete CEventController::_instance;
		};
	};
	static Garbor garbor;
	
};
