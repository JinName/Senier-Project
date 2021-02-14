#include <string.h>
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
	memset(m_cBuffer, 0, RINGBUF_SIZE);

	m_cpBufferEnd = m_cBuffer + RINGBUF_SIZE;

	m_cpPrimaryPointer = m_cBuffer;
	m_dwPrimarySize = 0;

	m_cpSecondPointer = nullptr;
	m_dwSecondSize = 0;
}

CircularBuffer::~CircularBuffer()
{
	m_cpBufferEnd = nullptr;

	m_cpPrimaryPointer = nullptr;
	m_dwPrimarySize = 0;

	m_cpSecondPointer = nullptr;
	m_dwSecondSize = 0;

	memset(m_cBuffer, 0, RINGBUF_SIZE);
}

/// private functions ////////////////////////////////////////////////////////
void CircularBuffer::allocateSecondPointer()
{
	m_cpSecondPointer = m_cBuffer;
}

size_t CircularBuffer::getAvailablePrimaryBufferSize()
{
	return (m_cpBufferEnd - (m_cpPrimaryPointer + m_dwPrimarySize));
}

size_t CircularBuffer::getAvailableSecondBufferSize()
{
	if (m_cpSecondPointer == nullptr)
	{
		return 0;
	}
	else
	{
		return (m_cpPrimaryPointer - (m_cpSecondPointer + m_dwSecondSize));
	}
}

size_t CircularBuffer::getBeforePrimaryBufferSize()
{
	return m_cpPrimaryPointer - m_cBuffer;
}
//////////////////////////////////////////////////////////////////////////////////

size_t CircularBuffer::GetAvailableBufferSize()
{
	if (m_cpSecondPointer != nullptr)
	{
		return getAvailableSecondBufferSize();
	}
	else
	{
		if (getBeforePrimaryBufferSize() > getAvailablePrimaryBufferSize())
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

char* CircularBuffer::GetWritablePointer()
{
	if (m_cpSecondPointer != nullptr)
	{
		return m_cpSecondPointer + m_dwSecondSize;
	}
	else
	{
		return m_cpPrimaryPointer + m_dwPrimarySize;
	}
}

char* CircularBuffer::GetBufferStartPointer()
{
	if (m_dwPrimarySize > 0)
	{
		return m_cpPrimaryPointer;
	}
	else
	{
		return m_cpSecondPointer;
	}
}

size_t CircularBuffer::GetBufferStartDataSize()
{
	if (m_dwPrimarySize > 0)
	{
		return m_dwPrimarySize;
	}
	else
	{
		return m_dwSecondSize;
	}
}

size_t CircularBuffer::GetWritedDataSize()
{
	return m_dwPrimarySize + m_dwSecondSize;
}

void CircularBuffer::CommitDataSize(size_t _len)
{
	if (m_cpSecondPointer != nullptr)
	{
		m_dwSecondSize += _len;
	}
	else
	{
		m_dwPrimarySize += _len;
	}
}

void CircularBuffer::RemoveData(size_t _len)
{
	size_t cnt = _len;

	// Primary Buffer 에서 삭제하는 경우
	if (m_dwPrimarySize > 0)
	{
		size_t removeSize = (cnt > m_dwPrimarySize) ? m_dwPrimarySize : cnt;
		m_dwPrimarySize -= removeSize;
		m_cpPrimaryPointer += removeSize;
		cnt -= removeSize;
	}

	// Primary Buffer 에서 삭제 후 Second Buffer 에서 마저 삭제해야 하는 경우
	if (cnt > 0 && m_dwSecondSize > 0)
	{
		size_t removeSize = (cnt > m_dwSecondSize) ? m_dwSecondSize : cnt;
		m_dwSecondSize -= removeSize;
		m_cpSecondPointer += removeSize;
		cnt -= removeSize;
	}

	// 삭제 후 Primary Buffer 는 비어있고, Second Buffer 에 데이터가 남아있을 경우
	// Second Buffer Data -> Primary Buffer
	if (m_dwPrimarySize == 0)
	{
		if (m_dwSecondSize > 0)
		{
			if (m_cpSecondPointer != m_cBuffer)
				memmove(m_cBuffer, m_cpSecondPointer, m_dwSecondSize);

			m_cpPrimaryPointer = m_cBuffer;
			m_dwPrimarySize = m_dwSecondSize;

			m_cpSecondPointer = nullptr;
			m_dwSecondSize = 0;
		}
		else
		{
			m_cpSecondPointer = nullptr;
			m_dwSecondSize = 0;

			m_cpPrimaryPointer = m_cBuffer;
			m_dwPrimarySize = 0;
		}
	}
}

bool CircularBuffer::Peek(char* _outBuffer, size_t _len)
{
	if (GetWritedDataSize() < _len)
		return false;

	size_t cnt = _len;

	// Primary Buffer 에서 Pop 하는 경우
	if (m_dwPrimarySize > 0)
	{
		size_t peekSize = (cnt > m_dwPrimarySize) ? m_dwPrimarySize : cnt;

		// 버퍼 복사
		memcpy(_outBuffer, m_cpPrimaryPointer, peekSize);

		cnt -= peekSize;
	}

	// Primary Buffer 에서 삭제 후 Second Buffer 에서 마저 삭제해야 하는 경우
	if (cnt > 0 && m_dwSecondSize > 0)
	{
		size_t peekSize = (cnt > m_dwSecondSize) ? m_dwSecondSize : cnt;

		// 버퍼 복사
		memcpy(_outBuffer + (_len - cnt), m_cpSecondPointer, peekSize);

		cnt -= peekSize;
	}

	return true;
}

bool CircularBuffer::Pop(char* _outBuffer, size_t _len)
{
	if (GetWritedDataSize() < _len)
		return false;

	size_t cnt = _len;

	// Primary Buffer 에서 Pop 하는 경우
	if (m_dwPrimarySize > 0)
	{
		size_t popSize = (cnt > m_dwPrimarySize) ? m_dwPrimarySize : cnt;

		// 버퍼 복사
		memcpy(_outBuffer, m_cpPrimaryPointer, popSize);

		// 내용 삭제
		memset(m_cpPrimaryPointer, 0, popSize);

		m_dwPrimarySize -= popSize;
		m_cpPrimaryPointer += popSize;
		cnt -= popSize;
	}

	// Primary Buffer 에서 삭제 후 Second Buffer 에서 마저 삭제해야 하는 경우
	if (cnt > 0 && m_dwSecondSize > 0)
	{
		size_t popSize = (cnt > m_dwSecondSize) ? m_dwSecondSize : cnt;

		// 버퍼 복사
		memcpy(_outBuffer + (_len - cnt), m_cpSecondPointer, popSize);

		// 내용 삭제
		memset(m_cpSecondPointer, 0, popSize);

		m_dwSecondSize -= popSize;
		m_cpSecondPointer += popSize;
		cnt -= popSize;
	}

	// 삭제 후 Primary Buffer 는 비어있고, Second Buffer 에 데이터가 남아있을 경우
	// Second Buffer Data -> Primary Buffer
	if (m_dwPrimarySize == 0)
	{
		if (m_dwSecondSize > 0)
		{
			if (m_cpSecondPointer != m_cBuffer)
				memmove(m_cBuffer, m_cpSecondPointer, m_dwSecondSize);

			m_cpPrimaryPointer = m_cBuffer;
			m_dwPrimarySize = m_dwSecondSize;

			m_cpSecondPointer = nullptr;
			m_dwSecondSize = 0;
		}
		else
		{
			m_cpSecondPointer = nullptr;
			m_dwSecondSize = 0;

			m_cpPrimaryPointer = m_cBuffer;
			m_dwPrimarySize = 0;
		}
	}

	return true;
}