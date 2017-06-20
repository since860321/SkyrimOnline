#pragma once
#include "stdafx.h"

//< ���� �Ž��� �̷���� Obj ������ �Ľ��ϴ� Ŭ���� �Դϴ�. ( ���� )
//< ���� �Ž��� �̷���� �ִµ� Ưȭ�Ǿ� �����Ƿ� �Ž��� ���� �κ����� ������ �ִ� ���Ͽ� ���ؼ��� �ùٸ��� �۵����� ���� �� �ֽ��ϴ�.
//< Obj�� �Ľ��ϴµ� �ʿ��� �����ʹ� .Obj, .mtl, �ؽ������� �Դϴ�.
//< 3ds_max���� export �Ҷ� �ؽ��� ������ ��ΰ� �ۼ��ǹǷ�, .mtl ���� ������ ��θ� "���ϸ�.Ȯ����" �� ����� ��� �����ؾ� �մϴ�.
//< �������� ��δ� Resources/Obj/ ���Ͽ� ��� ���ԵǾ� �־�� �ùٸ��� �۵��մϴ�.

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

