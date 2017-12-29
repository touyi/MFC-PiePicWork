
// MFC1229View.cpp : CMFC1229View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFC1229.h"
#endif
#include"Item.h"
#include "MFC1229Doc.h"
#include "MFC1229View.h"
#include"Pie.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC1229View

IMPLEMENT_DYNCREATE(CMFC1229View, CView)

BEGIN_MESSAGE_MAP(CMFC1229View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC1229View 构造/析构

CMFC1229View::CMFC1229View()
{
	// TODO: 在此处添加构造代码
	// TEST
	m_pies[CString("666")] = new CPie(CRect(), CString("这是一个牛皮的饼图"));
	auto pp = m_pies[CString("666")];
	pp->InsertItem(CString("类型66"), 200);
	pp->InsertItem(CString("类型2"), 120);
	pp->InsertItem(CString("类型3"), 140);
	pp->InsertItem(CString("类型4"), 160);
}

CMFC1229View::~CMFC1229View()
{
}

BOOL CMFC1229View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFC1229View 绘制

void CMFC1229View::OnDraw(CDC* pDC)
{
	CMFC1229Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rc;
	GetClientRect(rc);
	for (auto i = m_pies.begin(); i != m_pies.end(); i++)
	{
		i->second->SetDrawArea(rc);
		i->second->Draw(pDC);
	}
	
}

void CMFC1229View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC1229View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC1229View 诊断

#ifdef _DEBUG
void CMFC1229View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC1229View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC1229Doc* CMFC1229View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC1229Doc)));
	return (CMFC1229Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC1229View 消息处理程序
