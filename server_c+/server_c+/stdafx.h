#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <process.h>
#include <WinSock2.h>

#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT_NUM		10200
#define BLOG_SIZE		5
#define MAX_MSG_LEN		256
#define LOCAL_IP		"127.0.0.1"