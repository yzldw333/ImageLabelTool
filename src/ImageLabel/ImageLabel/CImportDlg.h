#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgImportVideo 对话框
typedef int ImportDlgType;
#define TYPE_CIMPFILEDLG 0
#define TYPE_CIMPDIRDLG 1
class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(ImportDlgType type = TYPE_CIMPDIRDLG, CWnd* pParent = NULL);
	virtual ~CImportDlg();

// 对话框数据
	enum { IDD = IDD_IMPORT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	CString pathEdit;
	CButton openButton;
	CString pathName;
	ImportDlgType type;
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOpenBut();
	afx_msg void OnBnClickedOk();
	CString GetPathName();
};
