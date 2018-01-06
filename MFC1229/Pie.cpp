#include "stdafx.h"
#include "Pie.h"
#include"Item.h"
#include<map>
#include<stdlib.h>
using std::map;


CPie::CPie(CRect& rc,CString title):m_showRect(rc),
m_title(title)
{
	
}

CPie::CPie(CString title):m_title(title)
{
}


CPie::~CPie()
{
}
// ����ͼ��
void CPie::Draw(CDC * pdc)
{
	float nowPray = CalRatio();

	// ���Ʊ�ͼ
	DrawPie(pdc, nowPray);
	// ���Ʊ���
	DrawTitle(pdc, nowPray);
	// ����ͼ��
	DrawLegend(pdc, nowPray);
	
}

void CPie::DeleteItem(CI & item)
{
	DeleteItemByName(item.m_name);
}

void CPie::DeleteItemByName(CString name)
{
	CI* temp = NULL;
	data.for_each([name,&temp](CI& it) {
		if (it.m_name == name)
		{
			temp = &it;
			return true;
		}
		return false;
	});
	data.erase(*temp);
}

void CPie::InsertItem(CString name, int count)
{
	
	data.push_back(CI(name, count, RGB(rand() % 255, rand() % 255, rand() % 255)));
}

void CPie::InsertItem(CI & item)
{
	data.push_back(item);
}

CPie::CI * CPie::ClickItem(CPoint pt)
{
	const float PI = 3.1415926;
	pt -= m_showRect.TopLeft();
	float ratio = CalRatio();
	float radius = 80 * ratio;
	int disx = (pt.x - radius);
	int disy = (pt.y - radius);
	if (disx*disx + disy*disy <= radius*radius)
	{
		if (data.size() <= 0)return NULL;
		map<CString, float> mp;
		double sum = 0;
		data.for_each([&sum](CI& it) {
			if (it.isActive)
				sum += it.m_count;
			return false;
		});

		for (int i = 0; i < data.size(); i++)
		{
			if (!data[i].isActive)continue;
			CString name = data[i].m_name;
			float pray = data[i].m_count / sum;
			mp[name] = pray;
		}
		disy = -disy; // y�ᷭת
		float thea = acos(disx / sqrt(disx*disx + disy*disy));
		if (disy < 0)
		{
			thea = 2 * PI - thea;
		}
		sum = 0;
		for (auto it = mp.begin(); it != mp.end(); it++)
		{
			sum += 2 * PI*it->second;
			if (sum >= thea)
			{
				return &(GetItemByName(it->first));
			}
		}
		return NULL;
	}
	
	return NULL;
}

CPie::CI& CPie::GetItemByName(CString name)
{
	CI* temp = NULL;
	data.for_each([name, &temp](CI& it) {
		if (it.m_name == name)
		{
			temp = &it;
			return true;
		}
		return false;
	});
	if (temp == NULL)
	{
		abort();
	}
	return *temp;
}

float CPie::CalRatio()
{
	const float Width_height_ratio = 8.0f / 6.0f;
	float nowRatio = m_showRect.Width()*1.0f / m_showRect.Height();
	float nowPray = 1;
	if (nowRatio > Width_height_ratio)
	{
		nowPray = m_showRect.Height() / 200.0f;
	}
	else
	{
		nowPray = m_showRect.Width() / (200.0f * Width_height_ratio);
	}
	return nowPray;
}

void CPie::DrawTitle(CDC * pdc, float windowPray)
{
	// ������
	CFont font, *oldFont;
	font.CreatePointFont(60 * windowPray, _T("����"), pdc);
	pdc->SetBkMode(TRANSPARENT);
	oldFont = pdc->SelectObject(&font);

	// ������Title
	int x, y;
	y = 160 * windowPray + 10 * windowPray + m_showRect.top;
	x = 35 * windowPray + m_showRect.left;
	pdc->TextOutW(x, y, m_title);

	pdc->SelectObject(oldFont);
	if (oldFont)
	{
		font.DeleteObject();
	}
}

void CPie::DrawLegend(CDC * pdc, float windowPray)
{
	CPoint lt = {int(200 * windowPray), int(40 * windowPray)};
	lt += m_showRect.TopLeft();
	int step = 20 * windowPray;
	CRect expRect;
	map<CString, int>mp;
	data.for_each([&mp](CI& it) {
		if (it.isActive)
		{
			mp[it.m_name] = it.m_color;
		}
		return false;
	});

	for (auto it = mp.begin(); it != mp.end(); it++)
	{
		CRect rc;
		rc.left = lt.x;
		rc.top = lt.y;
		rc.right = rc.left + step*0.6f;
		rc.bottom = rc.top + step*0.6f;
		pdc->SelectObject(CreateSolidBrush(it->second));
		pdc->Rectangle(rc);
		
		CFont font, *oldFont;
		font.CreatePointFont(60 * windowPray, _T("����"), pdc);
		pdc->SetBkMode(TRANSPARENT);
		oldFont = pdc->SelectObject(&font);
		pdc->TextOutW(lt.x + 20 * windowPray, lt.y, it->first);
		pdc->SelectObject(oldFont);

		lt.y += step;
	}
}

void CPie::DrawPie(CDC * pdc, float windowPray)
{
	// ���㼤��Ԫ�ص�ռ��
	map<CString, float> mp;
	double sum = 0;
	data.for_each([&sum](CI& it) {
		if (it.isActive)
			sum += it.m_count;
		return false;
	});

	for (int i = 0; i < data.size(); i++)
	{
		if (!data[i].isActive)continue;
		CString name = data[i].m_name;
		float pray = data[i].m_count / sum;
		mp[name] = pray;
	}
	// ������Ȧ
	int radius = 80 * windowPray;
	CRect rc(m_showRect.left, m_showRect.top, 2 * radius + m_showRect.left, 2 * radius + m_showRect.top);

	pdc->Ellipse(rc);
	// ��Ԫ��ʱӦ���˳�
	if (mp.size() <= 0)return;
	// ���Ʊ�ͼ
	const double PI = 3.1415926;
	CPoint mid = { radius + m_showRect.left,radius + m_showRect.top };
	float angle = 360.0*mp.begin()->second;
	CPoint p_start = { radius,0 };
	CPoint p_half_end = { int(radius*cos(angle / 2 * PI / 180)),-int(radius*sin(angle / 2 * PI / 180)) };
	CPoint p_end = { int(radius*cos(angle * PI / 180)),-int(radius*sin(angle * PI / 180)) };
	
	
	auto oldPen = pdc->SelectObject(CreateSolidBrush(GetItemByName(mp.begin()->first).m_color));
	if (mp.size() == 1)
		pdc->Ellipse(rc);
	else
		pdc->Pie(rc, p_start + mid, p_end + mid);

	// �����׸�ͼ��
	p_half_end += mid;
	// ���ƴ���TODO
	DrawPic(pdc, p_half_end, GetItemByName(mp.begin()->first).m_img);

	for (auto it = ++mp.begin(); it != mp.end(); it++)
	{
		// ��������
		p_start = p_end;
		angle += (*it).second*360.0;
		p_end = { int(radius*cos(angle * PI / 180)),-int(radius*sin(angle * PI / 180)) };
		pdc->SelectObject(CreateSolidBrush(GetItemByName(it->first).m_color));
		pdc->Pie(rc, p_start + mid, p_end + mid);

		// ����ͼ��
		float half_angle = angle - it->second * 180;
		p_half_end = { int(radius*cos(half_angle * PI / 180)),-int(radius*sin(half_angle * PI / 180)) };
		p_half_end += mid;
		// ���ƴ��� TODO
		DrawPic(pdc, p_half_end, GetItemByName(it->first).m_img);
	}
	pdc->SelectObject(oldPen);
}

void CPie::DrawPic(CDC * pdc, CPoint point,CString path)
{
	if (path == _T(""))
		return;
	CImage img;
	img.Load(path);
	if (img.IsNull())
	{
		return;
	}
	int iheight = img.GetHeight();
	int iwidth = img.GetWidth();
	CRect rect(point.x - iwidth / 2, point.y - iheight / 2, point.x + iwidth/2, point.y + iheight/2);
	HBITMAP hbitmap = img.Detach();
	CBitmap cBitmap;
	BITMAP bitmap;
	cBitmap.Attach(hbitmap);
	cBitmap.GetBitmap(&bitmap);
	if (cBitmap.GetSafeHandle())
	{
		CDC tmpdc;
		tmpdc.CreateCompatibleDC(pdc);
		CBitmap *pOldBitmap = tmpdc.SelectObject(&cBitmap);

		int nXOriginDest = 0;
		int nYOriginDest = 0;
		int nWidthDest = 0;
		int hHeightDest = 0;

		nXOriginDest = rect.left;// Ŀ��Xƫ��  
		nYOriginDest = rect.top;// Ŀ��Yƫ��  
		nWidthDest = rect.Width();// Ŀ����  
		hHeightDest = rect.Height();// Ŀ��߶�  

		::TransparentBlt(pdc->m_hDC, nXOriginDest, nYOriginDest, nWidthDest, hHeightDest,
			tmpdc.m_hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));//��ɫ��Ϊ͸��ɫ  
		tmpdc.SelectObject(pOldBitmap);
		tmpdc.DeleteDC();
	}
}
