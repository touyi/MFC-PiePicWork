
// MFC1229View.h : CMFC1229View 类的接口
//

#pragma once
#include"Pie.h"
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

	map<CString, CPie*> m_pies;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC1229View.cpp 中的调试版本
inline CMFC1229Doc* CMFC1229View::GetDocument() const
   { return reinterpret_cast<CMFC1229Doc*>(m_pDocument); }
#endif

