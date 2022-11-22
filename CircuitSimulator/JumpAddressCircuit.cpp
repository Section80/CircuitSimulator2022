#include "JumpAddressCircuit.h"

JumpAddressCircuit::JumpAddressCircuit()
	: Circuit(
		"Jump Address", ECircuitType::JumpAddress,
		2, 1, outBuf1, outBuf2, 32, 0.1f)
	, m_pc(*this, "pc", 32)
	, m_address(*this, "address", 26)
	, m_jump(*this, "jump", 0, 32)
{}

JumpAddressCircuit::JumpAddressCircuit(float x, float y)
	: JumpAddressCircuit()
{
	SetPos(x, y);
}

void JumpAddressCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_pc.Render();
	ImGui::SameLine();
	m_jump.Render();
	m_address.Render();

	ImNode::EndNode();
}

InputPin* JumpAddressCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_pc;
	case 1: 
		return &m_address;
	default:
		assert(false);
		break;
	}

	return nullptr;
}

OutputPin* JumpAddressCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_jump;
	default:
		assert(false);
		break;
	}

	return nullptr;
}

void JumpAddressCircuit::updateOutput()
{
	int mask = 0b11110000000000000000000000000000;
	uint32_t pc = m_pc.Value();
	uint32_t addr = m_address.Value();

	addr = addr << 2;
	uint32_t val = (pc & mask) | addr;

	setOutputDataByValue(m_jump, val);
}
