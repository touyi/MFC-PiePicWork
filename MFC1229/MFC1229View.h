
// MFC1229View.h : CMFC1229View ��Ľӿ�
//

#pragma once
#include"Pie.h"
#include<map>
using std::map;
class CMFC1229View : public CView
{
protected: // �������л�����
	CMFC1229View();
	DECLARE_DYNCREATE(CMFC1229View)

// ����
public:
	CMFC1229Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CMFC1229View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnChildActivate();
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // MFC1229View.cpp �еĵ��԰汾
inline CMFC1229Doc* CMFC1229View::GetDocument() const
   { return reinterpret_cast<CMFC1229Doc*>(m_pDocument); }
#endif

