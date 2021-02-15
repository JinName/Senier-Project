#pragma once
#include <Windows.h>
#include <time.h>

#include "Timer.h"

// ���ϴ� ������Ʈ �ֱ� (fps)�� �Է¹޾Ƽ�
// �ش� fps �� �´� sleep �ֱ⸦ ������ִ� Ŭ����
// ���� �����̷� sleep ���� �����ϴ� ���� ��ǥ
// 1. �ڵ� ����Ÿ�� üũ
// 2. sleep�� �Ұ����� �Ҽ����� ���� ���

class FpsDelayChecker
{
public:

	FpsDelayChecker(int _fps);
	~FpsDelayChecker();

	// ���� ��ġ��ŭ Sleep() �Լ� ����
	void DoDelay();	

	// �ڵ� ����Ÿ�� üũ
	void StartCodeRuntimeCheck();
	void EndCodeRuntimeCheck();

private:

	// �ڵ� ����Ÿ�� üũ
	Timer m_Timer;

	DWORD m_dwResultDelayTime;

	int m_iQuotientClock;			// fps ��� �� ��
	float m_fRemainClock;			// fps ��� �� �Ҽ��� ������

	float m_fTotalRemainTime;		// sleep �ϰ� ���� �Ҽ����� ���ؼ� 1�� ������� delaytime + 1

	// �ʱ�ȭ
	void init(int _fps);

	// ������ �ð� ���
	void calcDelayTime();

	DWORD getCodeRuntime();

};

