//#pragma once
//#include <stdio.h>
//#include <stdlib.h>
//#include <WinSock2.h>
//#include <process.h>
//#include <iostream>
//using namespace std;
//
//#define PORT_NUM      10200
//#define MAX_MSG_LEN   256
////#define SERVER_IP     "192.168.34.50"//���� IP �ּ�
//#define SERVER_IP     "127.0.0.1"//���� IP �ּ�
//#define MAX_BUFSIZE   4096
//
//#pragma comment(lib, "ws2_32.lib")
//
////void ErrorHandling(char* message);
////
////void ErrorHandling(char* message)
////{
////	fputs(message, stderr);
////	fputc('\n', stderr);
////	exit(1);
////}
//
//// enum I/O type
//enum IOType
//{
//	IO_NONE,
//	IO_SEND,
//	IO_RECV,
//	IO_RECV_ZERO,
//	IO_ACCEPT,
//	IO_DISCONNECT
//};
//
///*
//���� ���� ������ ����ü�� ����µ� �̶� WSABUF �� overlapped �� �����Ѵ�.
//WSABUF ��WSASend ��WSARecv �Լ��� ���ڷ� ���޵Ǵ� ���ۿ� ���Ǵ� ����ü �̱⿡ ���� �ǰ�
//overlapped ����ü ������ �־��ִ°� ���� �Ϸ�� ����� ������ ��� ���� ��� �ȴ�.
//*/
//typedef struct sOverlappedSocket : public OVERLAPPED
//{
//	sOverlappedSocket( )
//	{
//		// init
//		memset(&mBuffer, 0, MAX_BUFSIZE);
//		memset(&mWSABuf, 0, sizeof(WSABUF));
//	}
//
//	IOType					mIOType;
//	WSABUF					mWSABuf;
//	char					mBuffer[MAX_BUFSIZE];
//} SOVERLAPPED;

//#pragma once
//
//#include "stdafx.h"
//
//enum class PROTOCOL
//{
//	// CHAT
//	TEST_CHAT
//	
//	// LOGIN
//
//	// PLAY
//};
//
//typedef struct sPacket_Head
//{
//	UCHAR mCmd;
//	DWORD mDataSize;	
//} SHEAD;
//
//typedef struct sPacket_Tail
//{
//
//} STAIL;
//
//struct SCHAT
//{
//	SHEAD mHead;
//	char buf[MAX_MSG_LEN];
//};