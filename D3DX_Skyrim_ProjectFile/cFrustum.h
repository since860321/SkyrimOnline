#pragma once

//< �ø��� ���� ���������� �̱����������� �����մϴ�. (����)

class cFrustum
{
	D3DXPLANE			m_Plane[6];
	LPD3DXMESH			m_pMesh;		//< ������ �ڽ�
	D3DXMATRIXA16		m_matInv;		//< ������ ����ȯ ���

	cFrustum(void);
public:
	virtual ~cFrustum(void);
	static cFrustum& GetInstance();

	void Setup( void );

	void Move( void );
	bool CheckInFrustum( IN const D3DXVECTOR3* vCenter, IN const float* fRadius );
	void DebugRender( void );
	
	//< properties
	inline D3DXPLANE* GetPlanePtr( int nIndex ){ return &m_Plane[nIndex]; }
};




