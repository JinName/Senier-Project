#pragma once

#define MAX_PACKET_SIZE 540
#define THREAD_COUNT 12

#define	RINGBUF_SIZE 6480

class CircularBuffer
{
public:
	CircularBuffer();
	~CircularBuffer();

	// 현재 버퍼에 사용 가능한 공간 반환
	size_t GetAvailableBufferSize();

	// 현재 버퍼에 삽입 가능한 지점을 포인터로 반환
	char* GetWritablePointer();

	// 버퍼에 쌓여있는 데이터 시작 지점 반환
	char* GetBufferStartPointer();
	// 시작 지점부터의 데이터 길이 반환
	size_t GetBufferStartDataSize();
	// 버퍼에 들어있는 총 데이터 길이
	size_t GetWritedDataSize();

	// 매개변수에 입력된 길이만큼 포인터를 이동
	void CommitDataSize(size_t _len);

	// 사용이 끝난 데이터를 삭제
	void RemoveData(size_t _len);

	// 데이터를 삭제하지 않고 길이만큼만 반환하여 확인함
	bool Peek(char* _outBuffer, size_t _len);

	// 데이터를 추출 + 삭제
	bool Pop(char* _outBuffer, size_t _len);

private:
	char m_cBuffer[RINGBUF_SIZE];
	char* m_cpBufferEnd;

	// 버퍼를 포인터를 뒤로 밀면서 사용하다가
	// 일정 범위 이상을 넘어가면,
	// SecondPointer 로 버퍼의 첫 지점부터 다시 데이터를 복사한다.
	char* m_cpPrimaryPointer;
	size_t m_dwPrimarySize;

	char* m_cpSecondPointer;
	size_t m_dwSecondSize;

	// 첫번째 두번째 버퍼의 빈공간 계산
	size_t getAvailablePrimaryBufferSize();
	size_t getAvailableSecondBufferSize();

	// 두번째 포인터를 버퍼 첫 지점을 할당
	void allocateSecondPointer();

	// 삽입 / 삭제 로 인해 뒤로 밀린 포인터의 위치를 버퍼 시작점으로부터 계산
	size_t getBeforePrimaryBufferSize();
};