#pragma once

//< ��� ���� �κ�
#define CAMERA_SPEED_MOVE	  0.5f		 //< ī�޶��̵��� ���� �ӵ� �����Դϴ�~ �̰� ������
#define CAMERA_SPEED_ZOOM	  3.7f		 //< ī�޶� ���� �ܾƿ�
#define CAMERA_SPEED_ROTATION 0.001f		 //< ī�޶� ȸ�� �ӵ�


#define STARTX 0
#define STARTY 0

#define WINSIZEX 1600
#define WINSIZEY 900

#define GRIDSIZE 30
#define GRIDWIDTH 1

#define AXISLENGTH 20

#define MAPSIZE 128
#define TILEWIDTH 240 //< �ٴ� Ÿ�� ��ĭ ��
#define WORLDSIZE 10000//100000000

#define RECVBUFSIZE 64
#define SENDBUFSIZE 512

#define DAMAGESHOWMAX	5

#define GRID_D3DFVF ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define OBJ_D3DFVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1  )

#define IN
#define OUT
#define INOUT

#define ROCKMAX 1
#define ENEMYMAX 1
#define MOUNTAIN_01_MAX 1
#define MAXCLIENT 10

#define KEYMAX 256

//< enum

enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
enum PARTITION { PT_LTF, PT_RTF, PT_LTN, PT_RTN, PT_LBF, PT_RBF, PT_LBN, PT_RBN, PT_END }; //< Left/Right Top/Bottom Far/Near


enum PART { PART_HEADER, PART_LENGTH, PART_DATA, PART_END }; //< ��Ŷ ����
enum PACKET_OPTION { PO_STATUS, PO_TRANSFORM, PO_GENERATE, PO_DESTROY, PO_HPREDUCE, PO_END };	//< ��Ŷ ����
enum SENDER { SND_MANAGER, SND_PLAYER, SND_ENEMY };	//< �߽���ü
enum RECEIVER { RCV_MANAGER, RCV_PLAYER, RCV_ENEMY };	//< ������ü( �߽���ü�� ���� ��ġ��ų�� )
enum CHARACTERTYPE {CT_PLAYER , CT_MONSTER_DRAGON ,CT_END};
enum KEY { KEY_FORWARD, KEY_BACKWARD, KEY_LEFT, KEY_RIGHT, KEY_END };

enum LEVEL { LV_MOUNTAIN, LV_END }; 

//�̹�����
enum Image{
IMG_MOUSEPT,IMG_MENU,IMG_MENU1,
IMG_CORSSBOTTOM,IMG_CROSSLEFT,IMG_CORSSRIGHT,IMG_CORSSUP,
IMG_FONT1,IMG_FONT2,IMG_FONT3,IMG_FONT4,
IMG_MENUITEM,IMG_ITEMSELECT,
IMG_HP,IMG_HPAR,IMG_ENEMYHPAR,IMG_ENEMYHP,IMG_SHOW,IMG_SHOW2,IMG_SHOW3,
IMG_CROSSHEIR,IMG_RCROSSHEIR,
IMG_ITEM_MENU,IMG_SHOW4,
IMG_END};

// ����
enum tagSound{
SOUND_LODING,	// �ε�
SOUND_BATTLE,	// ����
SOUND_NORMAL,	// ����
SOUND_C_S_ATTACK,	// ������Ÿ��	character _ slash
SOUND_C_S_SWING,	// ��������ݽõ�	
SOUND_C_C_ATTACK,	// �б⹫��Ÿ�� character _ Crash
SOUND_C_C_SWING,	// �б⹫����ݽõ�
SOUND_C_RUN,// �ɸ��� �̵�
SOUND_C_DEATH,// �ɸ��� ����
SOUND_D_ATTACK,	// �� Ÿ��
SOUND_D_CRY,
SOUND_D_DEATH,	// ������
SOUND_D_WALK,	// ��ȱ�
SOUND_D_WING,	// �볯��
SOUND_U_OFF,	// �޴� ����
SOUND_U_ON,			
SOUND_U_CHOICE,// �޴� ����
SOUND_END};

//�ִϸ��̼�



enum PLAYER_ANIMATION {
	weapon_Dead,
	weapon_idle,
	weapon_runright,
	weapon_runleft,
	weapon_runforwardright,
	weapon_runforwardleft,
	weapon_runforward,
	weapon_runbackwardright,
	weapon_runbackwardleft,
	weapon_runbackward,
	weapon_attackright,
	weapon_attackpowerright,
	weapon_attackpowerleft,
	weapon_attackpowerbwd,
	weapon_attackpower,
	weapon_attackleft,

	ANIMATION_END
};

enum DRAGON_ANIMATION {
D_mtidle_hover,
D_mtforwardground,
D_mtfeatherloop,
D_mtfastforward_flap,
D_ground_injured,
D_ground_exhalefirebreath,
D_ground_bleedoutidle,
D_ground_bleedoutenter,
D_ground_bite,
D_ANI_END};


enum WEAPON_TYPE
{
	WEAPON_TWOHAND,
	WEAPON_HAMMER,
	WEAPON_TWOHAND_STEEL,
	WEAPON_BATTLE_AXE,
	WEAPON_EX_AXE,
	WEAPON_END
};

enum WIREFRAMECOLOR { WF_WHITE, WF_RED, WF_YELLOW, WF_BLUE, WF_GREEN, WF_END };