#pragma once

//< ��� ���� �κ�

#define BUFSIZE 64
#define MAXCLIENT 5
#define MAXENEMY 1

//< enum

enum SENDER { SND_MANAGER, SND_PLAYER, SND_ENEMY };	//< �߽���ü
enum RECEIVER { RCV_MANAGER, RCV_PLAYER, RCV_ENEMY };	//< ������ü( �߽���ü�� ���� ��ġ��ų�� )
enum PART { PART_HEADER, PART_LENGTH, PART_DATA, PART_END }; //< ��Ŷ ����
enum PACKET_OPTION { PO_STATUS, PO_TRANSFORM, PO_GENERATE, PO_DESTROY, PO_HPREDUCE, PO_END };	//< ��Ŷ ����

enum DIRECTION {
	DRT_BACK_RIGHT ,DRT_BACK_LEFT ,DRT_BACK ,DRT_RIGHT,DRT_LEFT ,DRT_FRONT_RIGHT ,DRT_FRONT_LEFT ,DRT_FRONT, DRT_END};

enum PLAYER_ANIMATION {
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
	weapon_attackpower,
	weapon_attackleft,
	weapon_attackforwardsprint,

	ANIMATION_END
};

// AI ��

enum CHARACTERTYPE {MONSTER_DRAGON ,CT_END};

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