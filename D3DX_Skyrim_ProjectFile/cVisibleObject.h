#pragma once

//���� ������Ʈ Ŭ���� (��)
#include "cObject.h"

namespace BaseD3D
{
	class BaseSkinnedMesh;
};

class cVisibleObject  :	public cObject
{
protected:
	BaseD3D::BaseSkinnedMesh*		m_pSkinnedMesh;     //�׷��� �𵨺���

public:
	cVisibleObject(void);
	virtual ~cVisibleObject(void);

	virtual void Render( void ); //< (õ��) �׸���
	void SetModel( IN BaseD3D::BaseSkinnedMesh* SkinnedMesh); //< (õ��) �׸���
	BaseD3D::BaseSkinnedMesh* GetMesh(){return m_pSkinnedMesh;}

};
