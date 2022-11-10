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
	addr = addr / 4;	// 32bit ����: 4����Ʈ�� ������. 
	
	m_val = 0;
	if (m_map.count(addr))
	{
		m_val = m_map[addr];
	}
	

	// #1
	// �� 0���� �ƴ϶� 3���� �����ñ�? 
	// 0���� offset�� 21�̴�. 
	// 21���� 32��Ʈ�� ���� �ڿ� �ִ� m_map�� ħ���Ѵ�. 
	// �׷��� ������ detor���� ������ �߻��ߴ�...
	// ������ ������ ��. 

	// #2
	// output�� m_op�� �߰��ϸ鼭 outputPin�� �ε����� 1�� �з�����. 
	// �׷��� ���̻� 3�� output�� offset�� 0�� �ƴϴ�. 
	// ������ 3���� ������ 4�� ���ų�,
	// �������� �߰��� 5�� �ᵵ �ȴ�. 5�� offset�� 0�̴�. 
	bool* buf = getOutputDataBuffer(5);
	Uint32ToBoolArray(m_val, buf);
}
