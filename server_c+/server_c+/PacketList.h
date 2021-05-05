#pragma once

#include "stdafx.h"

#define MAX_LEN_USER_INFO 32

/*
�¶��ΰ��ӿ��� ����ϴ� ���������� �⺻���� Ư���� �ֽ��ϴ�.

���� �ൿ�ϱ� ���ؼ� '��û'�ϴ°Ͱ�
�װͿ� ���ؼ� ������ '����'�ϴ°��� �ְ�.

���� �ƴ� �ٸ� ����� �ൿ�ߴٰ� '�뺸'�� ���°��� �ְ�
���ǿ� ������� ������ ������ ������ ���� '���'�� �ֽ��ϴ�.
('���'��..���� ���ڸ�.. ���� �� ������ HP�� �����ؾ� �Ѵٵ��� �̷���..)

�⺻������ �� 4�������� �Ļ��ǰų�, �ǹ̰� ȥ�յ˴ϴ�.
- Request, Reply, Notify, Command(compulsion)
> RQ, RP, NF, CM

�׸��� �� ��Ŷ���� Ư���� ������ �����ϴ�.

RQ, RP, CM : �ൿ����� �з����͸� ������ �ȴ�.
NF         : '�����ΰ�'�� �ൿ����� �з����͸� ������ �ȴ�.

RQ      : �ַ� ���������� �޴´�.
RP      : �ַ� Ŭ���̾�Ʈ������ �޴´�.
CM, NF  : �ݵ�� Ŭ���̾�Ʈ�� �޴´�. (�л꼭�� ����)
*/

enum class PROTOCOL
{
	// default
	NONE,

	// transfered to another thread
	TRANFERED,

	// CHAT
	TEST_CHAT,
	
	// LOGIN
	LOGIN_RQ, LOGIN_OK, LOGIN_DN,

	// MATCH
	MATCH_RQ, MATCH_RP, 
	
	// GAME START, END
	GAMESTART_CM, GAMEEND_CM, INITCOMPLETE_RQ,
	PLAYER_DIE_RQ,

	// PLAY
	P1_MOVE_RQ, P1_MOVE_RP, P2_MOVE_RQ, P2_MOVE_RP,
	MOVE_RQ, MOVE_RP, BRCAST_MOVE_RP,
	JUMP_RQ,
	CRASH_RQ,

	// UPDATE
	UPDATE_NF
};

enum class CHARACTER_STATE
{
	STAND, UP, DOWN, LEFT, ATTACK, JUMP, RIGHT
};

enum class GAMEEND_STATE
{
	DISCONNECTED, P1_WIN, P2_WIN
};

typedef struct sPacket_Head
{
	sPacket_Head() : m_IsTransferToInGame(false) {}

	UCHAR m_Cmd;
	DWORD m_PacketSize;
	bool m_IsTransferToInGame;
} SHEAD;

typedef struct sPacket_Tail
{
	bool m_IsEnd;
} STAIL;

struct SCHAT
{
	char m_Buffer[MAX_MSG_LEN];
};

struct SMATCH
{
	bool m_IsMatch;
};

struct SGAMESTART // 12bit
{
	bool m_IsStart;
	int m_PlayerIndex;	// 0 : 1p, 1 : 2p
	VECTOR3 m_StartPosition[2];
};

struct SGAMEEND
{
	GAMEEND_STATE m_GameEndState;
};

struct SCHARACTER
{
	SCHARACTER() : m_PlayerIndex(-1), m_IsLeft(false), m_IsRight(false), m_IsKeyDownSpace(false), m_IsAttack(false), m_IsDamaged(false), m_CharState(CHARACTER_STATE::STAND), m_PosX(0.0f), m_PosY(0.0f), m_DirectionX(0.0f) {}

	int m_PlayerIndex;

	float m_PosX;
	float m_PosY;
	float m_DirectionX;
	CHARACTER_STATE m_CharState;

	bool m_IsLeft;
	bool m_IsRight;
	bool m_IsKeyDownSpace;
	bool m_IsAttack;
	bool m_IsDamaged;
};

struct SINGAMEINFO
{
	SINGAMEINFO() {}
	SINGAMEINFO(SCHARACTER _player1, SCHARACTER _player2)
	{
		m_CharInfo[0] = _player1;
		m_CharInfo[1] = _player2;
	}
	SCHARACTER m_CharInfo[2];
};

struct SINITCOMPLETE
{
	int m_PlayerIndex;
	bool m_IsComplete;
};

// login
struct SLOGIN
{
	char m_ID[MAX_LEN_USER_INFO];
	char m_PW[MAX_LEN_USER_INFO];
};

// crash
struct SCRASH
{
	int m_PlayerIndex;
};

// player die
struct SPLAYERDIE
{
	int m_PlayerIndex;
};