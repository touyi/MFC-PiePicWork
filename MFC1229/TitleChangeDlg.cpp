// TitleChangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC1229.h"
#include "TitleChangeDlg.h"
#include "afxdialogex.h"


// CTitleChangeDlg 对话框

IMPLEMENT_DYNAMIC(CTitleChangeDlg, CDialogEx)

CTitleChangeDlg::CTitleChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_name(_T(""))
{

}

CTitleChangeDlg::~CTitleChangeDlg()
{
}

void CTitleChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
}


BEGIN_MESSAGE_MAP(CTitleChangeDlg, CDialogEx)
END_MESSAGE_MAP()


// CTitleChangeDlg 消息处理程序
