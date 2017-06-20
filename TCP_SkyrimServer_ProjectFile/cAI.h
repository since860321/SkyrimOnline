#pragma once

//���� AI �� Ŭ���� �̱��� : �ɸ��� ���� �����͸� ������ �������� �����͸� �޴´�
class cAIStatus;
class cActiveObject;
class cCharacter;
/*
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
*/
#include <map>

class cAI
{
	cAI(void);
	~cAI(void);

	cAIStatus*	m_AIStatus;
	cAIStatus*  m_DragonAIStatus;
	std::map<int ,stClientInfo>* PlayerInfo;


public:
	
	
	static cAI& GetInstance( void );
	
	void SetPlayerInfo(IN	std::map<int ,stClientInfo>* m_Client);

	void AI(IN	stEnemyInfo*  MonsterInfo);

};
