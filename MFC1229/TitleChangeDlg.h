#pragma once


// CTitleChangeDlg �Ի���

class CTitleChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTitleChangeDlg)

public:
	CTitleChangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTitleChangeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
};
