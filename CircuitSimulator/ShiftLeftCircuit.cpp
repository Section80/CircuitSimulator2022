#include "stdafx.h"
#include "ShiftLeftCircuit.h"

ShiftLeftCircuit::ShiftLeftCircuit()
	: Circuit("Shift Left 2", ECircuitType::ShiftLeft, 1, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_in(*this, "in", 32)
	, m_out(*this, "out", 0, 32)
{}

ShiftLeftCircuit::ShiftLeftCircuit(float x, float y)
	: ShiftLeftCircuit()
{
	SetPos(x, y);
}

void ShiftLeftCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in.Render();
	ImGui::SameLine();
	m_out.Render();

	ImNode::EndNode();
}

InputPin* ShiftLeftCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* ShiftLeftCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_out;
	default:
		assert(false);
	}

	return nullptr;
}

void ShiftLeftCircuit::updateOutput()
{
	bool* outBuf = getOutputDataBuffer(0);

	for (int i = 0; i < 30; i++)
	{
		bool bVal = m_in.ReadAt(i);
		outBuf[i + 2] = bVal;
	}

	outBuf[0] = false;
	outBuf[1] = false;
}
