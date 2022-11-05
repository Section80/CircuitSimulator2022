#include <assert.h>
#include <string.h>
#include "CircuitOutput.h"

CircuitOutput::CircuitOutput(bool* pBuffer1, bool* pBuffer2, int size)
	: m_pOldBuffer(pBuffer1)
	, m_pCurrentBuffer(pBuffer2)
	, m_size(size)
{
	if (size != 0)
	{
		assert(m_pOldBuffer != nullptr);
		assert(m_pCurrentBuffer != nullptr);
	}
}

void CircuitOutput::Swap()
{
	bool* temp = m_pOldBuffer;
	m_pOldBuffer = m_pCurrentBuffer;
	m_pCurrentBuffer = temp;
}

bool CircuitOutput::ReadAt(int index)
{
	assert(index < m_size);
	return m_pCurrentBuffer[index];
}

void CircuitOutput::Write(const bool* pData, int offset, int size)
{
	assert(offset + size <= m_size);
	memcpy(&m_pCurrentBuffer[offset], pData, size);
}

bool CircuitOutput::IsChanged(int offset, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (m_pCurrentBuffer[offset + i] != m_pOldBuffer[offset + i])
		{
			return true;
		}
	}

	return false;
}