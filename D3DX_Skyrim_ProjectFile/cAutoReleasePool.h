#pragma once

class cObject;
class cAutoReleasePool
{
	std::list<cObject*> m_AutoReleasePool; //< ����Ʈ�� �����Ѵ�.
	cAutoReleasePool();

public:
	virtual ~cAutoReleasePool();
	static cAutoReleasePool& GetInstance(); //< �̱���
	void AddObject(cObject* pObject); //< ���丱���� Ǯ�� ���
	void Drain(); //< ���丱���� Ǯ�� ��ϵ� ��� ��ü�� ���۷��� ī���� ����
};

