#pragma once

#include "NetworkHeader.h"

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

	// MATCH
	MATCH_RQ, MATCH_RP,

	// GAME START, END
	GAMESTART_CM, GAMEEND_CM, INITCOMPLETE_RQ,

	// PLAY
	P1_MOVE_RQ, P1_MOVE_RP, P2_MOVE_RQ, P2_MOVE_RP,
	MOVE_RQ, MOVE_RP, BRCAST_MOVE_RP,
	JUMP_RQ,

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
	sPacket_Head() : mTransferToInGame(false) {}

	UCHAR mCmd;
	DWORD mPacketSize;
	bool mTransferToInGame;
} SHEAD;

typedef struct sPacket_Tail
{
	bool end;
} STAIL;

struct SCHAT
{
	char buf[MAX_MSG_LEN];
};

struct SMATCH
{
	bool mInMatch;
};

struct SGAMESTART
{
	bool mStart;
	int mPlayerIndex;	// 0 : 1p, 1 : 2p
	VECTOR3 mStartPosition[2];
};

struct SGAMEEND
{
	GAMEEND_STATE mGameEndState;
};

struct SCHARACTER
{
	SCHARACTER() : mLeft(false), mRight(false), mKeyDownSpace(false), mAttack(false), mDamaged(false) {}

	int mPlayerIndex;

	float mPosX;
	float mPosY;
	float mDirectionX;
	CHARACTER_STATE mCharState;

	bool mLeft;
	bool mRight;
	bool mKeyDownSpace;
	bool mAttack;
	bool mDamaged;
};

struct SINITCOMPLETE
{
	int mPlayerIndex;
	bool mComplete;
};