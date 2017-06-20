#pragma once
#include "cVisibleObject.h"

//< 움직이지 않는 오브젝트를 관리하는 클래스입니다. ( 형석 )

class cMesh;
class cStaticObject :	public cVisibleObject
{
	//< 지형에 활용되는 오브젝트 etc.

public:
	cStaticObject(void);
	virtual ~cStaticObject(void);

	virtual void Setup( void );
	virtual void Render( void );
};

