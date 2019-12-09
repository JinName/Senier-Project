#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <process.h>
#include <WinSock2.h>

#include <map>

#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#define PORT_NUM		10200
#define BLOG_SIZE		5
#define MAX_MSG_LEN		256
#define LOCAL_IP		"127.0.0.1"
#define MAX_BUFSIZE		1024