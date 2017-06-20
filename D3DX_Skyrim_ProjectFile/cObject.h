#pragma once

class cObject
{
	int m_nRefCount;

protected:
	stTransformInfo	m_TransformInfo; //<<���� TM�� ����� ���� ����
	D3DXMATRIXA16	m_WolrdTM;		 //<<���� �׸��� �������� ��Ʈ����

public:
	cObject( void )
	: m_nRefCount(1) //< �ڱⰡ �����Ǹ� �ڱ��ڽŵ� ������ ������ ���ԵǹǷ� 1�� �ʱ�ȭ
	{}
	virtual ~cObject(void);

	//�޸� ���� �κ��Դϴ� << ���� 
	void Retain( void ); //< ���۷��� ī���͸� ������Ű�� �Լ�
	void Release( void ); //< ���۷��� ī���͸� ���ҽ�Ű�� ī���Ͱ� 0�� �Ǹ� �޸������� �ϴ� �Լ� 
	void AutoRelease( void ); //< �ش� ��ü�� ���丱���� Ǯ�� �ִ� �Լ�

	//�⺻ ������ ���ɴϴ�.
	stTransformInfo* GetTransformInfo(){return &m_TransformInfo;}
	D3DXMATRIXA16*	 GetWorldTM(){return &m_WolrdTM;}

	//<< ���� ��Ʈ��Ʈ�� ���մϴ�
	virtual void SetWorldTM( void ); 
};

