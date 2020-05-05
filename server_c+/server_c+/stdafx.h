#pragma once
#define _WINSOCKAPI_

#include <stdio.h>
#include <stdlib.h>

#include <process.h>
#include <WinSock2.h>
#include <Windows.h>

#include <map>

#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#include "Logger.h"

#define PORT_NUM		10200
#define BLOG_SIZE		5
#define MAX_MSG_LEN		256
#define LOCAL_IP		"127.0.0.1"
#define MAX_BUFSIZE		1024

struct VECTOR2
{
	float x;
	float y;
};

struct VECTOR3
{
	VECTOR3() {}
	VECTOR3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;
};