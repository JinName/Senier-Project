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
/// �ι�° Ŀ���� ������ ������������ �̵���Ų��.
/// </summary>
void CircularBuffer::allocateSecondPointer()
{
	m_pSecondCursor = m_Buffer;
}

/// <summary>
/// ù��° Ŀ������ ���� ������ ���������� ������ ������ ���� ���� ������ ��ȯ�Ѵ�.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::getAvailablePrimaryBufferSize()
{
	return (m_pBufferEnd - (m_pPrimaryCursor + m_PrimarySize));
}

/// <summary>
/// �ι�° Ŀ������ ���� ������ ���������� ù��° Ŀ������ ���� ������ ��ȯ�Ѵ�.
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
/// ���� ���� ������ ���۳��� ������ ����Ͽ� ��ȯ.
/// ������ ����ϸ� Ŀ���� ��ġ�� ���� ���� ������ �����Ѵ�.
/// GetWritablePointer() ������ ����Ǿ�� �ϴ� �Լ��̴�.
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
		// Primary Buffer �� ���������� MAX_PACKET_SIZE ���� ���� ���
		// Second Pointer �� ��ü ������ ù ������ �Ҵ��Ͽ�
		// Second Pointer ���� �����͸� �����ϵ��� ��
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
/// ���� ���� ������ ������ ��ȯ.
/// GetAvailableBufferSize() �Լ��� ����Ǿ�� �Ѵ�.
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
/// ���� ���۳��� �����Ͱ� �����ϰ� �ִ� ������ ��ȯ�Ѵ�.
/// </summary>
/// <returns>	size_t	</returns>
size_t CircularBuffer::GetWritedDataSize()
{
	return m_PrimarySize + m_SecondSize;
}

/// <summary>
/// Recv() �� ���۰� ������ ���� ���Ե� ������ ���̸�ŭ�� ������Ʈ�Ѵ�.
/// </summary>
/// <param name="len">	���� ���Ե� �������� ����	</param>
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
/// Peek() : ���� ���� �����Ϳ��� Head ���� Ȯ���Ͽ� Pop �ؾ��ϴ� �������� ���̸� Ȯ���Ѵ�.
/// </summary>
/// <param name="outBuffer">	Peek ��� ��	</param>
/// <param name="len">			Peek �� ����	</param>
/// <returns>					bool type		</returns>
bool CircularBuffer::Peek(char* outBuffer, size_t len)
{
	if (GetWritedDataSize() < len)
	{
		// ������ �߻��� Ŭ����, �Լ�, ���� ���� ���
		printError(this, __func__, "writed buffer size less than require");
		return false;
	}

	size_t cnt = len;

	// Primary Buffer ���� Peek �ϴ� ���
	if (m_PrimarySize > 0)
	{
		size_t peekSize = (cnt > m_PrimarySize) ? m_PrimarySize : cnt;

		// ���� ����
		memcpy(outBuffer, m_pPrimaryCursor, peekSize);

		cnt -= peekSize;
	}

	// Primary Buffer ���� Peek ���� Second Buffer ���� ���� Peek �ؾ� �ϴ� ���
	if (cnt > 0 && m_SecondSize > 0)
	{
		size_t peekSize = (cnt > m_SecondSize) ? m_SecondSize : cnt;

		// ���� ����
		memcpy(outBuffer + (len - cnt), m_pSecondCursor, peekSize);

		cnt -= peekSize;
	}

	return true;
}

/// <summary>
/// Pop() : ���ۿ��� ���ϴ� ���̸�ŭ �����͸� �����ϰ� �����Ѵ�.
/// </summary>
/// <param name="outBuffer">	Pop ��� ��		</param>
/// <param name="len">			Pop �� ����		</param>
/// <returns>					bool type		</returns>
bool CircularBuffer::Pop(char* outBuffer, size_t len)
{
	// Pop �Ϸ��� ������ ���̰� ���ۿ� ����ִ� ������ ���̺��� Ŭ ���,
	// return false;
	if (GetWritedDataSize() < len)
	{
		// ������ �߻��� Ŭ����, �Լ�, ���� ���� ���
		printError(this, __func__, "writed buffer size less than require");
		return false;
	}		

	size_t cnt = len;

	// Primary Buffer ���� Pop �ϴ� ���
	if (m_PrimarySize > 0)
	{
		size_t popSize = (cnt > m_PrimarySize) ? m_PrimarySize : cnt;

		// ���� ����
		memcpy(outBuffer, m_pPrimaryCursor, popSize);

		// ���� ����
		memset(m_pPrimaryCursor, 0, popSize);

		m_PrimarySize -= popSize;
		m_pPrimaryCursor += popSize;
		cnt -= popSize;
	}

	// Primary Buffer ���� Pop ���� Second Buffer ���� ���� Pop �ؾ� �ϴ� ���
	if (cnt > 0 && m_SecondSize > 0)
	{
		size_t popSize = (cnt > m_SecondSize) ? m_SecondSize : cnt;

		// ���� ����
		memcpy(outBuffer + (len - cnt), m_pSecondCursor, popSize);

		// ���� ����
		memset(m_pSecondCursor, 0, popSize);

		m_SecondSize -= popSize;
		m_pSecondCursor += popSize;
		cnt -= popSize;
	}

	// Pop ���� Primary Buffer �� ����ְ�	
	if (m_PrimarySize == 0)
	{
		// Second Buffer �� �����Ͱ� �������� ���
		if (m_SecondSize > 0)
		{
			// Second Buffer Data �� ������������ ����
			if (m_pSecondCursor != m_Buffer)
				memcpy(m_Buffer, m_pSecondCursor, m_SecondSize);

			// Primary Buffer Cursor �� ������������ ����
			// + Primary Buffer ������ ���ԵǾ��ִ� ������ ����
			m_pPrimaryCursor = m_Buffer;
			m_PrimarySize = m_SecondSize;
		}
		// Second Buffer �� ����ִ� ���
		else
		{
			// Primary Buffer Cursor �� ������������ ����
			m_pPrimaryCursor = m_Buffer;
			m_PrimarySize = 0;
		}

		// Cursor �̵��� ������ Second Cursor �ʱ�ȭ
		m_pSecondCursor = nullptr;
		m_SecondSize = 0;
	}

	return true;
}