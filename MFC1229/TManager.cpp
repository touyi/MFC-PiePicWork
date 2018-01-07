#include "stdafx.h"
#include "TManager.h"
#include"Item.h"
TManager* TManager::instance;
CString TManager::GetNowDocTitle()
{
	for (auto it = m_pies.begin(); it != m_pies.end(); it++)
	{
		if (it->second == m_nowPies)
		{
			return it->first;
		}
	}
	return CString();
}
CPie * TManager::GetPieByName(CString name)
{
	if (m_pies.count(name) > 0)
	{
		return m_pies[name];
	}
	return NULL;
}

void TManager::DeletePieByName(CString name)
{
	if (m_pies.count(name) > 0)
	{
		if (m_pies[name] == m_nowPies)
		{
			CallFunc(NULL, TMsgType::DeleteNowPie);
		}
		delete m_pies[name];
		m_pies.erase(name);
	}
}

void TManager::SetNowPie(CString name)
{
	if (m_pies.count(name) > 0)
	{
		if (m_pies[name] != m_nowPies)
		{
			m_nowPies = m_pies[name];
			CallFunc(m_nowPies, TMsgType::FoucsPieChange);
		}
	}
	else
	{
		{
			m_nowPies = new CPie(name);
			m_pies[name] = m_nowPies;
			CallFunc(m_nowPies, TMsgType::FoucsPieChange);
		}
	}
}

void TManager::RegistFunc(function<void(void*, TMsgType)> fun,CString name)
{
		funcs[name] = fun;
}

void TManager::UnRegistFunc(CString name)
{
	if(funcs.count(name)>0)
		funcs.erase(name);
}

void TManager::CallFunc(void * param, TMsgType type)
{
	for (auto it = funcs.begin(); it != funcs.end(); it++)
	{
		(*it).second(param, type);
	}
}

void TManager::SetPieItemActive(CString itemName, bool isActive)
{
	auto& item = m_nowPies->GetItemByName(itemName);
	if (isActive != item.isActive)
	{
		item.isActive = isActive;
		CallFunc(NULL, TMsgType::ReDrawPie);
	}
}

void TManager::SetPieItemName(CString oldName, CString newName)
{
}

void TManager::SetPieItemCount(CString itemName, int newCount)
{
}

void TManager::InsertPie(CString name,CPie * pie)
{
	if (pie != NULL)
	{
		if (m_pies.count(name) > 0)
		{
			delete m_pies[name];
		}
		m_pies[name] = pie;
	}
}


void TManager::InsertItem(CString name, int count, int color, cchar icon)
{
	CPie::CI* item = new CPie::CI(name, count, color);
	item->m_img = icon;
	m_nowPies->InsertItem(*item);

	CallFunc(m_nowPies, TMsgType::UpdateListAndPie);
}
void TManager::DeleteNowPieItem(CString name)
{
	if (m_nowPies == NULL)
		return;
	m_nowPies->DeleteItemByName(name);
	CallFunc(NULL, TMsgType::ReDrawPie);
}

TManager::TManager()
{
	/*m_nowPies = NULL;
	m_nowPies = new CPie(CString("MFC12291"));
	m_nowPies->InsertItem(_T("类型1"), 100);
	m_nowPies->InsertItem(_T("类型2"), 100);
	m_nowPies->InsertItem(_T("类型3"), 100);
	m_pies[CString("MFC12291")] = m_nowPies;

	m_nowPies = new CPie(CString("这是饼图2"));
	m_nowPies->InsertItem(_T("类型88"), 300);
	m_nowPies->InsertItem(_T("类型99"), 200);
	m_nowPies->InsertItem(_T("类型145"), 100);
	m_pies[CString("MFC12292")] = m_nowPies;*/
	m_nowPies = NULL;
}


TManager::~TManager()
{
	for (auto it = m_pies.begin(); it != m_pies.end(); it++)
	{
		if ((*it).second != NULL)
			delete (*it).second;
	}
}
