#include "AndCircuit.h"

AndCircuit::AndCircuit(float delay)
	: Circuit("And", 2, 1, &m_outputBuffer1, &m_outputBuffer2, 1, 1.0f)
	, m_inputPin0(*this, "in1", 1)
	, m_inputPin1(*this, "in2", 1)
	, m_outputPin(*this, "out", 0, 1)
{}

void AndCircuit::Render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text("And");
		m_inputPin0.Render();
		ImGui::SameLine();
		m_outputPin.Render();
		m_inputPin1.Render();
	ImNode::EndNode();
}

InputPin* AndCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_inputPin0;
	case 1:
		return &m_inputPin1;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* AndCircuit::GetOutputPin(int index)
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

void AndCircuit::updateOutput()
{
	bool b = true;
	if (m_inputPin0.ReadAt(0) && m_inputPin1.ReadAt(0))
	{
		printf("And True\n");
		setOutputData(0, &b);
		return;
	}

	b = false;
	setOutputData(0, &b);
	printf("And False\n");
}