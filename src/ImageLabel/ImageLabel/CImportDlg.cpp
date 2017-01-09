// DlgImportVideo.cpp : 导入文件对话框
//

#include "stdafx.h"
#include "CImportDlg.h"
#include "afxdialogex.h"
#include <Shlwapi.h>

// CDlgImportVideo 对话框

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)


CImportDlg::CImportDlg(ImportDlgType type, CWnd* pParent)
	: CDialogEx(CImportDlg::IDD, pParent)
	, pathEdit(_T(""))
{
	this->type = type;
}
CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IVD_PATH_EDT, pathEdit);
	DDX_Control(pDX, IDC_IV_DLG_OPEN_BUT, openButton);
}

// CDlgImportVideo 消息处理程序
BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IV_DLG_OPEN_BUT, &CImportDlg::OnBnClickedOpenBut)
	ON_BN_CLICKED(IDOK, &CImportDlg::OnBnClickedOk)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CImportDlg::OnInitDialog()
{
	switch (type)
	{
	case TYPE_CIMPDIRDLG:
		this->SetWindowTextW(_T("选择文件夹"));
		break;
	case TYPE_CIMPFILEDLG:
		this->SetWindowTextW(_T("选择文件"));
		break;
	}
	return true;
}

//打开导入窗口：根据初始化得到的type生成对应的对话框
void CImportDlg::OnBnClickedOpenBut()
{
	// TODO:  在此添加控件通知处理程序代码
	
	switch (type)
	{
	case TYPE_CIMPDIRDLG:
	{
		CFolderPickerDialog folderDlg(NULL, 0, this, 0);
		if (folderDlg.DoModal() == IDOK)
		{
			CString tmpPath = folderDlg.GetPathName();
			pathEdit = tmpPath;
			this->UpdateData(FALSE);
		}
		break;
	}
	case TYPE_CIMPFILEDLG:
	{
		CFileDialog fileDlg(TRUE, NULL, NULL, NULL);
		if (fileDlg.DoModal() == IDOK)
		{
			CString tmpPath = fileDlg.GetPathName();
			pathEdit = tmpPath;
			this->UpdateData(FALSE);
		}
		break;
	}
	}
	
}

void CImportDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	pathName = pathEdit;

	CDialogEx::OnOK();
}
CString CImportDlg::GetPathName()
{
	return pathName;
}

