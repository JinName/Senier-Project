#pragma once

#include "stdafx.h"

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
};

struct SCHARACTER
{
	bool mAttack;
	float mPosX;
	float mPosY;
};