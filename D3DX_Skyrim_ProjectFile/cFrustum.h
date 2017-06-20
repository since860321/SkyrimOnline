#pragma once

//< 컬링을 위한 프러스텀을 싱글턴패턴으로 정의합니다. (형석)

class cFrustum
{
	D3DXPLANE			m_Plane[6];
	LPD3DXMESH			m_pMesh;		//< 디버깅용 박스
	D3DXMATRIXA16		m_matInv;		//< 디버깅용 역변환 행렬

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




