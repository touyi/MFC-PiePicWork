#pragma once
#include"Pie.h"

// CItemInfo �Ի���

class CItemInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CItemInfoDlg)

public:
	CItemInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CItemInfoDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEM_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_color;
	CString m_name;
	double m_count;
	CString m_legend;
	CRect m_color_rect;
	CString m_caption;
	afx_msg void OnChooseLegendBtnClicked();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
