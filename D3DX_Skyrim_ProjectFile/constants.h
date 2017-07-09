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

#define KEYMAX 256

//< enum

enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
enum PARTITION { PT_LTF, PT_RTF, PT_LTN, PT_RTN, PT_LBF, PT_RBF, PT_LBN, PT_RBN, PT_END }; //< Left/Right Top/Bottom Far/Near

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

enum WIREFRAMECOLOR { WF_WHITE, WF_RED, WF_YELLOW, WF_BLUE, WF_GREEN, WF_END };