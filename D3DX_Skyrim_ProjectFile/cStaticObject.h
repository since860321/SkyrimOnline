#pragma once
#include "cVisibleObject.h"

//< �������� �ʴ� ������Ʈ�� �����ϴ� Ŭ�����Դϴ�. ( ���� )

class cMesh;
class cStaticObject :	public cVisibleObject
{
	//< ������ Ȱ��Ǵ� ������Ʈ etc.

public:
	cStaticObject(void);
	virtual ~cStaticObject(void);

	virtual void Setup( void );
	virtual void Render( void );
};

