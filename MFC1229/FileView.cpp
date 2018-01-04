
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
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | /*LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |*/ LVS_EX_CHECKBOXES;

	//m_wndListCtrl.Create(dwViewStyle, rectDummy, this, 4);
	if (!m_wndListCtrl.Create(dwViewStyle, rectDummy, this, ID_MY_LISTCTRL))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像: 
	 m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	 m_wndListCtrl.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();
	// 注册响应事件
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
	m_wndListCtrl.InsertColumn(1, _T("类目"), LVCFMT_LEFT, 80);
	m_wndListCtrl.InsertColumn(2, _T("金额"), LVCFMT_LEFT, 100);
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
//		// 选择已单击的项: 
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
			CString msg = CString("标题修改为：") + dlg.m_name;
			TIns->CallFunc(&msg, TMsgType::OutPutMessage);
		}
	}
}

void CFileView::OnFileOpen()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnFileOpenWith()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnDummyCompile()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCut()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCopy()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditClear()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

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
	// TODO: 在此添加控件通知处理程序代码
	// 选择状态改变
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
		//添加线程操作  
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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
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



// 新增元数据
void CFileView::OnListNew()
{
	CItemInfoDlg itemDlg;
	srand((unsigned)time(NULL));
	itemDlg.m_color = RGB(rand() % 255, rand() % 255, rand() % 255);
	itemDlg.m_caption = _T("新增");
	if (itemDlg.DoModal() == IDOK)
	{
		if (itemDlg.m_legend != _T(""))
			TIns->InsertItem(itemDlg.m_name, itemDlg.m_count, itemDlg.m_color,itemDlg.m_legend);
		else
			TIns->InsertItem(itemDlg.m_name, itemDlg.m_count, itemDlg.m_color);
		CString msg = CString("新增项：") + itemDlg.m_name;
		TIns->CallFunc(&msg, TMsgType::OutPutMessage);
	}
}


void CFileView::OnListDelete()
{
	// TODO: 在此添加命令处理程序代码
	auto pos = m_wndListCtrl.GetFirstSelectedItemPosition();
	int iitem = m_wndListCtrl.GetNextSelectedItem(pos);
	if (iitem != -1)
	{
		auto it = (CItem<cchar>*)m_wndListCtrl.GetItemData(iitem);
		CString msg = CString("删除项：") + it->m_name;
		TIns->CallFunc(&msg,TMsgType::OutPutMessage);
		m_wndListCtrl.DeleteItem(iitem);
		TIns->DeleteNowPieItem(it->m_name);
	}
}


void CFileView::OnListChange()
{
	// TODO: 在此添加命令处理程序代码
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
		itemDlg.m_caption = _T("修改");
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
				CString msg = CString("修改项：") +
					CString("名字") + it->m_name + CString("->") + itemDlg.m_name +
					CString(" 金额") + firmoney + CString("->") + secmoney +
					CString(" 颜色") + fircolor + CString("->") + seccolor;
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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (m_wndListCtrl.GetItemCount() <= 0)
//	{
//		g
//	}
//	CDockablePane::OnLButtonDown(nFlags, point);
//}
