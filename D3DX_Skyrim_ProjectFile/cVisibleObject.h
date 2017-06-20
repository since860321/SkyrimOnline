#pragma once

//가시 오브젝트 클래스 (백)
#include "cObject.h"

namespace BaseD3D
{
	class BaseSkinnedMesh;
};

class cVisibleObject  :	public cObject
{
protected:
	BaseD3D::BaseSkinnedMesh*		m_pSkinnedMesh;     //그려질 모델변수

public:
	cVisibleObject(void);
	virtual ~cVisibleObject(void);

	virtual void Render( void ); //< (천수) 그리기
	void SetModel( IN BaseD3D::BaseSkinnedMesh* SkinnedMesh); //< (천수) 그릴모델
	BaseD3D::BaseSkinnedMesh* GetMesh(){return m_pSkinnedMesh;}

};
