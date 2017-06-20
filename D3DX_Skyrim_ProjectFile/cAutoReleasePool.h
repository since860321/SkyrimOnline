#pragma once

class cObject;
class cAutoReleasePool
{
	std::list<cObject*> m_AutoReleasePool; //< 리스트로 관리한다.
	cAutoReleasePool();

public:
	virtual ~cAutoReleasePool();
	static cAutoReleasePool& GetInstance(); //< 싱글턴
	void AddObject(cObject* pObject); //< 오토릴리즈 풀에 등록
	void Drain(); //< 오토릴리즈 풀에 등록된 모든 객체의 레퍼런스 카운터 감소
};

