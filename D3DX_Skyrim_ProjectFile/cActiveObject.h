#pragma once
#include "cVisibleObject.h"

//< �����̴� ������Ʈ�� �����ϴ� Ŭ�����Դϴ�. ( ���� )

class cActiveObject : public cVisibleObject
{
protected:


public:
	cActiveObject(void);
	virtual ~cActiveObject(void);

	virtual void Update(void);

};




