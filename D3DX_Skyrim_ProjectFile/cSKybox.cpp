#include "StdAfx.h"
#include "cSKybox.h"

cSKybox::cSKybox(void)
{
}

cSKybox::~cSKybox(void)
{
	
	for(int i=0; i<6; i++) 
	{
		SAFE_RELEASE(m_Tex[i]);
		SAFE_RELEASE(m_Vtx[i]);
		SAFE_RELEASE(m_Idx[i]);
	}	

}


void cSKybox::SetTexture(char* top, char* bottom, char* left, char* right,char* front,char* back)
{
	//상자 만들기


	//< 윗면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, top, &m_Tex[0] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}


	//< 아래면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, bottom, &m_Tex[1] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}


	//< 왼쪽면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, left, &m_Tex[2] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}


	//< 오른쪽면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice,right, &m_Tex[3] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}

	//< 앞면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, front, &m_Tex[4] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}


	//< 뒷면에 사용되는 텍스쳐 할당
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, back, &m_Tex[5] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}

	for(int i = 0 ; i <6 ; ++i)
	{
	//< 지형에 사용되는 버텍스버퍼 할당
		if( FAILED( sgDevice->CreateVertexBuffer( sizeof(stObjVertex)*4 ,0, OBJ_D3DFVF, D3DPOOL_DEFAULT, &m_Vtx[i], NULL ) ))
		{
			MessageBox( g_hWnd, "지형 버텍스 버퍼 할당 실패", "실패", MB_OK );
			return ;
		}
	}

	//윗면
	stObjVertex Buffer[4];
	D3DXVECTOR3 vNormal = D3DXVECTOR3(0,-1,0);

	 Buffer[0].vPos		=	D3DXVECTOR3( 1.f, 1.f,-1.f);
	 Buffer[0].vNormal	=	vNormal;
	 Buffer[0].vTex		=	D3DXVECTOR2(0,1);

	 Buffer[1].vPos		=	D3DXVECTOR3(-1.f, 1.f,-1.f);
	 Buffer[1].vNormal	=	vNormal;
	 Buffer[1].vTex		=	D3DXVECTOR2(0,0);

	 Buffer[2].vPos		=	D3DXVECTOR3( 1.f, 1.f, 1.f);
	 Buffer[2].vNormal	=	vNormal;
	 Buffer[2].vTex		=	D3DXVECTOR2(1,1);
	
	 Buffer[3].vPos		=	D3DXVECTOR3(-1.f, 1.f, 1.f);
	 Buffer[3].vNormal	=	vNormal;
	 Buffer[3].vTex		=	D3DXVECTOR2(1,0);
	
	void* pAddr;
	m_Vtx[0]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[0]->Unlock();

	// 아랫면
	vNormal = D3DXVECTOR3(0,1,0);

	Buffer[0].vPos		=	D3DXVECTOR3( 1.f,-1.f, 1.f);
	Buffer[0].vNormal	=	vNormal;
	Buffer[0].vTex		=	D3DXVECTOR2(0,0);

	Buffer[1].vPos		=	D3DXVECTOR3(-1.f,-1.f, 1.f);
	Buffer[1].vNormal	=	vNormal;
	Buffer[1].vTex		=	D3DXVECTOR2(1,0);

	Buffer[2].vPos		=	D3DXVECTOR3( 1.f,-1.f,-1.f);
	Buffer[2].vNormal	=	vNormal;
	Buffer[2].vTex		=	D3DXVECTOR2(0,1);
	
	Buffer[3].vPos		=	D3DXVECTOR3(-1.f,-1.f,-1.f);
	Buffer[3].vNormal	=	vNormal;
	Buffer[3].vTex		=	D3DXVECTOR2(1,1);

	m_Vtx[1]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[1]->Unlock();

	// 좌측
	vNormal = D3DXVECTOR3(1,0,0);

	Buffer[0].vPos		=	D3DXVECTOR3( -1.f,1.f, 1.f);
	Buffer[0].vNormal	=	vNormal;
	Buffer[0].vTex		=	D3DXVECTOR2(0,0);

	Buffer[1].vPos		=	D3DXVECTOR3(-1.f,1.f, -1.f);
	Buffer[1].vNormal	=	vNormal;
	Buffer[1].vTex		=	D3DXVECTOR2(1,0);

	Buffer[2].vPos		=	D3DXVECTOR3( -1.f,-1.f,1.f);
	Buffer[2].vNormal	=	vNormal;
	Buffer[2].vTex		=	D3DXVECTOR2(0,1);
	
	Buffer[3].vPos		=	D3DXVECTOR3(-1.f,-1.f,-1.f);
	Buffer[3].vNormal	=	vNormal;
	Buffer[3].vTex		=	D3DXVECTOR2(1,1);

	m_Vtx[2]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[2]->Unlock();

	// 우측
	vNormal = D3DXVECTOR3(-1,0,0);

	Buffer[0].vPos		=	D3DXVECTOR3( 1.f,1.f, -1.f);
	Buffer[0].vNormal	=	vNormal;
	Buffer[0].vTex		=	D3DXVECTOR2(0,0);

	Buffer[1].vPos		=	D3DXVECTOR3(1.f,1.f, 1.f);
	Buffer[1].vNormal	=	vNormal;
	Buffer[1].vTex		=	D3DXVECTOR2(1,0);

	Buffer[2].vPos		=	D3DXVECTOR3( 1.f,-1.f,-1.f);
	Buffer[2].vNormal	=	vNormal;
	Buffer[2].vTex		=	D3DXVECTOR2(0,1);
	
	Buffer[3].vPos		=	D3DXVECTOR3(1.f,-1.f,1.f);
	Buffer[3].vNormal	=	vNormal;
	Buffer[3].vTex		=	D3DXVECTOR2(1,1);

	m_Vtx[3]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[3]->Unlock();


	// 앞면
	vNormal = D3DXVECTOR3(0,0,-1);

	Buffer[0].vPos		=	D3DXVECTOR3( 1.f,1.f, 1.f);
	Buffer[0].vNormal	=	vNormal;
	Buffer[0].vTex		=	D3DXVECTOR2(0,0);

	Buffer[1].vPos		=	D3DXVECTOR3(-1.f,1.f, 1.f);
	Buffer[1].vNormal	=	vNormal;
	Buffer[1].vTex		=	D3DXVECTOR2(1,0);

	Buffer[2].vPos		=	D3DXVECTOR3( 1.f,-1.f,1.f);
	Buffer[2].vNormal	=	vNormal;
	Buffer[2].vTex		=	D3DXVECTOR2(0,1);
	
	Buffer[3].vPos		=	D3DXVECTOR3(-1.f,-1.f,1.f);
	Buffer[3].vNormal	=	vNormal;
	Buffer[3].vTex		=	D3DXVECTOR2(1,1);

	m_Vtx[4]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[4]->Unlock();

	// 뒷면
	vNormal = D3DXVECTOR3(0,0,1);


	Buffer[0].vPos		=	D3DXVECTOR3( -1.f,1.f, -1.f);
	Buffer[0].vNormal	=	vNormal;
	Buffer[0].vTex		=	D3DXVECTOR2(0,0);

	Buffer[1].vPos		=	D3DXVECTOR3(1.f,1.f, -1.f);
	Buffer[1].vNormal	=	vNormal;
	Buffer[1].vTex		=	D3DXVECTOR2(1,0);

	Buffer[2].vPos		=	D3DXVECTOR3( -1.f,-1.f,-1.f);
	Buffer[2].vNormal	=	vNormal;
	Buffer[2].vTex		=	D3DXVECTOR2(0,1);
	
	Buffer[3].vPos		=	D3DXVECTOR3(1.f,-1.f,-1.f);
	Buffer[3].vNormal	=	vNormal;
	Buffer[3].vTex		=	D3DXVECTOR2(1,1);

	m_Vtx[5]->Lock( 0, sizeof(stObjVertex)*4, &pAddr, 0 );
	memcpy( pAddr, Buffer,sizeof(stObjVertex)*4 );
	m_Vtx[5]->Unlock();

	for(int i=0; i<6; i++)
	{
		//인덱스 버퍼
		if(FAILED(sgDevice->CreateIndexBuffer(sizeof(stIndex)*2, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_Idx[i], NULL))) return;
	}
	//인덱스 버퍼 초기화

	stIndex iv[2];
	iv[0].w0 = 3;
	iv[0].w1 = 1;
	iv[0].w2 = 2;
	iv[1].w0 = 2;
	iv[1].w1 = 1;
	iv[1].w2 = 0;
	
	
	for(int i=0; i<6; i++)
	{
		m_Idx[i]->Lock(0, sizeof(stIndex)*2, &pAddr, 0);
		memcpy( pAddr, iv, sizeof(stIndex)*2 );
		m_Idx[i]->Unlock();
	}

	return;
}


void cSKybox::Render()
{
	if( cDevice::GetInstance().m_bWireFrameMode == true ) return;

	if(!m_Vtx || !m_Idx) return;

	
	//스카이 박스 이동.
	D3DXVECTOR3 vPos = *vEyePos;
	D3DXMATRIX matPos,matscal,matWorld;

	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);
	D3DXMatrixScaling(&matscal, 55000, 55000, 55000);

	matWorld =matscal * matPos ;
	sgDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//텍스쳐 UV Address를 설정.
	sgDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); 
	sgDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

//	sgDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, this->m_Vertices.size()/3 );

	//스카이박스 드로우.
	for(int i=0; i<6; i++)
	{
		sgDevice->SetTexture(0,  m_Tex[i]);
		sgDevice->SetStreamSource(0, m_Vtx[i], 0, sizeof(stObjVertex));
		sgDevice->SetFVF(OBJ_D3DFVF);
		sgDevice->SetIndices(m_Idx[i]);
		sgDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	}

	sgDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); //< 디폴트값(형석)
	sgDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //< 디폴트값(형석)
}