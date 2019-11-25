#pragma once

#include "stdafx.h"
#include <list>
#include "ClientSession.h"

/*
�÷��̾ 1:1 ��ġ�� ��ٸ��� ��Ī����
��ġ�� ���� ������� List �� push.
��Ī ��⸦ ����� �� �ֱ⶧���� Queue ���� List �� �����Ѵ�.
*/
class MatchManager
{
public:
	
	// push back to list
	bool Push_Back(ClientSession* client);

	// ��Ī�� ����ϴ� Thread ������ ���ư�
	// Match List ���� 2���̻� ������� ��� ��Ī�Ϸ� �� ���� ����
	void ProcessMatchList();

private:
	std::list<ClientSession*> mMatchWaitList;
};

