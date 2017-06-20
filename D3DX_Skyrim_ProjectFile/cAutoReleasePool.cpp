#include "StdAfx.h"
#include "cObject.h"
#include "cAutoReleasePool.h"

cAutoReleasePool::cAutoReleasePool(void)
{
}

cAutoReleasePool::~cAutoReleasePool(void)
{
}

cAutoReleasePool& cAutoReleasePool::GetInstance( void )
{
	static cAutoReleasePool instance;

	return instance; 
}

void cAutoReleasePool::AddObject(cObject* pObject)
{
	m_AutoReleasePool.push_back(pObject);
}

void cAutoReleasePool::Drain( void )
{
	std::list<cObject*>::iterator it;

	for( it = m_AutoReleasePool.begin(); it != m_AutoReleasePool.end(); ++ it )
	{
		(*it)->Release(); //< 레퍼런스 카운터 감소시켜줘!
	}

	m_AutoReleasePool.clear(); //< 시킨대로 했으니 풀을 싹 비움
}

