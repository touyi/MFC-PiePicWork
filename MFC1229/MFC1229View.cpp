
// MFC1229View.cpp : CMFC1229View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
END_MESSAGE_MAP()

// CMFC1229View ����/����

CMFC1229View::CMFC1229View()
{
	// TODO: �ڴ˴���ӹ������
	TManager::Get()->RegistFunc(std::bind(&CMFC1229View::ReDraw, this, std::placeholders::_1, std::placeholders::_2));
}

CMFC1229View::~CMFC1229View()
{
}

BOOL CMFC1229View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC1229View ����

void CMFC1229View::OnDraw(CDC* pDC)
{
	CMFC1229Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// MessageBox(pDoc->GetTitle());
	// TManager::Get()->SetNowPie(pDoc->GetTitle());
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	// ˫�����ͼ
	CRect rc;
	GetClientRect(rc);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(rc, RGB(255, 255, 255));
	// ��������
	CRect showRc = rc;
	showRc.left = 20;
	showRc.top = 20;
	auto pie = TManager::Get()->GetPieByName(pDoc->GetTitle());
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


// CMFC1229View ���

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
	if (type == TMsgType::ReDrawPie)
	{
		Invalidate();
	}
}

CMFC1229Doc* CMFC1229View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC1229Doc)));
	return (CMFC1229Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC1229View ��Ϣ�������


//void CMFC1229View::OnChildActivate()
//{
//	CView::OnChildActivate();
//	CMFC1229Doc* pDoc = GetDocument();
//	MessageBox(pDoc->GetTitle());
//	// TODO: �ڴ˴������Ϣ����������
//}



//void CMFC1229View::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
//{
//	CView::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
//
//	// TODO: �ڴ˴������Ϣ����������
//	
//}


//void CMFC1229View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
//{
//	// TODO: �ڴ����ר�ô����/����û���
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
		TManager::Get()->SetNowPie(doc->GetTitle());
	}
	// TODO: �ڴ˴������Ϣ����������
}
