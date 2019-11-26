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
	// CHAT
	TEST_CHAT,
	
	// LOGIN

	// MATCH
	MATCH_RQ, MATCH_RP,

	// PLAY
	GAMESTART_CM,

	// default
	NONE
};

typedef struct sPacket_Head
{
	UCHAR mCmd;
	DWORD mDataSize;	
} SHEAD;

typedef struct sPacket_Tail
{

} STAIL;

struct SCHAT
{
	char buf[MAX_MSG_LEN];
};

struct SGAMEREADY
{
	bool mReady;
};

struct SGAMESTART
{
	bool mStart;
};

struct SCHARACTER
{
	float mPosX;
	float mPosY;
};