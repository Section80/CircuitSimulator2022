#include "stdafx.h"
#include "Convert.h"
#include "ProgramCounterCircuit.h"

ProgramCounterCircuit::ProgramCounterCircuit()
	: Circuit("Program Counter", ECircuitType::ProgramCounter, 3, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_inAddr(*this, "inAddr", 32)
	, m_write(*this, "write", 1)
	, m_clock(*this, "clock", 1)
	, m_outAddr(*this, "addr", 0, 32)
	, m_bLastClock(false)
	, m_addr(0x00400024)
{}

ProgramCounterCircuit::ProgramCounterCircuit(float x, float y)
	: ProgramCounterCircuit()
{
	SetPos(x, y);
}

void ProgramCounterCircuit::Render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
		renderDelay(160.0f);

		char buffer[16];
		sprintf_s(buffer, "%0#10x", m_addr);
		ImGui::Text(buffer);

		m_inAddr.Render();
		ImGui::SameLine();
		m_outAddr.Render();

		m_write.Render();
		m_clock.Render();
	ImNode::EndNode();
}

InputPin* ProgramCounterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_inAddr;
	case 1:
		return &m_write;
	case 2:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* ProgramCounterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_outAddr;
	default:
		assert(false);
	}

	return nullptr;
}

void ProgramCounterCircuit::updateOutput()
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

	bool* buf = getOutputDataBuffer(0);
	Uint32ToBoolArray(m_addr, buf);

	if (bRisingEdge)
	{
		if (m_write.ReadAt(0))
		{
			uint32_t newAddr = ReadToUint32(m_inAddr, 32);
			if (m_addr != newAddr)
			{
				m_addr = newAddr;
				resetDelay();
			}
		}
	}
}
