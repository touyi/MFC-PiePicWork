#pragma once
#include<map>
#include<functional>
#include"Pie.h"
using std::function;
using std::map;
#define TIns TManager::Get()
enum TMsgType {
	FoucsPieChange,
	DeleteNowPie,
	ReDrawPie,
	UpdateListAndPie,
	OutPutMessage
};
class TManager
{
private:
	
	static TManager* instance;
	map<CString, CPie*>m_pies;
	
	map<CString,function<void(void*, TMsgType)> >funcs;
public:
	CPie* m_nowPies;
	static TManager* Get() {
		if (instance == NULL)
			instance = new TManager();
		return instance;
	}
	CString GetNowDocTitle();
	CPie* GetPieByName(CString);
	void DeletePieByName(CString);
	void SetNowPie(CString name);
	void RegistFunc(function<void(void*, TMsgType)> fun,CString name);
	void UnRegistFunc(CString name);
	void CallFunc(void* param, TMsgType type);
	void SetPieItemActive(CString itemName, bool isActive);
	void SetPieItemName(CString oldName, CString newName);
	void SetPieItemCount(CString itemName, int newCount);
	void InsertPie(CString name,CPie*);

	void InsertItem(CString name, int count, int color, cchar icon = CString("./res/pic/star.png"));

	void DeleteNowPieItem(CString name);
	TManager();
	~TManager();
};

