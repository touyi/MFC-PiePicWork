// ItemInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC1229.h"
#include "ItemInfo.h"
#include "afxdialogex.h"
#include "resource.h"

// CItemInfo �Ի���

IMPLEMENT_DYNAMIC(CItemInfoDlg, CDialogEx)

CItemInfoDlg::CItemInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ITEM_INFO, pParent)
	, m_name(_T(""))
	, m_count(0)
	, m_legend(_T(""))
	, m_color_rect(CRect(120, 110, 195, 135))
{

}

CItemInfoDlg::~CItemInfoDlg()
{
}

void CItemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_count);
	DDX_Text(pDX, IDC_EDIT3, m_legend);
}


BEGIN_MESSAGE_MAP(CItemInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CItemInfoDlg::OnChooseLegendBtnClicked)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CItemInfo ��Ϣ�������


void CItemInfoDlg::OnChooseLegendBtnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE,
		_T(".png"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("ͼƬ(*.png)|*.png||"));
	if (fileDlg.DoModal() == IDOK)
	{
		UpdateData();
		m_legend = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CItemInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
	auto tBrush = CreateSolidBrush(m_color);
	auto oldObj = dc.SelectObject(tBrush);
	dc.Rectangle(m_color_rect);
	dc.SelectObject(oldObj);
	SetWindowTextW(m_caption);
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CItemInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_color_rect.PtInRect(point))
	{
		CColorDialog colorDlg;
		if (colorDlg.DoModal() == IDOK)
		{
			m_color = colorDlg.GetColor();
			InvalidateRect(m_color_rect);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
