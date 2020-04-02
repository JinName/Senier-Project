#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"
#include "GameDBManger.h"

PacketManager::PacketManager() : mStopFlag(false)
{
	InitializeCriticalSection(&mCS);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&mCS);
}

void PacketManager::Init()
{
	InitializeCriticalSection(&mCS);
}

void PacketManager::Clean()
{
	while (!mBufferQueue.empty())
	{
		mBufferQueue.pop();
	}

	DeleteCriticalSection(&mCS);
}

bool PacketManager::Enqueue(ClientSession* client, char* buffer)
{
	if (buffer == nullptr)
	{
		cout << "Enqueue : buffer is null" << endl;
		return false;
	}

	ClientPacket pack(client, buffer);

	EnterCS();
	mBufferQueue.push(pack);
	LeaveCS();

	return true;
}

bool PacketManager::Dequeue()
{


	return true;
}

void PacketManager::ProcessAllQueue()
{
	// ��Ŷť�� ���� ó���� ������ �ݺ�
	while (true)
	{
		if (mStopFlag) break;

		if (!mBufferQueue.empty())
		{
			// ���� ó���Ǿ���� ��Ŷ�� ���� �� ����
			EnterCS();

			ClientPacket pack = mBufferQueue.front();
			mBufferQueue.pop();	

			// ��Ŷ ��� Ȯ��
			PROTOCOL protocol = ParsingPacket(pack);

			// �������ݿ� ���� ��Ŷ ó��
			ProcessPacket(protocol, pack);

			LeaveCS();
		}

		if (mStopFlag) break;
	}
}

PROTOCOL PacketManager::ParsingPacket(ClientPacket pack)
{
	if (pack.mBuffer == nullptr)
	{
		cout << "parsing buffer is nullptr... return PROTOCOL::NONE" << endl;
		return PROTOCOL::NONE;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, pack.mBuffer, sizeof(SHEAD));

	if (head.mTransferToInGame)
	{
		InGameManager::GetInstance()->Enqueue(pack);

		return PROTOCOL::TRANFERED;
	}

	return (PROTOCOL)head.mCmd;
}

bool PacketManager::MakeSendPacket(ClientSession* client, char* data, DWORD dataBufferSize, PROTOCOL protocol)
{
	if (client == nullptr)
	{
		cout << "PacketManager::MakeSendPacket - client is nullptr.." << endl;
		return false;
	}

	// make packet
	char* p = client->GetSendOverlappedBuffer();

	// set header
	SHEAD head;
	head.mCmd = (unsigned char)protocol;
	head.mPacketSize = sizeof(SHEAD) + dataBufferSize;
	head.mTransferToInGame = false;

	memcpy(p, (char*)&head, sizeof(SHEAD));
	memcpy(p + sizeof(SHEAD), data, dataBufferSize);

	// set send overlapped
	client->SetSendOverlapped();

	return true;
}

void PacketManager::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
{
	switch (protocol)
	{
	case PROTOCOL::TRANFERED:
	{
		// �ٸ� ��Ŷó��������� ��Ŷ�� ���۵� ���
		//cout << "PACKET TRANSFER TO >> InGameManager" << endl;
		break;
	}

	case PROTOCOL::TEST_CHAT:
	{
		SCHAT chat;
		memset(&chat, 0, sizeof(SCHAT));
		memcpy(&chat, pack.mBuffer + sizeof(SHEAD), sizeof(SCHAT));
		cout << "Message From Client : " << chat.buf << endl;
		break;
	}

	case PROTOCOL::MATCH_RQ:
	{
		SMATCH match;
		memset(&match, 0, sizeof(SMATCH));
		memcpy(&match, pack.mBuffer + sizeof(SHEAD), sizeof(SMATCH));
		if (match.mInMatch == true)
		{
			MatchManager::GetInstance()->PushBackClient(pack.mSession);
		}

		break;
	}
	case PROTOCOL::LOGIN_RQ:
	{
		SLOGIN login;
		memset(&login, 0, sizeof(SLOGIN));
		memcpy(&login, pack.mBuffer + sizeof(SHEAD), sizeof(SLOGIN));

		bool loginResult = g_pGameDBManager->Login(login.mID, login.mPW);

		// ������ LOGIN_OK, ���н� LOGIN_DN ����
		if (loginResult)
		{
			PacketManager::GetInstance()->MakeSendPacket(pack.mSession, NULL, 0, PROTOCOL::LOGIN_OK);
			pack.mSession->Send();
		}
		else
		{
			PacketManager::GetInstance()->MakeSendPacket(pack.mSession, NULL, 0, PROTOCOL::LOGIN_DN);
			pack.mSession->Send();
		}

		break;
	}
	}
}