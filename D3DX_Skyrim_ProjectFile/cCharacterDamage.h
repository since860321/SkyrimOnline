#pragma once

#include <list>

class cCharacterDamage
{		
	std::list<stShowDamage>	stData;
	DWORD			dwDelay;
	DWORD			dwMoveSpeed;
	std::list<stShowDamage>::iterator	iter;
	cCharacterDamage(void);
	~cCharacterDamage(void);

public:
	static cCharacterDamage& GetInstance();
	void SetUp(DWORD	dwDelay,	DWORD			dwMoveSpeed);
	void SetDamage(DWORD	dwDamage);
	void Update();
	void Render();
};
