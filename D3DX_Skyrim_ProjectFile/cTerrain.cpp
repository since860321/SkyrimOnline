#include "StdAfx.h"
#include "cTerrain.h"


cTerrain::cTerrain(void)
{
	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexture = NULL;
}


cTerrain::~cTerrain(void)
{
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );
	SAFE_RELEASE( m_pTexture );
}

void cTerrain::Setup( void )
{
	//< 지형에 사용되는 텍스쳐 할당
//	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, "Resources/textures/reachmoss01.dds", &m_pTexture ) ))
	if( FAILED ( D3DXCreateTextureFromFile( sgDevice, "Resources/obj/fieldgrass02.dds", &m_pTexture ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return ;
	}

	//< 지형에 사용되는 버텍스버퍼 할당
	if( FAILED( sgDevice->CreateVertexBuffer( MAPSIZE*MAPSIZE*sizeof(stObjVertex), 0, OBJ_D3DFVF, D3DPOOL_DEFAULT, &m_pVB, 0 ) ))
	{
		MessageBox( g_hWnd, "지형 버텍스 버퍼 할당 실패", "실패", MB_OK );
		return ;
	}
	//< 지형에 사용되는 인덱스버퍼 할당
	if( FAILED( sgDevice->CreateIndexBuffer( (MAPSIZE-1)*(MAPSIZE-1)*2*sizeof(stIndex), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, 0 ) ))
	{
		MessageBox( g_hWnd, "지형 인덱스 버퍼 할당 실패", "실패", MB_OK );
		return ;
	}

	//< Height Map 정보 읽어오기
	HANDLE hFile;
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	DWORD dwRead;
	DWORD dwDataSize;
	BYTE* pData;
	hFile = CreateFile( "Resources/height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	ReadFile( hFile, &fh, sizeof(fh), &dwRead, NULL );
	ReadFile( hFile, &ih, sizeof(ih), &dwRead, NULL );
	dwDataSize = fh.bfSize - fh.bfOffBits;
	pData = new BYTE[dwDataSize];
	ReadFile( hFile, pData, dwDataSize, &dwRead, NULL );
	CloseHandle( hFile );

	DWORD dwAddr;
	for( int i = 0 ; i < MAPSIZE ; i ++ )
	{
		for( int j = 0 ; j < MAPSIZE ; j ++ )
		{
			dwAddr = ( ih.biHeight - 1 - i )*MAPSIZE*3 + j*3;
			m_vPos[i * MAPSIZE + j] = D3DXVECTOR3( ((-MAPSIZE/2)+j)*TILEWIDTH, pData[dwAddr]/1.0f*20.0f-1700.0f, ((MAPSIZE/2)-1-i)*TILEWIDTH );
		}
	}

	delete[] pData;

	//< 버텍스버퍼에 값 넣기
	stObjVertex VertexTerrain[MAPSIZE*MAPSIZE];
	float nX, nY;
	for( int i = 0 ; i < MAPSIZE ; i ++ )
	{
		for( int j = 0 ; j < MAPSIZE ; j ++ )
		{
			VertexTerrain[i*MAPSIZE+j].vPos = m_vPos[i*MAPSIZE+j];
			VertexTerrain[i*MAPSIZE+j].vNormal = D3DXVECTOR3( 0, -1, 0 );
//			VertexTerrain[i*MAPSIZE+j].vTex = D3DXVECTOR2( j / float(MAPSIZE-1), i / float(MAPSIZE-1) );
			if( j%2 == 0 ){	nX = 0;}
			else{ nX = 1; }
			if( i%2 == 0 ){ nY = 0;}
			else { nY = 1; }
			VertexTerrain[i*MAPSIZE+j].vTex = D3DXVECTOR2( nX, nY );
		}
	}

	void* pAddr;
	m_pVB->Lock( 0, sizeof( stObjVertex ) * MAPSIZE * MAPSIZE, &pAddr, 0 );
	memcpy( pAddr, VertexTerrain, sizeof(stObjVertex) * MAPSIZE * MAPSIZE );
	m_pVB->Unlock();

	//< 인덱스버퍼 메모리카피
	stIndex Index[(MAPSIZE-1) * (MAPSIZE-1) * 2];
	DWORD dwCount = 0;

	for( int i = 0 ; i < MAPSIZE - 1; i ++ )
	{
		for( int j = 0 ; j < MAPSIZE - 1; j ++ )
		{
			dwAddr = i * MAPSIZE + j;
			Index[dwCount].w0 = dwAddr + MAPSIZE;
			Index[dwCount].w1 = dwAddr;
			Index[dwCount].w2 = dwAddr + MAPSIZE + 1;
			dwCount ++;

			Index[dwCount].w0 = dwAddr + MAPSIZE + 1;
			Index[dwCount].w1 = dwAddr;
			Index[dwCount].w2 = dwAddr + 1;
			dwCount ++;
		}
	}

	m_pIB->Lock( 0, sizeof( stIndex ) * (MAPSIZE-1) * (MAPSIZE-1) * 2, &pAddr, 0 );
	memcpy( pAddr, Index, sizeof( stIndex) * (MAPSIZE-1) * (MAPSIZE-1) * 2 );
	m_pIB->Unlock();
}

void cTerrain::Render( void )
{
//	sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	sgDevice->SetTexture( 0, m_pTexture );
	cDevice::GetInstance().SetWireFrameTexture( WF_GREEN ); //< 와이어프레임

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	sgDevice->SetTransform( D3DTS_WORLD, &matWorld );

	sgDevice->SetStreamSource( 0, m_pVB, 0, sizeof(stObjVertex) );
	sgDevice->SetFVF( OBJ_D3DFVF );
	sgDevice->SetIndices( m_pIB);	//인덱스버퍼가 있는 경우만
	sgDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, MAPSIZE * MAPSIZE, 0,  (MAPSIZE - 1) * (MAPSIZE - 1) * 2);

//	sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
}

void cTerrain::SetHeight( IN D3DXVECTOR3* pPos )
{
	/* HeightMap에 HeightPicking을 적용하여 높이를 설정하는 함수*/

	//< 이동하기전에 HeightPicking 으로 서있는 높이 구하기
	bool IsMovable = false;

	D3DXVECTOR3 origin( pPos->x, 10000.0f, pPos->z ); 
	D3DXVECTOR3 vRay( 0, -1, 0 ); 
	float distance = 0.0f;
	float u, v = 0.0f;

	int nX, nZ;
	DWORD dwAddr;
	nX = pPos->x/TILEWIDTH + MAPSIZE/2;
	nZ = MAPSIZE/2 - 1 - pPos->z/TILEWIDTH;
	dwAddr = nZ*MAPSIZE + nX;

	//< 어느 삼각형 위에 있는지 구하기
	float fRelativeX, fRelativeZ;
	fRelativeX = pPos->x - m_vPos[dwAddr].x;
	fRelativeZ = m_vPos[dwAddr].z - pPos->z;

	//< 네 꼭지점의 y값
	float fY[4];
	fY[0] = m_vPos[dwAddr].y;
	fY[1] = m_vPos[dwAddr+1].y;
	fY[2] = m_vPos[dwAddr+MAPSIZE].y;
	fY[3] = m_vPos[dwAddr+MAPSIZE+1].y;

	D3DXVECTOR3 vTriangle[3];
	float fHeight;
	if( fRelativeX  >= fRelativeZ ) //< 우상단
	{
		fHeight = (fY[0] + fY[1] + fY[2])/3.0f;

		vTriangle[0] = m_vPos[dwAddr];
		vTriangle[1] = m_vPos[dwAddr+1];
		vTriangle[2] = m_vPos[dwAddr+MAPSIZE];
	}
	else //< 좌하단
	{
		fHeight = (fY[0] + fY[3] + fY[2])/3.0f;

		vTriangle[0] = m_vPos[dwAddr];
		vTriangle[1] = m_vPos[dwAddr+MAPSIZE];
		vTriangle[2] = m_vPos[dwAddr+MAPSIZE+1];
	}

	IsMovable = IntersectTriangle( origin, vRay, vTriangle[0],
		vTriangle[1],
		vTriangle[2],
		&distance, &u, &v );
	if( IsMovable == true )
	{
		pPos->y = origin.y - distance;
	}
}

bool cTerrain::IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
							   D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
							   FLOAT* t, FLOAT* u, FLOAT* v )
{
	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross( &pvec, &dir, &edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot( &edge1, &pvec );

	D3DXVECTOR3 tvec;
	if( det > 0 )
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if( det < 0.0001f )
		return FALSE;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot( &tvec, &pvec );
	if( *u < 0.0f || *u > det )
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot( &dir, &qvec );
	if( *v < 0.0f || *u + *v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
}







