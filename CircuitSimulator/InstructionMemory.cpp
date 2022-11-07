#include "stdafx.h"
#include "Convert.h"
#include "Instruction.h"
#include "InstructionMemory.h"

InstructionMemoryCircuit::InstructionMemoryCircuit()
	: Circuit("Instruction Memory", 1, 4, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_addr(*this, "addr", 32)
	, m_rs(*this, "rs", 21, 5)
	, m_rt(*this, "rt", 16, 5)
	, m_rd(*this, "rd", 11, 5)
	, m_low16(*this, "low16", 0, 16)
	, m_val(0)
{
	for (int i = 0; i < 100; i++)
	{
		m_map[i] = i;
	}
}

InstructionMemoryCircuit::InstructionMemoryCircuit(float x, float y)
	: InstructionMemoryCircuit()
{
	SetPos(x, y);
}

void InstructionMemoryCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());
	renderDelay(160.0f);

	char buffer[16];
	_itoa_s(m_val, buffer, 10);
	ImGui::Text(buffer);

	ImGui::BeginHorizontal("IO");

		ImGui::BeginVertical("Inputs");
		m_addr.Render();
		ImGui::EndVertical();

		ImGui::BeginVertical("outputs");
		m_rs.Render();
		m_rt.Render();
		m_rd.Render();
		m_low16.Render();
		ImGui::EndVertical();

	ImGui::EndHorizontal();

	ImNode::EndNode();
}

InputPin* InstructionMemoryCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_addr;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* InstructionMemoryCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_rs;
	case 1:
		return &m_rt;
	case 2:
		return &m_rd;
	case 3:
		return &m_low16;
	default:
		assert(false);
	}

	return nullptr;
}

void InstructionMemoryCircuit::updateOutput()
{
	int addr = ReadToUint32(m_addr, 32);
	addr = addr / 4;	// 32bit 단위: 4바이트로 나눈다. 
	
	m_val = 0;
	if (m_map.count(addr))
	{
		m_val = m_map[addr];
	}
	

	bool* buf = getOutputDataBuffer(0);
	Uint32ToBoolArray(m_val, buf);
}
