#pragma once

#define MAX_PACKET_SIZE 540
#define THREAD_COUNT 12

#define	RINGBUF_SIZE 6480

class CircularBuffer
{
public:
	CircularBuffer();
	~CircularBuffer();

	// ���� ���ۿ� ��� ������ ���� ��ȯ
	size_t GetAvailableBufferSize();

	// ���� ���ۿ� ���� ������ ������ �����ͷ� ��ȯ
	char* GetWritablePointer();

	// ���ۿ� �׿��ִ� ������ ���� ���� ��ȯ
	char* GetBufferStartPointer();
	// ���� ���������� ������ ���� ��ȯ
	size_t GetBufferStartDataSize();
	// ���ۿ� ����ִ� �� ������ ����
	size_t GetWritedDataSize();

	// �Ű������� �Էµ� ���̸�ŭ �����͸� �̵�
	void CommitDataSize(size_t _len);

	// ����� ���� �����͸� ����
	void RemoveData(size_t _len);

	// �����͸� �������� �ʰ� ���̸�ŭ�� ��ȯ�Ͽ� Ȯ����
	bool Peek(char* _outBuffer, size_t _len);

	// �����͸� ���� + ����
	bool Pop(char* _outBuffer, size_t _len);

private:
	char m_cBuffer[RINGBUF_SIZE];
	char* m_cpBufferEnd;

	// ���۸� �����͸� �ڷ� �и鼭 ����ϴٰ�
	// ���� ���� �̻��� �Ѿ��,
	// SecondPointer �� ������ ù �������� �ٽ� �����͸� �����Ѵ�.
	char* m_cpPrimaryPointer;
	size_t m_dwPrimarySize;

	char* m_cpSecondPointer;
	size_t m_dwSecondSize;

	// ù��° �ι�° ������ ����� ���
	size_t getAvailablePrimaryBufferSize();
	size_t getAvailableSecondBufferSize();

	// �ι�° �����͸� ���� ù ������ �Ҵ�
	void allocateSecondPointer();

	// ���� / ���� �� ���� �ڷ� �и� �������� ��ġ�� ���� ���������κ��� ���
	size_t getBeforePrimaryBufferSize();
};