#pragma once
typedef CString cchar;

template<typename T>
struct CItem;

#include"TVector.h"

class CPie
{
public:
	typedef CItem<cchar> CI;
	CPie(CRect &rc, CString title);
	explicit CPie(CString title);
	~CPie();
	void Draw(CDC* pdc);
	void DeleteItem(CI& item);
	void DeleteItemByName(CString name);
	void InsertItem(CString name, int count);
	void InsertItem(CI& item);
	void SetDrawArea(CRect &rc) {
		m_showRect = rc;
	}
	void SetTitle(CString name)
	{
		m_title = name;
	}
	CString GetTitle()
	{
		return m_title;
	}
	CI& GetItemByName(CString name);
	TVector<CI>* GetDatas()
	{
		return &data;
	}
private:
	TVector<CI> data;
	CString m_title;
	CRect m_showRect;

	void DrawTitle(CDC* pdc, float windowPray);
	void DrawLegend(CDC* pdc, float windowPray);
	void DrawPie(CDC* pdc, float windowPray);
	void DrawPic(CDC* pdc, CPoint point,CString);
};

