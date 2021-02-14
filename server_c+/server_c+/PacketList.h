#pragma once

#include "stdafx.h"

#define MAX_LEN_USER_INFO 32

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

struct SGAMESTART // 12bit
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
	SCHARACTER() : mPlayerIndex(-1), mLeft(false), mRight(false), mKeyDownSpace(false), mAttack(false), mDamaged(false), mCharState(CHARACTER_STATE::STAND), mPosX(0.0f), mPosY(0.0f), mDirectionX(0.0f) {}

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

// login
struct SLOGIN
{
	char mID[MAX_LEN_USER_INFO];
	char mPW[MAX_LEN_USER_INFO];
};

// crash
struct SCRASH
{
	int mPlayerIndex;
};

// player die
struct SPLAYERDIE
{
	int mPlayerIndex;
};