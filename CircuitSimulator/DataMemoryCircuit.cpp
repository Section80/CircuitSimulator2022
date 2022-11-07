#include "stdafx.h"
#include "Convert.h"
#include "DataMemoryCircuit.h"

DataMemoryCircuit::DataMemoryCircuit()
	: Circuit("Data Memory", 5, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_addr(*this, "addr", 32)
	, m_wData(*this, "wData", 32)
	, m_clock(*this, "clock", 1)
	, m_memRead(*this, "memRead",1 )
	, m_memWrite(*this, "memWrite", 1)
	, m_rData(*this, "rData", 0, 32)
	, m_bLastClock(false)
{}

DataMemoryCircuit::DataMemoryCircuit(float x, float y)
	: DataMemoryCircuit()
{
	SetPos(x, y);
}

void DataMemoryCircuit::render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
		renderDelay(160.0f);

		m_addr.Render();
		ImGui::SameLine();
		m_rData.Render();

		m_wData.Render();
		m_memRead.Render();
		m_memWrite.Render();
		m_clock.Render();
	ImNode::EndNode();
}

InputPin* DataMemoryCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_addr;
	case 1:
		return &m_wData;
	case 2:
		return &m_memRead;
	case 3:
		return &m_memWrite;
	case 4:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* DataMemoryCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_rData;
	default:
		assert(false);
	}

	return nullptr;
}

void DataMemoryCircuit::updateOutput()
{
	bool bClock = m_clock.ReadAt(0);
	bool bRisingEdge = false;
	bool bFallingEdge = false;
	if (m_bLastClock != bClock)
	{
		if (bClock)
		{
			// rising edge
			bRisingEdge = true;
		}
		else
		{
			// falling edge
			bFallingEdge = true;
		}
	}
	m_bLastClock = bClock;

	uint32_t addr = ReadToUint32(m_addr, 32);
	uint32_t bits = m_data[addr];
	bool* data_buffer = getOutputDataBuffer(0);
	Uint32ToBoolArray(bits, data_buffer);

	if (bRisingEdge)
	{
		if (m_memWrite.ReadAt(0))
		{
			// update edge triggred part here
			uint32_t addr = ReadToUint32(m_addr, 32);
			int val = ReadToUint32(m_wData, 32);
			m_data[addr] = val;

			// 입력이 변하지 않더라도 출력을 업데이트하도록
			// 남은 딜레이를 리셋시킨다. 
			resetDelay();
		}
	}
}
