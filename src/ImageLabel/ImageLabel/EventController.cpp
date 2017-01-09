#include "stdafx.h"
#include "EventController.h"
#include "LabelXMLFile.h"
#include "ImageLabelFile.h"
#include "CImportDlg.h"
#include "Resource.h"
CEventController* CEventController::_instance = NULL;
CEventController::Garbor CEventController::garbor;
CEventController* CEventController::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new CEventController();
	}
	return _instance;
}
CString CEventController::OnImportImageDirectory()
{
	CImportDlg dlg(TYPE_CIMPDIRDLG);
	if (dlg.DoModal() == IDOK)
	{
		return dlg.GetPathName();
	}
	else
	{
		return NULL;
	}
}
int CEventController::OnImportLabelFile(LabelXMLFileNode &label)
{
	CImportDlg dlg(TYPE_CIMPFILEDLG);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();
		if (!PathFileExistsW(pathName))
		{
			AfxMessageBox(_T("路径有误！"));
			return -1;
		}
		label = CLabelXMLFileIOController::GetLabelXMLFileNode(pathName);
	}
	else
	{
		return -1;
	}
	return 0;
}
int CEventController::OnExportVOCXML()
{
	CImportDlg dlg(TYPE_CIMPDIRDLG);
	CString pathName;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
		if (!PathFileExistsW(pathName))
		{
			AfxMessageBox(_T("路径有误！"));
			return -1;
		}
	}
	else
	{
		return -1;
	}
	CImageLabelFileIOController::GetInstance()->SaveFileToVOCXML(pathName);
}
int CEventController::OnExportNIMIDetectionXML()
{
	CImportDlg dlg(TYPE_CIMPDIRDLG);
	CString pathName;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
		if (!PathFileExistsW(pathName))
		{
			AfxMessageBox(_T("路径有误！"));
			return -1;
		}
	}
	else
	{
		return -1;
	}
	CImageLabelFileIOController::GetInstance()->SaveFileToMINIObjectDetectionXML(pathName);
}
BOOL CEventController::OnAddLabel(CString fileName, CString domain, CString type, CString labelName, CRect drawrect,CRect winRect, CRect picRect,int depth,CString shot,COLORREF color,BOOL bTruncated,BOOL bDifficult,CString pose)
{
	CLabel label;
	label.frect = GetFrect(drawrect, winRect, picRect);
	label.domain = domain;
	label.type = type;
	label.color = color;
	label.name = labelName;
	label.bDifficult = bDifficult;
	label.bTruncated = bTruncated;
	label.pose = pose;
	CImageLabelFileIOController::GetInstance()->AddImageLabel(fileName,picRect,depth,shot,label);
	CImageLabelFileIOController::GetInstance()->SaveFileToXML();
	return TRUE;
}
void CEventController::OnDelLabel(CString fileName, int index)
{
	CImageLabelFileIOController::GetInstance()->DeleteImageLabel(fileName, index);
	CImageLabelFileIOController::GetInstance()->SaveFileToXML();
}
void CEventController::OnModifyLabel(CString fileName, int index, CString domain, CString type, CString labelName, CString shot, BOOL bTruncated, BOOL bDifficult, CString pose)
{
	CLabel label;
	label.domain = domain;
	label.type = type;
	label.name = labelName;
	label.bTruncated = bTruncated;
	label.bDifficult = bDifficult;
	label.pose = pose;
	CImageLabelFileIOController::GetInstance()->ModifyImageLabel(fileName, index, shot, label);
	CImageLabelFileIOController::GetInstance()->SaveFileToXML();
}