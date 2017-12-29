#pragma once
template<typename T>
struct CItem
{
	CItem(CString name, int count,int color) :m_count(count), m_name(name),m_color(color)
	{
	}
	CItem() = default;
	CString m_name;
	int m_count;
	int m_color;
	T m_img;
	bool isActive = true;
	bool operator==(const CItem<T>& item)
	{
		if (item.m_name == m_name)
			return true;
		return false;
	}
};

