#pragma once

#include "stdafx.h"

enum class PROTOCOL
{
	// CHAT
	TEST_CHAT,
	
	// LOGIN

	// PLAY

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
	SHEAD mHead;
	char buf[MAX_MSG_LEN];
};

struct SCHARACTER
{
	SHEAD mHead;
	float mPosX;
	float mPosY;
};