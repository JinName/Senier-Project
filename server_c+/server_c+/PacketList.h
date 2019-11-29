#pragma once

#include "stdafx.h"

/*
온라인게임에서 사용하는 프로토콜은 기본적인 특성이 있습니다.

내가 행동하기 위해서 '요청'하는것과
그것에 대해서 서버가 '응답'하는것이 있고.

내가 아닌 다른 사람이 행동했다고 '통보'가 오는것이 있고
자의에 상관없이 서버로 부터의 데이터 수정 '명령'이 있습니다.
('명령'은..예를 들자면.. 남이 날 때려서 HP를 수정해야 한다든지 이런거..)

기본적으로 이 4가지에서 파생되거나, 의미가 혼합됩니다.
- Request, Reply, Notify, Command(compulsion)
> RQ, RP, NF, CM

그리고 이 패킷들의 특성은 다음과 같습니다.

RQ, RP, CM : 행동내용과 패러미터만 있으면 된다.
NF         : '누구인가'와 행동내용과 패러미터만 있으면 된다.

RQ      : 주로 서버에서만 받는다.
RP      : 주로 클라이언트에서만 받는다.
CM, NF  : 반드시 클라이언트만 받는다. (분산서버 예외)
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