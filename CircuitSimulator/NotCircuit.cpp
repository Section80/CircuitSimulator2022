#include "stdafx.h"
#include "NotCircuit.h"

NotCircuit::NotCircuit()
	: Circuit("     Not", ECircuitType::Not, 1, 1, &m_outBuf1, &m_outBuf1, 1, 0.0f)
	, m_inputPin(*this, "i", 1)
	, m_outputPin(*this, "o", 0, 1)
	, m_outBuf1(false)
	, m_outBuf2(false)
{}

NotCircuit::NotCircuit(float x, float y)
	: NotCircuit()
{
	SetPos(x, y);
}

void NotCircuit::Render()
{
	ImNode::BeginNode(GetNodeId());
		ImGui::Text(GetName());
		
		m_inputPin.Render();
		ImGui::SameLine();
		m_outputPin.Render();
	ImNode::EndNode();
}

InputPin* NotCircuit::GetInputPin(int index)
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

OutputPin* NotCircuit::GetOutputPin(int index)
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

void NotCircuit::updateOutput()
{
	bool b = !m_inputPin.ReadAt(0);
	setOutputData(0, &b);
}

