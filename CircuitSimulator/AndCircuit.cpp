#include "stdafx.h"
#include "AndCircuit.h"

AndCircuit::AndCircuit()
	: Circuit("And", ECircuitType::And, 2, 1, &m_outputBuffer1, &m_outputBuffer2, 1, 0.1f)
	, m_inputPin0(*this, "in1", 1)
	, m_inputPin1(*this, "in2", 1)
	, m_outputPin(*this, "out", 0, 1)
{}

AndCircuit::AndCircuit(float x, float y)
	: AndCircuit()
{
	SetPos(x, y);
}

void AndCircuit::render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
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
		setOutputData(0, &b);
		return;
	}

	b = false;
	setOutputData(0, &b);
}


NandCircuit::NandCircuit()
	: Circuit("Nand", ECircuitType::Nand, 2, 1, &m_outputBuffer1, &m_outputBuffer2, 1, 1.0f)
	, m_inputPin0(*this, "in1", 1)
	, m_inputPin1(*this, "in2", 1)
	, m_outputPin(*this, "out", 0, 1)
{}

NandCircuit::NandCircuit(float x, float y)
	: NandCircuit()
{
	SetPos(x, y);
}

void NandCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());
	m_inputPin0.Render();
	ImGui::SameLine();
	m_outputPin.Render();
	m_inputPin1.Render();
	ImNode::EndNode();
}

InputPin* NandCircuit::GetInputPin(int index)
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

OutputPin* NandCircuit::GetOutputPin(int index)
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

void NandCircuit::updateOutput()
{
	bool b = false;
	if (m_inputPin0.ReadAt(0) && m_inputPin1.ReadAt(0))
	{
		setOutputData(0, &b);
		return;
	}

	b = true;
	setOutputData(0, &b);
}