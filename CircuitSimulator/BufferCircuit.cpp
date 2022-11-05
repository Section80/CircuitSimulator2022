#include "BufferCircuit.h"

BufferCircuit::BufferCircuit()
	: Circuit("Buffer", 1, 1, &m_outBuf1, &m_outBuf1, 1, 0.0f)
	, m_inputPin(*this, "in", 1)
	, m_outputPin(*this, "out", 0, 1)
	, m_outBuf1(false)
	, m_outBuf2(false)
{}

void BufferCircuit::Render()
{
	ImNode::BeginNode(GetNodeId());
		ImGui::Text(GetName());
		m_inputPin.Render();
		ImGui::SameLine();
		m_outputPin.Render();
	ImNode::EndNode();
}

InputPin* BufferCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_inputPin;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* BufferCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_outputPin;
	default:
		assert(false);
	}

	return nullptr;
}

void BufferCircuit::updateOutput()
{
	bool b = m_inputPin.ReadAt(0);
	setOutputData(0, &b);
}
