#pragma once
#include<map>
#include<functional>
#include"Pie.h"
using std::function;
using std::map;
enum TMsgType {
	FoucsPieChange,
	DeleteNowPie
};
class TManager
{
private:
	
	static TManager* instance;
	map<CString, CPie*>m_pies;
	
	vector<function<void(void*, TMsgType)> >funcs;
public:
	CPie* m_nowPies;
	static TManager* Get() {
		if (instance == NULL)
			instance = new TManager();
		return instance;
	}

	CPie* GetPieByName(CString);
	void DeletePieByName(CString);
	void SetNowPie(CString name);
	void RegistFunc(function<void(void*, TMsgType)> fun);
	void CallFunc(void* param, TMsgType type);
	TManager();
	~TManager();
};

