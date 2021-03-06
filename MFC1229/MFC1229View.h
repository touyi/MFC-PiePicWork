
// MFC1229View.h : CMFC1229View 类的接口
//

#pragma once
#include"TManager.h"
#include<map>
using std::map;
class CMFC1229View : public CView
{
protected: // 仅从序列化创建
	CMFC1229View();
	DECLARE_DYNCREATE(CMFC1229View)

// 特性
public:
	CMFC1229Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMFC1229View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ReDraw(void*, TMsgType);
// 生成的消息映射函数
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
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	virtual void OnInitialUpdate();
//	afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
//	afx_msg void OnClose();
//	virtual BOOL DestroyWindow();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

#ifndef _DEBUG  // MFC1229View.cpp 中的调试版本
inline CMFC1229Doc* CMFC1229View::GetDocument() const
   { return reinterpret_cast<CMFC1229Doc*>(m_pDocument); }
#endif

