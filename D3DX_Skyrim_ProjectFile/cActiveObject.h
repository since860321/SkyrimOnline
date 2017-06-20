#pragma once
#include "cVisibleObject.h"

//< 움직이는 오브젝트를 관리하는 클래스입니다. ( 형석 )

class cActiveObject : public cVisibleObject
{
protected:


public:
	cActiveObject(void);
	virtual ~cActiveObject(void);

	virtual void Update(void);

};




