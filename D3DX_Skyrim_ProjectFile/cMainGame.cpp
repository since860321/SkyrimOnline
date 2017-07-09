#include "StdAfx.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cFrustum.h"
#include "cOctree.h"
#include "cGameTime.h"
#include "cObjParser.h"
#include "BaseSkinnedMesh.h"
#include "cPlayerCharacter.h"
#include "cEnemyCharacter.h"
#include "cTerrain.h"
#include "cTerrainObject.h"
#include "cNetworkMgr.h"
#include "cObjFileMesh.h"
#include "cResourceMgr.h"
#include "cInventory.h"
#include "CSprite.h"
#include "cMainGame.h"
#include "cSkybox.h"
#include "cSoundMgr.h"
#include "cCharacterDamage.h"

cMainGame::cMainGame(void)
{
	m_pLoadingDevice = NULL;

    m_pGrid = NULL;
    m_pCamera = NULL;

    m_pObjParser = NULL;
    m_pDragonModel	= NULL;
    m_pPlayerModel	= NULL;
    m_pOctree = NULL;

    //<< Active
    m_pDragon	= NULL;
    m_pPlayer	= NULL;

    m_pTerrain = NULL;

	m_bSetupComplete = false;
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE( m_pLoadingDevice );

    SAFE_DELETE( m_pGrid );
    m_pCamera->Release();

    m_pDragonModel->Release();
    SAFE_DELETE( m_pDragonModel );
    m_pPlayerModel->Release();
    SAFE_DELETE( m_pPlayerModel	);
	
    SAFE_DELETE( m_pObjParser );
    SAFE_DELETE( m_pOctree );

    SAFE_DELETE(m_pResMgr);
	SAFE_DELETE(m_pInven);

    m_pPlayer->Release();
	SAFE_DELETE_ARRAY( m_pDragon );
    m_pTerrain->Release();
 
	SAFE_DELETE( m_pSkyBox );

	std::map<std::string, cObjFileMesh*>::iterator iter = m_pLevelMesh.begin();
	while( !m_pLevelMesh.empty() )
	{
		SAFE_DELETE ( iter->second );
		iter = m_pLevelMesh.erase( iter );
	}

	std::vector<cTerrainObject*>::iterator iterLevel = m_pLevel.begin();
	while( !m_pLevel.empty() )
	{
		SAFE_DELETE ( *iterLevel );
		iterLevel = m_pLevel.erase( iterLevel );
	}

	for( int i = 0 ; i < m_pWeapon.size(); i ++ )
	{
		SAFE_DELETE( m_pWeapon[i]);
	}
	m_pWeapon.clear();
}

bool cMainGame::Setup( void )
{
   //< 타임
    cGameTime::Instance().Start();
	ShowCursor(false);

	cSoundMgr::GetInstance()->Init(TOTAL_SOUND_CHANNEL);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/back_normal.wav",SOUND_LODING,true,true);
	cSoundMgr::GetInstance()->SoundPlay(SOUND_LODING,3.0f); // 로딩 배경음
	
	m_nProcess = 0;
    cDevice::GetInstance().CreateDevice( g_hWnd );
	//< 로딩 전용 디바이스 생성 및 세팅(형석)
	m_pLoadingDevice = new cDevice();
	m_pLoadingDevice->CreateDevice( g_hWnd );
	SetupLoadingCamera( m_pLoadingDevice->GetDevice() );

    //< 컬모드 Off
    sgDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    //< Z버퍼
    sgDevice->SetRenderState( D3DRS_ZENABLE, TRUE ); 

    //< 빛
    D3DXVECTOR3 vDrt;
    D3DLIGHT9 light;
    memset( &light, 0, sizeof(light) );
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.78f;
    vDrt = D3DXVECTOR3( 1, -1, 1 );
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vDrt );
	sgDevice->SetLight( 0, &light );
    sgDevice->LightEnable( 0, TRUE );
    sgDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	sgDevice->SetRenderState( D3DRS_AMBIENT, 0x00ffffff );
	//< 로딩 전용 디바이스 랜더스테이트 세팅(형석)
	m_pLoadingDevice->GetDevice()->SetLight( 0, &light );
    m_pLoadingDevice->GetDevice()->LightEnable( 0, TRUE );
    m_pLoadingDevice->GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pLoadingDevice->GetDevice()->SetRenderState( D3DRS_AMBIENT, 0x00aaaaaa );

	m_pObjParser = new cObjParser();

	m_pLogo = new cObjFileMesh;
	m_pObjParser->Parse( m_pLoadingDevice->GetDevice(), "Resources/obj/logo.obj", m_pLogo );

	CreateSetupThread(); //< 셋업스레드( 매쉬 로딩과 같은 무거운 작업은 스레드 안에서 처리하면 됩니다.( 형석 )
	this->RenderLogo(); //< 셋업스레드에서 셋업이 완료될때까지 로고 애니매이션 무한반복

	//<Inven
	m_pInven = new cInventory();
	m_pInven->GetMesh(m_pWeapon[WEAPON_EX_AXE],"처형용 도끼",10,0,5);
	m_pInven->GetMesh(m_pWeapon[WEAPON_BATTLE_AXE],"배틀 액스",14,10,5);
	m_pInven->GetMesh(m_pWeapon[WEAPON_TWOHAND_STEEL],"투핸드소드",13,12,5);
	m_pInven->GetMesh(m_pWeapon[WEAPON_HAMMER],"워 해머",6,7,5);
	m_pInven->GetMesh(m_pWeapon[WEAPON_TWOHAND],"실버 소드",10,7,5);
	
	//< UI
	m_pResMgr = new cResourceMgr();
	m_pResMgr->SetUp(m_pPlayer,m_pDragon,m_pInven);	
 
	m_nProcess += 5;
	strcpy( m_szLoadFileName, "사운드 셋팅중..." );

	//	< 네트워크 연결
	m_nProcess += 5;
	strcpy( m_szLoadFileName, "네트워크 설정중..." );

	NetworkMgr.CreateSocket(); //< 클라이언트 소켓 생성
 	NetworkMgr.SetReceiver( (cPlayerCharacter*)m_pPlayer, (cEnemyCharacter*)m_pDragon ); //< 데이터 전달받는 객체 설정
	NetworkMgr.GetWeaponMeshPtr( m_pWeapon ); //< 다른 클라이언트의 무기 출력을 위한 매시 포인터 얻어오기

	cSoundMgr::GetInstance()->SoundStop(SOUND_LODING); // 로딩 배경음
	cSoundMgr::GetInstance()->SoundPlay(SOUND_NORMAL,3.0f); // 기본 배경음

	if( FAILED( NetworkMgr.ConnectServer() ) ) return true; //< 서버에 접속
 	NetworkMgr.CreateNetworkThread();	//< 리시브 전용 스레드 생성

	return true;
}

DWORD WINAPI cMainGame::ProcessSetup( LPVOID arg )
{
	cMainGame* maingame = (cMainGame*)arg; 

	//< Object
	cObjFileMesh* pTwohand;
	pTwohand = new cObjFileMesh;

	maingame->m_pObjParser->Parse( sgDevice, "Resources/obj/TwoHand.obj", pTwohand );
	maingame->m_pWeapon.push_back(pTwohand);
	
	cObjFileMesh* pHammer;
	pHammer = new cObjFileMesh;

	maingame->m_pObjParser->Parse( sgDevice, "Resources/obj/Hammar.obj",pHammer );
	maingame->m_pWeapon.push_back( pHammer );

	cObjFileMesh* pTwohandsteel;
	pTwohandsteel = new cObjFileMesh;

	maingame->m_pObjParser->Parse( sgDevice, "Resources/obj/TwoHandsteel.obj", pTwohandsteel );
	maingame->m_pWeapon.push_back(pTwohandsteel);

	cObjFileMesh* pBattleAxe;
	pBattleAxe = new cObjFileMesh;

	maingame->m_pObjParser->Parse( sgDevice, "Resources/obj/BattleAxe.obj", pBattleAxe );
	maingame->m_pWeapon.push_back(pBattleAxe);
	
	cObjFileMesh* pEXAxe;
	pEXAxe = new cObjFileMesh;

	maingame->m_pObjParser->Parse( sgDevice, "Resources/obj/executioneraxe.obj", pEXAxe );
	maingame->m_pWeapon.push_back(pEXAxe);
	
////////
	
	srand(GetTickCount());
	maingame->m_LodingImage = rand()%5;

	// 로딩화면 최소조건 셋업
	maingame->m_pCamera = new cCamera();
	maingame->m_pPlayer = new cPlayerCharacter();
	maingame->m_pCamera->Setup( g_hWnd , maingame->m_pPlayer );

    //< 옥트리
    maingame->m_pOctree = new cOctree();
    maingame->m_pOctree->Setup( 3 );

	maingame->m_nProcess = 0;
	strcpy( maingame->m_szLoadFileName, "디바이스 생성중.." );

	//플레이어모델링
    maingame->m_pPlayerModel = new BaseD3D::BaseSkinnedMesh();
    maingame->m_pPlayerModel->Init( sgDevice, "./Resources/xfile/character_test01.X", NULL );
	maingame->m_pPlayerModel->SetAnimationIndex(1, false); //< 모델이랑 시작인덱스 바뀌면 서버단에서도 바꿔줘야 해요~ (형석)

	//< 플레이어 셋업
	maingame->Setup_Player();
	//< 카메라
    maingame->m_bFPS_Mode = true;//이거 false 하면 개발자 카메라 모드 가능

    
	/////////////////////////////////////// 로딩최소조건에 끌려온것들 마무리  

	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/back_battle.wav",SOUND_NORMAL,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/SonsOfSkyrim.wav",SOUND_BATTLE,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_S_Attack.wav",SOUND_C_S_ATTACK,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_S_Swing.wav",SOUND_C_S_SWING,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_C_Attack.wav",SOUND_C_C_ATTACK,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_C_Swing.wav",SOUND_C_C_SWING,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_Run.wav",SOUND_C_RUN,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Character_Death.wav",SOUND_C_DEATH,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Dragon_Attack.wav",SOUND_D_ATTACK,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Dragon_Cry.wav",SOUND_D_CRY,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Dragon_Death.wav",SOUND_D_DEATH,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Dragon_Walk.wav",SOUND_D_WALK,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/Dragon_Wing.wav",SOUND_D_WING,true,true);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/ui_select_off.wav",SOUND_U_OFF,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/ui_select_on.wav",SOUND_U_ON,false,false);
	cSoundMgr::GetInstance()->SoundSetup("Resources/sound/ui_select_choice.wav",SOUND_U_CHOICE,false,false);

    maingame->m_pGrid = new cGrid();
    maingame->m_pGrid->Setup( g_hWnd );
    
	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "옥트리 생성중.." );

//	sgFrustum.Setup();	

    //< 옥트리
	maingame->m_pOctree = new cOctree();
    maingame->m_pOctree->Setup( 2 ); //5 );

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "시간 설정 및 파일 파싱중.." );

////////

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "대장간에서 무기 생산중 ..." );

	maingame->m_nProcess += 15;
	strcpy( maingame->m_szLoadFileName, "몬스터 생성중..." );

    //Xfilefacer
    maingame->m_pDragonModel = new BaseD3D::BaseSkinnedMesh();
    maingame->m_pDragonModel->Init( sgDevice, "./Resources/xfile/dragon.X", 0 );
    maingame->m_pDragonModel->SetAnimationIndex(0, false);

	maingame->m_nProcess += 10;
	strcpy( maingame->m_szLoadFileName, "플레이어 생성중..." );

	maingame->m_nProcess += 10;
	strcpy( maingame->m_szLoadFileName, "플레이어 셋업중..." );

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "몬스터 셋업중..." );

	//< 드레곤 선언
    maingame->m_pDragon = new cEnemyCharacter[ENEMYMAX];
	for( int i = 0 ; i < ENEMYMAX ; ++ i )
	{
		maingame->m_pDragon[i].SetModel(maingame->m_pDragonModel);
		//<< 드래곤 셋팅
		maingame->m_pDragon[i].GetTransformInfo()->vPos		= D3DXVECTOR3(0,0,0);
		maingame->m_pDragon[i].GetTransformInfo()->vRot		= D3DXVECTOR3(0,0,0);
		maingame->m_pDragon[i].GetTransformInfo()->vScale		= D3DXVECTOR3(1,1,1);
						
		maingame->m_pDragon[i].GetCharacterInfo()->dwHP					=	50;
		maingame->m_pDragon[i].GetCharacterInfo()->dwMaxHP			=	50;
		maingame->m_pDragon[i].GetCharacterInfo()->dwAttackDamage	=	4;
		maingame->m_pDragon[i].GetCharacterInfo()->dwRadius			=	270;
		maingame->m_pDragon[i].GetCharacterInfo()->bAlive				= true;
		maingame->m_pDragon[i].GetCharacterInfo()->fMoveSpeed		=	10;
		maingame->m_pDragon[i].GetCharacterInfo()->fRotSpeed		=   0.05f;
		maingame->m_pDragon[i].GetCharacterInfo()->CharacterType	=	CT_MONSTER_DRAGON;
		maingame->m_pDragon[i].GetCharacterInfo()->m_fCamere_UpDownValue = 0;
	}

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "카메라 설정중..." );

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "하늘을 창조중..." );

    //스카이 박스
    maingame->m_pSkyBox = new cSKybox;
    ((cSKybox*)maingame->m_pSkyBox)->SetTexture("Resources/textures/skybox/vanilla_sky_frost_up.jpg","Resources/textures/skybox/vanilla_sky_frost_dn.jpg",
                        "Resources/textures/skybox/vanilla_sky_frost_lf.jpg","Resources/textures/skybox/vanilla_sky_frost_rt.jpg",
                        "Resources/textures/skybox/vanilla_sky_frost_ft.jpg","Resources/textures/skybox/vanilla_sky_frost_bk.jpg");
    ((cSKybox*)maingame->m_pSkyBox)->SetEyePos(maingame->m_pCamera->GetEye());

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "몬스터 세부정보 설정중!$!" );
	
	for( size_t i = 0 ; i < ENEMYMAX ; ++ i )
	{
		stMonsterInfo MonsterInfo;

		MonsterInfo.fPerceptionLength	=	200;
		MonsterInfo.vStartPos			=	maingame->m_pDragon[i].GetTransformInfo()->vPos;
		MonsterInfo.vTargetPos		=	MonsterInfo.vStartPos;
		MonsterInfo.nAniIndex		=	MonsterInfo.NowAni	=	D_ground_bite;
		MonsterInfo.dwUpdateTime	=	0;
		MonsterInfo.bMyIndex		=	i;
		((cEnemyCharacter*)&maingame->m_pDragon[i])->SetMonsterInfo(MonsterInfo);
	}

	maingame->m_nProcess += 5;
	strcpy( maingame->m_szLoadFileName, "대지를 창조중 .." );
 
    //< 지형
    maingame->m_pTerrain = new cTerrain();
    maingame->m_pTerrain->Setup();
	maingame->ParseTerrainLocationInformation( "Resources/obj/맵_배경_좌표값.txt" ); //< 반드시 Terrain Setup후에 호출할것 ( 형석 )

	maingame->m_bSetupComplete = true;

	return 0;
}

void cMainGame::Manager( void )
{
    cGameTime::Instance().Update();
	cSoundMgr::GetInstance()->SoundUpdate();
    Update();
    Render();

}

void cMainGame::Update( void )
{
	m_pPlayer->Update();

	if( m_pPlayer->GetCharacterInfo()->bAlive)
	{

		((cTerrain*)m_pTerrain)->SetHeight(&m_pPlayer->GetTransformInfo()->vPos);
		m_pPlayerModel->PlayFrame(cGameTime::Instance().Get_ElapsedTime() / 1000.0f);
	}
	else
	{
		static float y = 0;
		((cTerrain*)m_pTerrain)->SetHeight(&m_pPlayer->GetTransformInfo()->vPos);
		m_pPlayer->GetTransformInfo()->vRot.x = -D3DX_PI/2;
		m_pPlayer->GetTransformInfo()->vPos.y -=y;
		y+=0.1f;
	}
	

	for( size_t i = 0 ; i < ENEMYMAX ; ++ i )
	{
		m_pDragon[i].Update();
		
		if((((cEnemyCharacter*)&m_pDragon[i])->GetMonsterInfo()->nAniIndex != D_mtidle_hover) &&
			(	((cEnemyCharacter*)&m_pDragon[i])->GetMonsterInfo()->nAniIndex != D_mtfastforward_flap))
		{
			((cTerrain*)m_pTerrain)->SetHeight(&m_pDragon[i].GetTransformInfo()->vPos);
		}

		m_pDragonModel->PlayFrame(cGameTime::Instance().Get_ElapsedTime() / 1000.0f);

	}



    if( m_bFPS_Mode )
    {
        m_pCamera->Setup_FPS();
	}
	
	sgFrustum.Move();
	m_pOctree->Culling( m_pOctree->GetRootPtr() ); //< 옥트리 컬링

	//< 컬링후에 랜더해야 하는 대상과 충돌처리
	//m_pOctree->Collision( m_pOctree->GetRootPtr(), (cPlayerCharacter*) m_pPlayer );

	NetworkMgr.MoveOtherClient();
	

	// 픽킹
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd , &m_ptMouse);

	cCharacterDamage::GetInstance().Update();
	
	CrashSphere();
	

}

void cMainGame::Render( void )
{
    sgDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00202020, 1.0f, 0 );
    
    if( SUCCEEDED ( sgDevice->BeginScene() ))
    {
        //스카이박스
        m_pSkyBox->Render();
        
        //< 프레임레이트 출력
    //cGameTime::Instance().Render();

        //< ActiveObject
		for( size_t i = 0 ; i < ENEMYMAX ; ++ i )
		{
			
			m_pDragon[i].Render();

			//와이어 프레임 박스 그리기
			////////////////////////////////////////////////
// 			LPD3DXMESH m_BoundingBoxMesh;
// 
// 			sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
// 
// 			D3DXCreateSphere(sgDevice , m_pDragon[i].GetCharacterInfo()->dwRadius , 30,30, &m_BoundingBoxMesh, NULL );
// 
// 			D3DXMATRIXA16 matTemp;
// 			D3DXMatrixTranslation(&matTemp , m_pDragon[i].GetTransformInfo()->vPos.x ,
// 				m_pDragon[i].GetTransformInfo()->vPos.y //+ m_pDragon[i].GetCharacterInfo()->dwRadius
// 				,
// 				m_pDragon[i].GetTransformInfo()->vPos.z);
// 			sgDevice->SetTransform( D3DTS_WORLD, &matTemp );
// 
// 			m_BoundingBoxMesh->DrawSubset( 0 );
// 
// 			sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

			
		}

		m_pPlayer->Render();


        //< 그리기
       // m_pGrid->Render();
		m_pOctree->Render( m_pOctree->GetRootPtr() );
		
		cCharacterDamage::GetInstance().Render();

		m_pTerrain->Render();
		
		//< 명석 시험용
		m_pResMgr->Render();	
	
		sgFrustum.DebugRender();

		NetworkMgr.RenderOtherClient();		
        sgDevice->EndScene();
    }

    sgDevice->Present( 0, 0, 0, 0 );
	

}

LRESULT CALLBACK cMainGame::MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    if(m_pPlayer != NULL) m_pPlayer->WndProc( hWnd,  iMessage,  wParam,  lParam );
    
    switch( iMessage )
    {
    case WM_KEYDOWN:
        {
            if( wParam == VK_ESCAPE )
            {
                SendMessage( hWnd, WM_DESTROY, 0, 0 );
            }
			if( wParam == VK_F1 )
			{
				cDevice::GetInstance().SetWireFrame();
			}
        }
        break;
    case WM_LBUTTONDOWN:
        {		
            if(!m_bFPS_Mode)
            {
                m_pCamera->SetCursor( LOWORD(lParam), HIWORD(lParam) );

            }
        }
        break;
    case WM_MOUSEMOVE:
        {
            if(!m_bFPS_Mode)
            {
                if( wParam & MK_LBUTTON )
                {
                    if( GetKeyState( VK_CONTROL ) & 0x8000 )
                    {
                        m_pCamera->Rotation( LOWORD(lParam), HIWORD(lParam) );
                        break;
                    }
                    m_pCamera->Move( LOWORD(lParam), HIWORD(lParam) );
                }
            }
        }
        break;
    case WM_MOUSEWHEEL:
        {
            if(!m_bFPS_Mode)
            {
                if( (SHORT)HIWORD(wParam) > 0 )
                {
                    m_pCamera->ZoomIn();
                }
                else
                {
                    m_pCamera->ZoomOut();
                }
            }
            
        }
        break;
	case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        return 0;
		
    }

    return DefWindowProc( hWnd, iMessage, wParam, lParam );
}

void cMainGame::Setup_Player()
{

	m_pPlayer->SetModel(m_pPlayerModel);

	//< 플레이어 셋팅
	m_pPlayer->GetTransformInfo()->vPos			= D3DXVECTOR3(0,0,1500);
	m_pPlayer->GetTransformInfo()->vRot			= D3DXVECTOR3(0,0,0);
	m_pPlayer->GetTransformInfo()->vScale		= D3DXVECTOR3(0.5f,0.5f,0.5f);

	m_pPlayer->GetCharacterInfo()->dwHP				=	6650;
	m_pPlayer->GetCharacterInfo()->dwMaxHP			=	6650;
	m_pPlayer->GetCharacterInfo()->dwAttackDamage	=	3;
	m_pPlayer->GetCharacterInfo()->dwRadius			=	50;

	m_pPlayer->GetCharacterInfo()->fMoveSpeed			=	3.2f;
	m_pPlayer->GetCharacterInfo()->fRotSpeed			=   0.25f;
	m_pPlayer->GetCharacterInfo()->CharacterType		=	CT_PLAYER;	
	m_pPlayer->GetCharacterInfo()->m_fCamere_UpDownValue = 4.5f;

	m_pPlayer->GetCharacterInfo()->m_pWeapon	=	&m_pWeapon;
	m_pPlayer->GetCharacterInfo()->m_UseWeapon  =  WEAPON_TYPE(m_LodingImage);
	m_pPlayer->GetCharacterInfo()->m_bMouseMove	= false;
	m_pPlayer->GetCharacterInfo()->bAlive	=	true;
	m_pPlayer->GetCharacterInfo()->dwDeadTime	=	0;
	
}




///픽킹

Ray cMainGame::CalcPickingRay( int x , int y )
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	sgDevice->GetViewport(&vp);

	D3DXMATRIXA16 proj;
	sgDevice->GetTransform(D3DTS_PROJECTION , &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0,0);
	py = (((-2.0f*y) / vp.Height) +1.0f) / proj(1,1);

	Ray ray;
	ray.origin = D3DXVECTOR3(0,0,0);
	ray.direction = D3DXVECTOR3(px,py,1.0f);

	return ray;
}

void cMainGame::TransformRay( Ray* ray,D3DXMATRIXA16* T )
{
	D3DXVec3TransformCoord(&ray->origin,&ray->origin,T);
	D3DXVec3TransformNormal(&ray->direction,&ray->direction,T);
	D3DXVec3Normalize(&ray->direction , &ray->direction);
}


bool cMainGame::IntersectSphere( Ray ray,stSphere sphere )
{
	D3DXMATRIXA16 matPos , matPosInv;
	//스피어의 위치 행렬과 그것의 역행렬을 구한다 
	D3DXMatrixTranslation(&matPos, sphere.vPos.x , sphere.vPos.y,sphere.vPos.z );
	D3DXMatrixInverse(&matPosInv, NULL,&matPos);

	//ray에 역행렬을 곱하여서 , 원의 로컬 좌표로 바꾸어준다.
	D3DXVec3TransformCoord(&ray.origin , &ray.origin , &matPosInv);
	D3DXVec3TransformNormal(&ray.direction , &ray.direction , &matPosInv);

	//판별식을 구한다
	float a = D3DXVec3Dot(&ray.direction,&ray.direction); 
	float b = D3DXVec3Dot(&ray.origin,&ray.direction); 
	float c = D3DXVec3Dot(&ray.origin,&ray.origin) - (sphere.fRad*sphere.fRad);

	if(c > 0.0f && b > 0.0f)           
		return false;                                                         

	if(b*b - a*c >= 0 ) return true;

	return false;
}

void cMainGame::CrashSphere()
{

	Ray ray;
	ray = CalcPickingRay(m_ptMouse.x , m_ptMouse.y);

	D3DXMATRIXA16 view , viewInv;
	sgDevice->GetTransform(D3DTS_VIEW , &view);
	D3DXMatrixInverse(&viewInv , 0 , &view);
	TransformRay(&ray, &viewInv);

	stSphere					m_Sp;

	for( size_t i = 0 ; i < ENEMYMAX ; ++ i )
	{
		m_Sp.vPos = m_pDragon[i].GetTransformInfo()->vPos;
		m_Sp.fRad = m_pDragon[i].GetCharacterInfo()->dwRadius;

		if( IntersectSphere(ray,m_Sp) )
		{
			
			m_pPlayer->GetCharacterInfo()->m_pFocus = &m_pDragon[i];
			//MessageBox(g_hWnd, "das",0,0);
		}
		else
		{
			m_pPlayer->GetCharacterInfo()->m_pFocus  = NULL;
		}
		
	}

}

void cMainGame::ParseTerrainLocationInformation( char* szFileName )
{
	FILE* fp;
	fopen_s( &fp, szFileName, "r" );

	fseek( fp, 0, SEEK_SET );		//< 파일포인터 맨 앞으로 이동

	char szMeshName[1024];
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 fRot;
	float fScale;

	//< 파일 읽기
	while( !feof(fp) )
	{
		fscanf( fp, "%s		%f	%f%f	%f%f%f	%f", szMeshName, &vPos.x, &vPos.y, &vPos.z, &fRot.x, &fRot.y, &fRot.z, &fScale );
		if( m_pLevelMesh[szMeshName] == NULL )
		{
			m_pLevelMesh[szMeshName] = new cObjFileMesh();
			m_pObjParser->Parse( sgDevice, szMeshName, m_pLevelMesh[szMeshName] );
		}
		cTerrainObject* pNewObj = new cTerrainObject();
		if(( strcmp( szMeshName, "Resources/obj/grass.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/forestgrass01.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/forestgrass02.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/frozenmarshgrassobj01.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/tundragrassobj01.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/tundragrassobj04.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/volcanictundragrassobj01.obj" ) == 0 )
			||( strcmp( szMeshName, "Resources/obj/volcanictundragrassobj02.obj" ) == 0 )
			)
		{
			pNewObj->GetTransformInfo()->vPos = D3DXVECTOR3( rand()%7000-3500, 0.0f, rand()%7000-3500 );
			//pNewObj->GetTransformInfo()->vPos = 
			//	D3DXVECTOR3( rand()%(MAPSIZE*TILEWIDTH) - (MAPSIZE*TILEWIDTH)/2, 0.0f, 
			//	rand()%(MAPSIZE*TILEWIDTH) - (MAPSIZE*TILEWIDTH)/2 );
			fScale = rand()%5;
			((cTerrain*)m_pTerrain)->SetHeight(&pNewObj->GetTransformInfo()->vPos);
			if( strcmp( szMeshName, "Resources/obj/grass.obj" ) == 0 )
			{
				pNewObj->GetTransformInfo()->vPos.y -= 50.0f; //< 잔디 예외처리
			}
		}
		else
		{
			pNewObj->GetTransformInfo()->vPos = vPos;
		}
		fRot.x = fRot.x*3.14f/180.0f;
		fRot.y = fRot.y*3.14f/180.0f;
		fRot.z = fRot.z*3.14f/180.0f;
		pNewObj->GetTransformInfo()->vRot = fRot;
		pNewObj->GetTransformInfo()->vScale.x = fScale;
		pNewObj->GetTransformInfo()->vScale.y = fScale;
		pNewObj->GetTransformInfo()->vScale.z = fScale;
		pNewObj->Setup( m_pLevelMesh[szMeshName] );
		m_pLevel.push_back( pNewObj );
		m_pOctree->PushObject( m_pOctree->GetRootPtr(), m_pLevel.back() );
	}

	fclose( fp );
}

void cMainGame::SetupLoadingCamera( LPDIRECT3DDEVICE9 m_pd3dDevice )
{
	//< 뷰스페이스
	D3DXVECTOR3 vEye, vLook, vUp;
	D3DXMATRIXA16 matView;
	vEye		= D3DXVECTOR3( 100.0f, 200.0f, 500.0f );
	vLook		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	vUp		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEye, &vLook, &vUp );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	//< 투영
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, WINSIZEX/(float)WINSIZEY, 1.0f, 100000.0f );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void cMainGame::CreateSetupThread( void )
{
	m_hThread = CreateThread( NULL, 0, ProcessSetup, this, 0, &m_ThreadId );
	if( m_hThread == NULL )
	{
		std::cout << "셋업 스레드 생성 실패";
	}
	else
	{
		CloseHandle( m_hThread );
	}
}

void cMainGame::RenderLogo( void )
{
	while( true )
	{
//		m_pLoadingDevice->GetDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00202020, 1.0f, 0 );
		m_pLoadingDevice->GetDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );
    
		if( SUCCEEDED ( m_pLoadingDevice->GetDevice()->BeginScene() ))
		{
			char szBuf[128];
			sprintf(szBuf, "NOW LOADING :  %d %% , %s ", m_nProcess , m_szLoadFileName);
			RECT rc3 = { 10 , 10 , 0 ,0};
			m_pLoadingDevice->GetFont()->DrawText
				(NULL, szBuf, strlen(szBuf), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);

			D3DXMATRIXA16 matRoty;
			static float fRot = 0.0f;

			D3DXMatrixRotationY(&matRoty,fRot);
			fRot+= 0.01f;

			m_pLoadingDevice->GetDevice()->SetTransform(D3DTS_WORLD, &matRoty);
			m_pLoadingDevice->GetDevice()->SetStreamSource(0, m_pLogo->m_pVB,0,sizeof(stObjVertex));
			m_pLoadingDevice->GetDevice()->SetTexture( 0, m_pLogo->m_pTex  );
			m_pLoadingDevice->GetDevice()->SetMaterial( &m_pLogo->m_Mtrl );
			m_pLoadingDevice->GetDevice()->SetFVF(OBJ_D3DFVF);

			m_pLoadingDevice->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0,m_pLogo->m_Vertices.size()/3);
			m_pLoadingDevice->GetDevice()->EndScene();
		}
		m_pLoadingDevice->GetDevice()->Present( 0, 0, 0, 0 );

		if( m_bSetupComplete == true ) //< 쓰레드가 종료하면 탈출
		{
			return;
		}
	}
}