#pragma once
#include "cobject.h"

//< 랜더링 되지 않는 객체를 정의합니다. (형석)

class cUnvisibleObject : public cObject
{
public:
	cUnvisibleObject(void);
	virtual ~cUnvisibleObject(void);
};

