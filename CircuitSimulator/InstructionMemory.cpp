#include "stdafx.h"
#include "Convert.h"
#include "Instruction.h"
#include "InstructionMemory.h"

InstructionMemoryCircuit::InstructionMemoryCircuit()
	: Circuit("Instruction Memory", 1, 6, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_addr(*this, "addr", 32)
	, m_op(*this, "op", 26, 5)
	, m_rs(*this, "rs", 21, 5)
	, m_rt(*this, "rt", 16, 5)
	, m_rd(*this, "rd", 11, 5)
	, m_funct(*this, "funt", 0, 5)
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

InstructionMemoryCircuit::~InstructionMemoryCircuit()
{
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

		ImGui::BeginVertical("Input");
		m_addr.Render();
		ImGui::EndVertical();

		ImGui::BeginVertical("output");
		m_op.Render();
		m_rs.Render();
		m_rt.Render();
		m_rd.Render();
		m_funct.Render();
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
		return &m_op;
	case 1:
		return &m_rs;
	case 2:
		return &m_rt;
	case 3:
		return &m_rd;
	case 4:
		return &m_funct;
	case 5:
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
	

	// #1
	// 왜 0번이 아니라 3번을 가져올까? 
	// 0번은 offset이 21이다. 
	// 21부터 32비트를 쓰면 뒤에 있는 m_map을 침범한다. 
	// 그래서 실제로 detor에서 에러가 발생했다...
	// 오버런 조심할 것. 

	// #2
	// output에 m_op를 추가하면서 outputPin의 인덱스가 1씩 밀려났다. 
	// 그래서 더이상 3번 output은 offset이 0이 아니다. 
	// 기존의 3번을 쓰려면 4를 쓰거나,
	// 마지막에 추가된 5를 써도 된다. 5도 offset이 0이다. 
	bool* buf = getOutputDataBuffer(5);
	Uint32ToBoolArray(m_val, buf);
}
