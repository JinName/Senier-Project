#pragma once

#include "stdafx.h"

enum PROTOCOL
{
	// CHAT
	TEST_CHAT
	
	// LOGIN

	// PLAY
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