
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
#include"TManager.h"

// CMFC1229View

IMPLEMENT_DYNCREATE(CMFC1229View, CView)

BEGIN_MESSAGE_MAP(CMFC1229View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_CHILDACTIVATE()
//ON_WM_SETFOCUS()
//ON_WM_KILLFOCUS()
//ON_WM_MDIACTIVATE()
ON_WM_SETFOCUS()
//ON_COMMAND(ID_LIST_NEW, &CMFC1229View::OnListNew)
//ON_COMMAND(ID_LIST_DELETE, &CMFC1229View::OnListDelete)
//ON_COMMAND(ID_LIST_CHANGE, &CMFC1229View::OnListChange)
//ON_WM_ACTIVATE()
//ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMFC1229View 构造/析构

CMFC1229View::CMFC1229View()
{
	// TODO: 在此处添加构造代码
	TIns->RegistFunc(std::bind(&CMFC1229View::ReDraw, this, std::placeholders::_1, std::placeholders::_2),_T("view"));
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
	// MessageBox(pDoc->GetTitle());
	// TIns->SetNowPie(pDoc->GetTitle());
	// TODO: 在此处为本机数据添加绘制代码
	// 双缓冲绘图
	CRect rc;
	GetClientRect(rc);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(rc, RGB(255, 255, 255));
	// 绘制区域
	CRect showRc = rc;
	showRc.left = 20;
	showRc.top = 20;
	auto pie = TIns->GetPieByName(pDoc->GetTitle());
	if (pie != NULL)
	{
		pie->SetDrawArea(showRc);
		pie->Draw(&memDC);
	}
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	bitmap.DeleteObject();
	memDC.DeleteDC();
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

void CMFC1229View::ReDraw(void *, TMsgType type)
{
	CMFC1229Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (type == TMsgType::ReDrawPie || TMsgType::UpdateListAndPie)
	{
		Invalidate();
	}
}

CMFC1229Doc* CMFC1229View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC1229Doc)));
	return (CMFC1229Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC1229View 消息处理程序


//void CMFC1229View::OnChildActivate()
//{
//	CView::OnChildActivate();
//	CMFC1229Doc* pDoc = GetDocument();
//	MessageBox(pDoc->GetTitle());
//	// TODO: 在此处添加消息处理程序代码
//}



//void CMFC1229View::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
//{
//	CView::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
//
//	// TODO: 在此处添加消息处理程序代码
//	
//}


//void CMFC1229View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
//}


void CMFC1229View::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	CString str1;
	auto doc = GetDocument();
	if (doc != NULL)
	{
		TIns->SetNowPie(doc->GetTitle());
	}
	// TODO: 在此处添加消息处理程序代码
}


//void CMFC1229View::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CView::OnActivate(nState, pWndOther, bMinimized);
//	
//	// TODO: 在此处添加消息处理程序代码
//}


//BOOL CMFC1229View::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	BOOL flag = CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//	TIns->RegistFunc(std::bind(&CMFC1229View::ReDraw, this, std::placeholders::_1, std::placeholders::_2));
//	return flag;
//}


//void CMFC1229View::OnInitialUpdate()
//{
//	CView::OnInitialUpdate();
//	TIns->RegistFunc(std::bind(&CMFC1229View::ReDraw, this, std::placeholders::_1, std::placeholders::_2),GetDocument()->GetTitle());
//	// TODO: 在此添加专用代码和/或调用基类
//}


//void CMFC1229View::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	TIns->UnRegistFunc(GetDocument()->GetTitle());
//	CView::OnClose();
//}

