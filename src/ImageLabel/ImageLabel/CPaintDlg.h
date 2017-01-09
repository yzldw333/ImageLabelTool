#pragma once
#include "Resource.h"
#include "CVideoLabelImageFile.h"
#include <vector>

using namespace std;
// CPaintDlg 对话框
const COLORREF rectColor[20] = { 0x00FFCC,0xFFFFCC,0x0066CC,0x9966CC,0xFF66CC,
							0xFF6633,0xFF66FF,0xFF3366,0xFF0000,0xFF00FF,
							0x6600FF,0xFF99FF,0x6699FF,0x009999,0x999999,
							0xFF9999,0x00FF00,0xFFFF00,0x000033,0xCCFFCC};
class CPaintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaintDlg)

public:
	CPaintDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaintDlg();
	CPoint m_pointA;
	CPoint m_pointB;
	CRect m_newRect;
	CSize m_videoSize;
	CImg m_tmpImg;		//临时标注信息
	bool isMouseDown;
	void SetPoint(CPoint point);
	void SetPoint(long x, long y);
	FRect GetLabelFRect(CRect rect);
	CRect GetLabelCRect(FRect frect);
	CToolTipCtrl m_toolTip;
// 对话框数据
	enum { IDD = IDD_PAINT_DLG };
	void OnMouseMove(CPoint& screenPoint);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	inline void DrawLabel();
	virtual void OnPaint();
	virtual BOOL OnInitDialog(); 
	
	DECLARE_MESSAGE_MAP()

};
