#include <iostream>
#include "TCPClient.h"

void main()
{
	TCPClient tcpClient;

	tcpClient.Init();

	tcpClient.Update();

	tcpClient.Clean();
}