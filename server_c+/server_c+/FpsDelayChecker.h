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

	FpsDelayChecker(int fps);
	~FpsDelayChecker();

	// ���� ��ġ��ŭ Sleep() �Լ� ����
	void DoDelay();	

	// �ڵ� ����Ÿ�� üũ
	void StartCodeRuntimeCheck();
	void EndCodeRuntimeCheck();

private:

	// �ڵ� ����Ÿ�� üũ
	Timer m_Timer;

	DWORD m_ResultDelayTime;

	int m_QuotientClock;			// fps ��� �� ��
	float m_RemainClock;			// fps ��� �� �Ҽ��� ������

	float m_TotalRemainTime;		// sleep �ϰ� ���� �Ҽ����� ���ؼ� 1�� ������� delaytime + 1

	// �ʱ�ȭ
	void init(int fps);

	// ������ �ð� ���
	void calcDelayTime();

	DWORD getCodeRuntime();

};

