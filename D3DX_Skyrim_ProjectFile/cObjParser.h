#pragma once
#include "stdafx.h"

//< 단일 매쉬로 이루어진 Obj 파일을 파싱하는 클래스 입니다. ( 형석 )
//< 단일 매쉬로 이루어져 있는데 특화되어 있으므로 매쉬가 여러 부분으로 나눠져 있는 파일에 대해서는 올바르게 작동하지 않을 수 있습니다.
//< Obj를 파싱하는데 필요한 데이터는 .Obj, .mtl, 텍스쳐파일 입니다.
//< 3ds_max에서 export 할때 텍스쳐 파일의 경로가 작성되므로, .mtl 파일 내부의 경로를 "파일명.확장자" 만 남기고 모두 삭제해야 합니다.
//< 데이터의 경로는 Resources/Obj/ 이하에 모두 포함되어 있어야 올바르게 작동합니다.

class cObjFileMesh;
class cObjParser
{
	FILE*		m_fp;

public:
	cObjParser(void);
	~cObjParser(void);

	void Parse( LPDIRECT3DDEVICE9 m_pd3dDevice, char* szFileName, cObjFileMesh* pMap );
	void ParseMaterial( LPDIRECT3DDEVICE9 m_pd3dDevice, char* szFileName, cObjFileMesh* pMap );
	char* GetToken( FILE* fp );
	float GetFloat( FILE* fp );
	int GetInteger( FILE* fp );
	bool IsWhite( char ch );
	bool IsEqualString( char* str1, char* str2 );
};

