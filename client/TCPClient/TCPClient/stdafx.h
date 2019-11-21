#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
using namespace std;

#define PORT_NUM      10200
#define MAX_MSG_LEN   256
//#define SERVER_IP     "192.168.34.50"//서버 IP 주소
#define SERVER_IP     "127.0.0.1"//서버 IP 주소
#define MAX_BUFSIZE   4096

#pragma comment(lib, "ws2_32.lib")

//void ErrorHandling(char* message);
//
//void ErrorHandling(char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}

// enum I/O type
enum class IOTYPE
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT,
	IO_DISCONNECT
};

/*
소켓 버퍼 정보를 구조체로 만드는데 이때 WSABUF 와 overlapped 를 포함한다.
WSABUF 는WSASend 나WSARecv 함수의 인자로 전달되는 버퍼에 사용되는 구조체 이기에 포함 되고
overlapped 구조체 변수를 넣어주는건 현재 완료된 입출력 정보를 얻어 낼때 사용 된다.
*/
typedef struct sOverlappedSocket : public OVERLAPPED
{
	sOverlappedSocket()
	{
		// init
		memset(mBuffer, 0, MAX_BUFSIZE);
		memset(&mWSABuf, 0, sizeof(WSABUF));
	}
	IOTYPE					mIOType;
	WSABUF					mWSABuf;
	char					mBuffer[MAX_BUFSIZE];
} SOVERLAPPED;