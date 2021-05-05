#include <string.h>
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
	memset(m_Buffer, 0, RINGBUF_SIZE);

	m_pBufferEnd = m_Buffer + RINGBUF_SIZE;

	m_pPrimaryCursor = m_Buffer;
	m_PrimarySize = 0;

	m_pSecondCursor = nullptr;
	m_SecondSize = 0;
}

CircularBuffer::~CircularBuffer()
{
	m_pBufferEnd = nullptr;

	m_pPrimaryCursor = nullptr;
	m_PrimarySize = 0;

	m_pSecondCursor = nullptr;
	m_SecondSize = 0;

	memset(m_Buffer, 0, RINGBUF_SIZE);
}

/// private functions ////////////////////////////////////////////////////////

/// <summary>
/// 두번째 커서를 버퍼의 시작지점으로 이동시킨다.
/// </summary>
void CircularBuffer::allocateSecondPointer()
{
	m_pSecondCursor = m_Buffer;
}

/// <summary>
/// 첫번째 커서부터 쌓인 데이터 끝지점에서 버퍼의 끝지점 까지 남은 공간을 반환한다.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::getAvailablePrimaryBufferSize()
{
	return (m_pBufferEnd - (m_pPrimaryCursor + m_PrimarySize));
}

/// <summary>
/// 두번째 커서부터 쌓인 데이터 끝지점에서 첫번째 커서까지 남은 공간을 반환한다.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::getAvailableSecondBufferSize()
{
	if (m_pSecondCursor == nullptr)
	{
		return 0;
	}
	else
	{
		return (m_pPrimaryCursor - (m_pSecondCursor + m_SecondSize));
	}
}

//////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 현재 삽입 가능한 버퍼내의 공간을 계산하여 반환.
/// 공간을 계산하며 커서의 위치에 따라 삽입 지점을 조절한다.
/// GetWritablePointer() 이전에 선행되어야 하는 함수이다.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::GetAvailableBufferSize()
{
	if (m_pSecondCursor != nullptr)
	{
		return getAvailableSecondBufferSize();
	}
	else
	{
		// Primary Buffer 의 여유공간이 MAX_PACKET_SIZE 보다 작을 경우
		// Second Pointer 에 전체 버퍼의 첫 지점을 할당하여
		// Second Pointer 부터 데이터를 삽입하도록 함
		if (MAX_PACKET_SIZE > getAvailablePrimaryBufferSize())
		{
			allocateSecondPointer();
			return getAvailableSecondBufferSize();
		}
		else
		{
			return getAvailablePrimaryBufferSize();
		}
	}
}

/// <summary>
/// 버퍼 삽입 가능한 지점을 반환.
/// GetAvailableBufferSize() 함수가 선행되어야 한다.
/// </summary>
/// <returns>	char*	</returns>
char* CircularBuffer::GetWritablePointer()
{
	if (m_pSecondCursor != nullptr)
	{
		return m_pSecondCursor + m_SecondSize;
	}
	else
	{
		return m_pPrimaryCursor + m_PrimarySize;
	}
}

/// <summary>
/// 현재 버퍼내에 데이터가 차지하고 있는 공간을 반환한다.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::GetWritedDataSize()
{
	return m_PrimarySize + m_SecondSize;
}

/// <summary>
/// Recv() 로 버퍼가 도착한 이후 삽입된 데이터 길이만큼을 업데이트한다.
/// </summary>
/// <param name="len">	현재 삽입된 데이터의 길이	</param>
void CircularBuffer::CommitDataSize(size_t len)
{
	if (m_pSecondCursor != nullptr)
	{
		m_SecondSize += len;
	}
	else
	{
		m_PrimarySize += len;
	}
}

/// <summary>
/// Peek() : 버퍼 내의 데이터에서 Head 만을 확인하여 Pop 해야하는 데이터의 길이를 확인한다.
/// </summary>
/// <param name="outBuffer">	Peek 결과 값	</param>
/// <param name="len">			Peek 할 길이	</param>
/// <returns>					bool type		</returns>
bool CircularBuffer::Peek(char* outBuffer, size_t len)
{
	if (GetWritedDataSize() < len)
	{
		// 에러가 발생한 클래스, 함수, 원인 등을 출력
		printError(this, __func__, "writed buffer size less than require");
		return false;
	}

	size_t cnt = len;

	// Primary Buffer 에서 Peek 하는 경우
	if (m_PrimarySize > 0)
	{
		size_t peekSize = (cnt > m_PrimarySize) ? m_PrimarySize : cnt;

		// 버퍼 복사
		memcpy(outBuffer, m_pPrimaryCursor, peekSize);

		cnt -= peekSize;
	}

	// Primary Buffer 에서 Peek 이후 Second Buffer 에서 마저 Peek 해야 하는 경우
	if (cnt > 0 && m_SecondSize > 0)
	{
		size_t peekSize = (cnt > m_SecondSize) ? m_SecondSize : cnt;

		// 버퍼 복사
		memcpy(outBuffer + (len - cnt), m_pSecondCursor, peekSize);

		cnt -= peekSize;
	}

	return true;
}

/// <summary>
/// Pop() : 버퍼에서 원하는 길이만큼 데이터를 추출하고 삭제한다.
/// </summary>
/// <param name="outBuffer">	Pop 결과 값		</param>
/// <param name="len">			Pop 할 길이		</param>
/// <returns>					bool type		</returns>
bool CircularBuffer::Pop(char* outBuffer, size_t len)
{
	// Pop 하려는 데이터 길이가 버퍼에 담겨있는 데이터 길이보다 클 경우,
	// return false;
	if (GetWritedDataSize() < len)
	{
		// 에러가 발생한 클래스, 함수, 원인 등을 출력
		printError(this, __func__, "writed buffer size less than require");
		return false;
	}		

	size_t cnt = len;

	// Primary Buffer 에서 Pop 하는 경우
	if (m_PrimarySize > 0)
	{
		size_t popSize = (cnt > m_PrimarySize) ? m_PrimarySize : cnt;

		// 버퍼 복사
		memcpy(outBuffer, m_pPrimaryCursor, popSize);

		// 내용 삭제
		memset(m_pPrimaryCursor, 0, popSize);

		m_PrimarySize -= popSize;
		m_pPrimaryCursor += popSize;
		cnt -= popSize;
	}

	// Primary Buffer 에서 Pop 이후 Second Buffer 에서 마저 Pop 해야 하는 경우
	if (cnt > 0 && m_SecondSize > 0)
	{
		size_t popSize = (cnt > m_SecondSize) ? m_SecondSize : cnt;

		// 버퍼 복사
		memcpy(outBuffer + (len - cnt), m_pSecondCursor, popSize);

		// 내용 삭제
		memset(m_pSecondCursor, 0, popSize);

		m_SecondSize -= popSize;
		m_pSecondCursor += popSize;
		cnt -= popSize;
	}

	// Pop 이후 Primary Buffer 는 비어있고	
	if (m_PrimarySize == 0)
	{
		// Second Buffer 에 데이터가 남아있을 경우
		if (m_SecondSize > 0)
		{
			// Second Buffer Data 를 시작지점으로 복사
			if (m_pSecondCursor != m_Buffer)
				memcpy(m_Buffer, m_pSecondCursor, m_SecondSize);

			// Primary Buffer Cursor 를 시작지점으로 변경
			// + Primary Buffer 지점에 삽입되어있는 사이즈 변경
			m_pPrimaryCursor = m_Buffer;
			m_PrimarySize = m_SecondSize;
		}
		// Second Buffer 도 비어있는 경우
		else
		{
			// Primary Buffer Cursor 를 시작지점으로 변경
			m_pPrimaryCursor = m_Buffer;
			m_PrimarySize = 0;
		}

		// Cursor 이동이 끝나면 Second Cursor 초기화
		m_pSecondCursor = nullptr;
		m_SecondSize = 0;
	}

	return true;
}