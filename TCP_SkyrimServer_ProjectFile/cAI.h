//#pragma once

//몬스터 AI 용 클래스 싱글톤 : 케릭터 정보 포인터를 가지고 몬스터정보 포인터를 받는다
class cAIStatus;
class cActiveObject;
class cCharacter;
struct stClientInfo;
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
	
	void SetPlayerInfo(std::map<int ,stClientInfo>* m_Client);

	void AI(stEnemyInfo*  MonsterInfo);

};
