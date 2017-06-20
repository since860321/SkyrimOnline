#pragma once


namespace BaseD3D
{
	class BaseSkinnedMesh;
};

class cGrid;
class cCamera;
class cFrustum;
class cOctree;
class cObjParser;
class cActiveObject;
class cCharacter;
class cStaticObject;
class cPlayerCharacter;
class cResourceMgr;
class CSprite;
class cTerrain;
class cTerrainObject;
class cObjFileMesh;
class cInventory;
class cMainGame
{
	cDevice*						m_pLoadingDevice;

	cGrid*						m_pGrid;

	cObjParser*					m_pObjParser;
						
	std::vector<cObjFileMesh*>				m_pWeapon;
	std::map<std::string, cObjFileMesh*>	m_pLevelMesh;

	BaseD3D::BaseSkinnedMesh*	m_pDragonModel;
	BaseD3D::BaseSkinnedMesh*	m_pPlayerModel;

	cOctree*					m_pOctree;

	//< Active Object
	cCharacter*					m_pPlayer;
	cCharacter*					m_pDragon;

	//< Static Object
	cStaticObject*				m_pTerrain;	
	cStaticObject*				m_pSkyBox;
	std::vector<cTerrainObject*>			m_pLevel;

	//< Unvisible Object
	cCamera*					m_pCamera;


	//FPS 카메라 모드 on/ off
	bool						m_bFPS_Mode;

	//UI이미지
	cResourceMgr*			m_pResMgr;
	cInventory*					m_pInven;
	cObjFileMesh*				m_pLogo;
	int								m_nProcess;
	char							m_szLoadFileName[1024];
	bool							m_bSetupComplete;

	POINT						m_ptMouse;

	int							m_LodingImage;

	HANDLE					m_hThread;		//< 스레드 핸들
	DWORD					m_ThreadId;		//< 스레드 ID

private:
	void Update( void );
	void Render( void );
	void RenderLogo( void );
	void SetupLoadingCamera( LPDIRECT3DDEVICE9 m_pd3dDevice );
	void CreateSetupThread( void );

	//플레이어 셋업
	void Setup_Player();

	Ray CalcPickingRay(int x , int y);
	void TransformRay(Ray* ray,D3DXMATRIXA16* T);

	void CrashSphere();
	bool IntersectSphere(Ray ray,stSphere sphere);

	void ParseTerrainLocationInformation( char* szFileName );

public:
	cMainGame(void);
	~cMainGame(void);

	bool Setup( void );
	void Manager( void );

	LRESULT CALLBACK MainProc( HWND, UINT, WPARAM, LPARAM );
	static DWORD WINAPI ProcessSetup( LPVOID arg );
};

