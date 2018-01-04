
// MFC1229Doc.cpp : CMFC1229Doc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFC1229.h"
#endif

#include "MFC1229Doc.h"

#include <propkey.h>
#include"TManager.h"
#include"Item.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFC1229Doc

IMPLEMENT_DYNCREATE(CMFC1229Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFC1229Doc, CDocument)
END_MESSAGE_MAP()


// CMFC1229Doc 构造/析构

CMFC1229Doc::CMFC1229Doc()
{
	// TODO: 在此添加一次性构造代码

}

CMFC1229Doc::~CMFC1229Doc()
{
}

BOOL CMFC1229Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMFC1229Doc 序列化

void CMFC1229Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << TIns->m_nowPies->GetTitle();
		ar << TIns->m_nowPies->GetDatas()->size();
		auto& datas = *TIns->m_nowPies->GetDatas();
		for (int i = 0; i < datas.size(); i++)
		{
			ar << datas[i].m_name << datas[i].m_count << datas[i].m_color << datas[i].m_img;
		}
	}
	else
	{
		CString name;
		ar >> name;
		CPie* pie = new CPie(name);
		int size;
		ar >> size;
		CPie::CI item;
		for (int i = 0; i < size; i++)
		{
			ar >> item.m_name >> item.m_count >> item.m_color >> item.m_img;
			pie->InsertItem(item);
		}
		TIns->InsertPie(ar.GetFile()->GetFileName(), pie);
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMFC1229Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMFC1229Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMFC1229Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFC1229Doc 诊断

#ifdef _DEBUG
void CMFC1229Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC1229Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC1229Doc 命令
