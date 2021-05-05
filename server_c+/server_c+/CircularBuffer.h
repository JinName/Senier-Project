#pragma once
#include "CommonFunction.h"

#define MAX_PACKET_SIZE 540
#define THREAD_COUNT 12
#define	RINGBUF_SIZE 6480

class CircularBuffer
{
public:
	CircularBuffer();
	~CircularBuffer();

	// ���� ���ۿ� ��� ������ ���� ��ȯ + ���� ���������� �����ϸ� ������ �������� ����
	size_t GetAvailableBufferSize();

	// ���� ���ۿ� ���� ������ ������ �����ͷ� ��ȯ
	char* GetWritablePointer();

	// ���ۿ� ����ִ� �� ������ ����
	size_t GetWritedDataSize();

	// �Ű������� �Էµ� ���̸�ŭ �����͸� �̵�
	// ���Ե� ������ ������ �ݿ�
	void CommitDataSize(size_t len);

	// �����͸� �������� �ʰ� ���̸�ŭ�� ��ȯ�Ͽ� Ȯ����
	bool Peek(char* outBuffer, size_t len);

	// �����͸� ���� + ����
	bool Pop(char* outBuffer, size_t len);

private:
	char m_Buffer[RINGBUF_SIZE];
	char* m_pBufferEnd;

	// ���۸� �����͸� �ڷ� �и鼭 ����ϴٰ�
	// ���� ���� ������ MAX_PACKET_SIZE ���� ������,
	// SecondPointer �� ������ ù �������� �ٽ� �����͸� �޴´�.
	char* m_pPrimaryCursor;
	size_t m_PrimarySize;

	char* m_pSecondCursor;
	size_t m_SecondSize;

	// ù��° �ι�° ������ ����� ���
	size_t getAvailablePrimaryBufferSize();
	size_t getAvailableSecondBufferSize();

	// �ι�° �����͸� ���� ù ������ �Ҵ�
	void allocateSecondPointer();
};