
// ImageLabelDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "LabelXMLFile.h"
#include "CColorButton.h"
#include "ImageLabelFile.h"
#include "opencv2/opencv.hpp"
#include <list>
#include <map>
using namespace std;
// CImageLabelDlg 对话框
class CImageLabelDlg : public CDialogEx
{
// 构造
public:
	CImageLabelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGELABEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void OnPressDelLabel();
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
	afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	afx_msg LONG OnCloseUp(UINT lParam, LONG wParam);
	afx_msg LONG OnDropDown(UINT lParam, LONG wParam);
	afx_msg void OnCustomdrawListShow(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnRefreshListShowCtrl();
	DECLARE_MESSAGE_MAP()
	void InitImageFileTreeView(HTREEITEM hparent);
	CString GetImageFileTreePath(HTREEITEM htreeItem,BOOL rootNeeded);
	CString GetPoseRadioString(int value);
	CString GetShotRadioString(int value);
	void OnRefreshLabelTreeViewCtrl(HTREEITEM hTreeItem, LabelXMLFileNode node);
	BOOL ShowImage(CDC* pDC, CString strPath, int x, int y);
public:
	
private:
	CTabCtrl m_tab_label;
	CTreeCtrl m_tre_file;
	CTreeCtrl m_tre_label;
	CListCtrl m_lst_label;
	LabelXMLFileNode m_labelXMLFileNode;
	map<HTREEITEM, CString> m_map_tre_path;
	CPoint m_point_bg;
	CPoint m_point_ed;
	COLORREF m_drawColor;
	CRect m_rect_draw;
	CRect m_rect_pic;
	CRect m_rect_cur_showImg;
	list<CRect> m_labelRectList;
	bool m_bIsMouseDown;
	bool m_bIsPicShow;
	HACCEL m_hAcc;
public:
	afx_msg void OnPressImportImgDir();
	afx_msg void OnPressExportVOCXML();
	afx_msg void OnPressImportLabel();
	afx_msg void OnTcnSelchangeLabelTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreLabel(NMHDR *pNMHDR, LRESULT *pResult);
	TCITEMW CImageLabelDlg::GetTabSelectItem(CTabCtrl& tabCtrl, TCHAR *buffer);
protected:
	CStatic m_picture;
	cv::Mat  m_imageMat;
public:
	CStatic m_rectangle;
	CColorButton m_volColor;
protected:
	CButton m_bt_add;
public:
	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnNMRClickLstShow(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	int m_radio_pose;
public:
	BOOL m_bTruncated;
	BOOL m_bDifficult;
	afx_msg void OnNMRClickTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPressDelFile();
	afx_msg void OnNMCustomdrawTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTreeLabel(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	int m_radio_shot;
public:
	afx_msg void OnPressExportNIMIObjectDetectionXML();
	afx_msg void OnLvnItemchangedLstShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPressModifyLabel();
	afx_msg void OnSaveLabelInfoXML();
};
