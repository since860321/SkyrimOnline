#include "stdafx.h"
#include "cObjFileMesh.h"
#include "cObjParser.h"


cObjParser::cObjParser(void)
{
	m_fp = NULL;
}


cObjParser::~cObjParser(void)
{
}

void cObjParser::Parse( LPDIRECT3DDEVICE9 m_pd3dDevice, char* szFileName, cObjFileMesh* pMesh )
{
	fopen_s( &m_fp, szFileName, "r" );

	fseek( m_fp, 0, SEEK_SET );		//< ���������� �� ������ �̵�
	char* szToken;

	std::vector<D3DXVECTOR3>		VertexPos; //< �ʿ� ���� ����.
	std::vector<D3DXVECTOR2>		VertexTex; //< �ʿ� ���� ����.
	std::vector<D3DXVECTOR3>		VertexNormal; //< �ʿ� ���� ����.

	stObjVertex v;
	int nIndex[3];

	//< ���� �б�
	while( !feof(m_fp) )
	{
		szToken = GetToken( m_fp );
		if( IsEqualString( szToken, "mtllib" ) )
		{
			szToken = GetToken( m_fp );
			ParseMaterial( m_pd3dDevice, szToken, pMesh );
		}
		else if( IsEqualString( szToken, "v" ) )
		{	
			D3DXVECTOR3 v;
			GetToken( m_fp );	//< �����ϳ� ����
			v.x = GetFloat( m_fp );
			pMesh->GetMaxPtr()->x = max( pMesh->GetMaxPtr()->x, v.x );
			pMesh->GetMinPtr()->x = min( pMesh->GetMinPtr()->x, v.x );
			v.y = GetFloat( m_fp );
			pMesh->GetMaxPtr()->y = max( pMesh->GetMaxPtr()->y, v.y );
			pMesh->GetMinPtr()->y = min( pMesh->GetMinPtr()->y, v.y );
			v.z = GetFloat( m_fp );
			pMesh->GetMaxPtr()->z = max( pMesh->GetMaxPtr()->z, v.z );
			pMesh->GetMinPtr()->z = min( pMesh->GetMinPtr()->z, v.z );
			VertexPos.push_back( v );
		}
		else if( IsEqualString( szToken, "vt" ) )
		{
			D3DXVECTOR2 tex;

//			GetToken( m_fp );	//< �����ϳ� ����
			tex.x = GetFloat( m_fp );
			tex.y = GetFloat( m_fp );
			tex.y = 1 - tex.y;
			GetToken( m_fp );
			VertexTex.push_back( tex );
		}
		else if( IsEqualString( szToken, "vn" ) )
		{
			D3DXVECTOR3 normal;
//			GetToken( m_fp );	//< �����ϳ� ����
			normal.x = GetFloat( m_fp );
			normal.y = GetFloat( m_fp );
			normal.z = GetFloat( m_fp );
			VertexNormal.push_back( normal );
		}
		else if( IsEqualString( szToken, "usemtl" ) )
		{
//			Mesh.m_MtrlName = GetToken( m_fp );
		}
		else if( IsEqualString( szToken, "f" ) )
		{
			//< ���� 1��
			szToken = GetToken(m_fp);
			sscanf_s( szToken, "%d/%d/%d", &nIndex[0], &nIndex[1], &nIndex[2] );
			v.vPos = VertexPos[nIndex[0]-1];
			v.vTex = VertexTex[nIndex[1]-1];
			v.vNormal = VertexNormal[nIndex[2]-1];
			pMesh->m_Vertices.push_back(v);
			//< ���� 1��
			szToken = GetToken(m_fp);
			sscanf_s( szToken, "%d/%d/%d", &nIndex[0], &nIndex[1], &nIndex[2] );
			v.vPos = VertexPos[nIndex[0]-1];
			v.vTex = VertexTex[nIndex[1]-1];
			v.vNormal = VertexNormal[nIndex[2]-1];
			pMesh->m_Vertices.push_back(v);
			//< ���� 1��
			szToken = GetToken(m_fp);
			sscanf_s( szToken, "%d/%d/%d", &nIndex[0], &nIndex[1], &nIndex[2] );
			v.vPos = VertexPos[nIndex[0]-1];
			v.vTex = VertexTex[nIndex[1]-1];
			v.vNormal = VertexNormal[nIndex[2]-1];
			pMesh->m_Vertices.push_back(v);
		}
	}

	//< ���ؽ� ���� �Ҵ��ϰ�
	if( FAILED (m_pd3dDevice->CreateVertexBuffer( pMesh->m_Vertices.size()*sizeof(stObjVertex), 0, OBJ_D3DFVF, D3DPOOL_DEFAULT, &pMesh->m_pVB, 0 )))
	{
		MessageBox( g_hWnd, "���ؽ������Ҵ����", "����", MB_OK );
	}

	//< ���ؽ����ۿ� �� �ְ�
	void* pAddr;
	pMesh->m_pVB->Lock( 0, pMesh->m_Vertices.size()*sizeof(stObjVertex), &pAddr, 0 );
	memcpy( pAddr, &pMesh->m_Vertices[0], pMesh->m_Vertices.size()*sizeof(stObjVertex) );
	pMesh->m_pVB->Unlock();

	fclose( m_fp );
}

void cObjParser::ParseMaterial( LPDIRECT3DDEVICE9 m_pd3dDevice, char* szFileName, cObjFileMesh* pMesh )
{
	FILE* fp = NULL;

	char szTemp[1024] = "Resources/obj/";
	strcat( szTemp, szFileName );

	fopen_s( &fp, szTemp, "r" );
	char* szToken;

	//< ���� �б�
	while( !feof(fp) )
	{
		szToken = GetToken( fp );
		if( IsEqualString( szToken, "newmtl" ) )
		{
//			szToken = GetToken( fp );
//			strcpy( szMtrlName, szToken );
//			pMap->m_Mtrl[szMtrlName]; //< �ʿ� ������ ���� �� ����
		}
		else if( IsEqualString( szToken, "Ka" ) )	
		{
			D3DCOLORVALUE ambient;
//			GetToken( fp );	//< �����ϳ� ����
			ambient.r = GetFloat( fp );
			ambient.g = GetFloat( fp );
			ambient.b = GetFloat( fp );
			pMesh->m_Mtrl.Ambient = ambient;
		}
		else if( IsEqualString( szToken, "Kd" ) )
		{
			D3DCOLORVALUE diffuse;
//			GetToken( fp );	//< �����ϳ� ����
			diffuse.r = GetFloat( fp );
			diffuse.g = GetFloat( fp );
			diffuse.b = GetFloat( fp );
			pMesh->m_Mtrl.Diffuse = diffuse;
		}
		else if( IsEqualString( szToken, "Ks" ) )
		{
			D3DCOLORVALUE specular;
//			GetToken( fp );	//< �����ϳ� ����
			specular.r = GetFloat( fp );
			specular.g = GetFloat( fp );
			specular.b = GetFloat( fp );
			pMesh->m_Mtrl.Specular = specular;
		}
		else if( IsEqualString( szToken, "map_Kd" ) )
		{
			szToken = GetToken( fp );

			if( pMesh->m_pTex == NULL ) //< �ؽ��� ���� �ߺ� ���� ����
			{
				//< �ؽ��� ���� �Ҵ�
				char szTemp2[1024] = "Resources/obj/";
				strcat( szTemp2, szToken );
				
				if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, szTemp2, &pMesh->m_pTex ) ))
				{
					MessageBox( g_hWnd, "�ؽ��Ĺ����Ҵ����", "����", MB_OK );
				}
			}
		}
	}

	fclose( fp );
}

char* cObjParser::GetToken( FILE* fp )
{
	char* pCh;
	char Ch;
	static char szBuf[1024];			//< ��ū�� ��� ������ ������ �����ϸ� �Ҹ��ϹǷ� static���� ����
	pCh = &szBuf[0];

	while( true )
	{
		Ch = fgetc( fp );			//< �ѱ��ھ� �о�ͼ� Ch ����
		*pCh = Ch;					//< ��ū ���ۿ� �о�� ���ڸ� ����
		if( IsWhite( *pCh )|| feof(fp) )		//< ��ū�� ���� ���鹮���̰ų� ������ ���̸� �߶󳻱�
		{
			*pCh = NULL;				//< ��ū�� �� ������ NULL ó��
			break;
		}
		pCh++;							//< ��ū�� ����Ű�� ������ ��ġ �̵�
	}

	return szBuf;
}

bool cObjParser::IsWhite( char ch )
{
	if( (0 <= ch) && (ch <= 32) )
	{
		return true;
	}
	return false;
}

bool cObjParser::IsEqualString( char* str1, char* str2 )
{
	if( strcmp( str1, str2 ) == 0 ) return true;
	return false;
}

float cObjParser::GetFloat( FILE* fp )
{
	char* szBuf;
	szBuf = GetToken( fp );
	return atof( szBuf );
}

int cObjParser::GetInteger( FILE* fp )
{
	char* szBuf;
	szBuf = GetToken( fp );
	return atoi( szBuf );
}

