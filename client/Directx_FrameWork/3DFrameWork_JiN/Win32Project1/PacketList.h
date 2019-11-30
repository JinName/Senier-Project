#pragma once

#include "NetworkHeader.h"

/*
�¶��ΰ��ӿ��� ����ϴ� ���������� �⺻���� Ư���� �ֽ��ϴ�.

���� �ൿ�ϱ� ���ؼ� '��û'�ϴ°Ͱ�
�װͿ� ���ؼ� ������ '����'�ϴ°��� �ְ�.

���� �ƴ� �ٸ� ����� �ൿ�ߴٰ� '�뺸'�� ���°��� �ְ�
���ǿ� ������� ������ ������ ������ ���� '���'�� �ֽ��ϴ�.
('���' : ���� �� ������ HP�� �����ؾ� �Ѵ� -> Ŭ���̾�Ʈ�� ���������������)

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

	// CHAT
	TEST_CHAT,

	// LOGIN

	// MATCH
	MATCH_RQ, MATCH_RP, GAMESTART_CM,

	// PLAY
	MOVE_RQ	
};

typedef struct sPacket_Head
{
	UCHAR mCmd;
	DWORD mPacketSize;
} SHEAD;

typedef struct sPacket_Tail
{

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
};

struct SCHARACTER
{
	bool mAttack;
	bool mDamaged;
	float mPosX;
	float mPosY;
};