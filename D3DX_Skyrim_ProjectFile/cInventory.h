#pragma once
#include "macros.h"

class CSprite;
class cResourceMgr;
class cTerrainObject;
class cObjFileMesh;
class cCharacter;
class cInventory
{
private:
	std::map<char*,stInvenObjInfo> m_InvenObject;
	cCharacter*		m_Player;



public:
	cInventory(void);
	~cInventory(void);


	void GetMesh(cObjFileMesh* obj, char* ObjName,int Attack, int Defence, int Weight);
	void Inventory(cResourceMgr*	ResMg, CSprite*	Spriter, cCharacter*	character);
	void Render(char* ObjName,stTransformInfo TransInfo); //오브젝트 보여주기
	void Delete();

};
