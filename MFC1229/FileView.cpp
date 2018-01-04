
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "MFC1229.h"
#include "Item.h"
#include"ItemInfo.h"
#include"TitleChangeDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_nowPie = NULL;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(LVN_ITEMCHANGED,ID_MY_LISTCTRL,&CFileView::OnLVNChangedCtrlList)
	ON_NOTIFY(NM_RCLICK, ID_MY_LISTCTRL, &CFileView::OnNMRClickCtrlList)
	ON_COMMAND(ID_LIST_NEW, &CFileView::OnListNew)
	ON_COMMAND(ID_LIST_DELETE, &CFileView::OnListDelete)
	ON_COMMAND(ID_LIST_CHANGE, &CFileView::OnListChange)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | /*LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |*/ LVS_EX_CHECKBOXES;

	//m_wndListCtrl.Create(dwViewStyle, rectDummy, this, 4);
	if (!m_wndListCtrl.Create(dwViewStyle, rectDummy, this, ID_MY_LISTCTRL))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	 m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	 m_wndListCtrl.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillFileView();
	AdjustLayout();
	// ע����Ӧ�¼�
	TIns->RegistFunc(std::bind(&CFileView::TMsgFunction, this, std::placeholders::_1, std::placeholders::_2),_T("FileView"));
	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CFileView::OnShowControlBarMenu(CPoint point)
{
	return TRUE;
}

void CFileView::FillFileView()
{
	m_wndListCtrl.DeleteAllItems();
	if (m_nowPie == NULL)
	{
		return;
	}
	m_wndListCtrl.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndListCtrl.InsertColumn(0, _T(" "), LVCFMT_LEFT, 25);
	m_wndListCtrl.InsertColumn(1, _T("��Ŀ"), LVCFMT_LEFT, 80);
	m_wndListCtrl.InsertColumn(2, _T("���"), LVCFMT_LEFT, 100);
	auto data = m_nowPie->GetDatas();
	for (int i = 0; i < data->size(); i++)
	{
		auto& it = (*data)[i];

		m_wndListCtrl.InsertItem(i, _T(""));
		m_wndListCtrl.SetItemData(i, (DWORD_PTR)(&it)); 
		m_wndListCtrl.SetCheck(i, it.isActive);

		m_wndListCtrl.SetItemText(i, 1, it.m_name);
		CString num;
		num.Format(_T("%d"), it.m_count);
		m_wndListCtrl.SetItemText(i, 2, num);
	}
}

void CFileView::TMsgFunction(void * param, TMsgType type)
{
	switch (type)
	{
	case UpdateListAndPie:
	case FoucsPieChange:
		m_nowPie = static_cast<CPie*> (param);
		FillFileView();
		break;
	case DeleteNowPie:
		m_nowPie = NULL;
		FillFileView();
		break;
	default:
		break;
	}
}

//void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
//	ASSERT_VALID(pWndTree);
//
//	if (pWnd != pWndTree)
//	{
//		CDockablePane::OnContextMenu(pWnd, point);
//		return;
//	}
//
//	if (point != CPoint(-1, -1))
//	{
//		// ѡ���ѵ�������: 
//		CPoint ptTree = point;
//		pWndTree->ScreenToClient(&ptTree);
//
//		UINT flags = 0;
//		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
//		if (hTreeItem != NULL)
//		{
//			pWndTree->SelectItem(hTreeItem);
//		}
//	}
//
//	pWndTree->SetFocus();
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
//}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndListCtrl.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	
	if (TIns->m_nowPies == NULL)
	{
		return;
	}
	CTitleChangeDlg dlg;
	dlg.m_name = TIns->m_nowPies->GetTitle();
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_name != TIns->m_nowPies->GetTitle())
		{
			TIns->m_nowPies->SetTitle(dlg.m_name);
			TIns->CallFunc(NULL, TMsgType::ReDrawPie);
			CString msg = CString("�����޸�Ϊ��") + dlg.m_name;
			TIns->CallFunc(&msg, TMsgType::OutPutMessage);
		}
	}
}

void CFileView::OnFileOpen()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnFileOpenWith()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditClear()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndListCtrl.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndListCtrl.SetFocus();
}

void CFileView::OnLVNChangedCtrlList(NMHDR * pNMHDR, LRESULT * pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ѡ��״̬�ı�
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) 
		|| (pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) 
		)
	{
		auto data = reinterpret_cast<CItem<cchar>*>(pNMLV->lParam);
		bool checkState = 1 & m_wndListCtrl.GetCheck(pNMLV->iItem);
		if(data->isActive != checkState)
			TIns->SetPieItemActive(data->m_name, checkState);
	}
	*pResult = 0;
}

void CFileView::OnNMRClickCtrlList(NMHDR * pNMHDR, LRESULT * pResult)
{
	auto pos = m_wndListCtrl.GetFirstSelectedItemPosition();
	int iitem = m_wndListCtrl.GetNextSelectedItem(pos);
	//if (iitem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���  
		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		if (iitem == -1)
		{
			popup->EnableMenuItem(ID_LIST_DELETE, MF_BYCOMMAND|MF_GRAYED);
			popup->EnableMenuItem(ID_LIST_CHANGE, MF_BYCOMMAND|MF_GRAYED);
		}
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndListCtrl.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}



// ����Ԫ����
void CFileView::OnListNew()
{
	CItemInfoDlg itemDlg;
	srand((unsigned)time(NULL));
	itemDlg.m_color = RGB(rand() % 255, rand() % 255, rand() % 255);
	itemDlg.m_caption = _T("����");
	if (itemDlg.DoModal() == IDOK)
	{
		if (itemDlg.m_legend != _T(""))
			TIns->InsertItem(itemDlg.m_name, itemDlg.m_count, itemDlg.m_color,itemDlg.m_legend);
		else
			TIns->InsertItem(itemDlg.m_name, itemDlg.m_count, itemDlg.m_color);
		CString msg = CString("�����") + itemDlg.m_name;
		TIns->CallFunc(&msg, TMsgType::OutPutMessage);
	}
}


void CFileView::OnListDelete()
{
	// TODO: �ڴ���������������
	auto pos = m_wndListCtrl.GetFirstSelectedItemPosition();
	int iitem = m_wndListCtrl.GetNextSelectedItem(pos);
	if (iitem != -1)
	{
		auto it = (CItem<cchar>*)m_wndListCtrl.GetItemData(iitem);
		CString msg = CString("ɾ���") + it->m_name;
		TIns->CallFunc(&msg,TMsgType::OutPutMessage);
		m_wndListCtrl.DeleteItem(iitem);
		TIns->DeleteNowPieItem(it->m_name);
	}
}


void CFileView::OnListChange()
{
	// TODO: �ڴ���������������
	auto pos = m_wndListCtrl.GetFirstSelectedItemPosition();
	int iitem = m_wndListCtrl.GetNextSelectedItem(pos);
	if (iitem != -1)
	{
		auto it = (CItem<cchar>*)m_wndListCtrl.GetItemData(iitem);
		CItemInfoDlg itemDlg;
		itemDlg.m_name = it->m_name;
		itemDlg.m_color = it->m_color;
		itemDlg.m_count = it->m_count;
		itemDlg.m_legend = it->m_img;
		itemDlg.m_caption = _T("�޸�");
		if (itemDlg.DoModal() == IDOK)
		{
			if (itemDlg.m_name != it->m_name ||
				itemDlg.m_color != it->m_color ||
				itemDlg.m_count != it->m_count ||
				itemDlg.m_legend != it->m_img)
			{
				CString firmoney,secmoney,fircolor,seccolor;
				firmoney.Format(L"%d", it->m_count);
				secmoney.Format(L"%d", itemDlg.m_count);
				fircolor.Format(L"%d", it->m_count);
				seccolor.Format(L"%d", itemDlg.m_count);
				CString msg = CString("�޸��") +
					CString("����") + it->m_name + CString("->") + itemDlg.m_name +
					CString(" ���") + firmoney + CString("->") + secmoney +
					CString(" ��ɫ") + fircolor + CString("->") + seccolor;
				TIns->CallFunc(&msg, TMsgType::OutPutMessage);
				it->m_name = itemDlg.m_name;
				it->m_color = itemDlg.m_color;
				it->m_count = itemDlg.m_count;
				it->m_img = itemDlg.m_legend;
				TIns->CallFunc(TIns->m_nowPies, TMsgType::UpdateListAndPie);
			}
		}
	}
}


//void CFileView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if (m_wndListCtrl.GetItemCount() <= 0)
//	{
//		g
//	}
//	CDockablePane::OnLButtonDown(nFlags, point);
//}
