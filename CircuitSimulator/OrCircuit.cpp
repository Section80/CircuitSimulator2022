#include "stdafx.h"
#include "OrCircuit.h"

OrCircuit::OrCircuit()
	: Circuit("Or", ECircuitType::Or, 2, 1, &m_outputBuffer1, &m_outputBuffer2, 1, 1.0f)
	, m_inputPin0(*this, "in1", 1)
	, m_inputPin1(*this, "in2", 1)
	, m_outputPin(*this, "out", 0, 1)
{}

OrCircuit::OrCircuit(float x, float y)
	: OrCircuit()
{
	SetPos(x, y);
}

void OrCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text("Or");
	m_inputPin0.Render();
	ImGui::SameLine();
	m_outputPin.Render();
	m_inputPin1.Render();
	ImNode::EndNode();
}

InputPin* OrCircuit::GetInputPin(int index)
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

OutputPin* OrCircuit::GetOutputPin(int index)
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

void OrCircuit::updateOutput()
{
	bool b = true;
	if (m_inputPin0.ReadAt(0) || m_inputPin1.ReadAt(0))
	{
		setOutputData(0, &b);
		return;
	}

	b = false;
	setOutputData(0, &b);
}


NorCircuit::NorCircuit()
	: Circuit("Nor", ECircuitType::Not, 2, 1, &m_outputBuffer1, &m_outputBuffer2, 1, 1.0f)
	, m_inputPin0(*this, "in1", 1)
	, m_inputPin1(*this, "in2", 1)
	, m_outputPin(*this, "out", 0, 1)
{}

NorCircuit::NorCircuit(float x, float y)
	: NorCircuit()
{
	SetPos(x, y);
}

void NorCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text("Nor");
	m_inputPin0.Render();
	ImGui::SameLine();
	m_outputPin.Render();
	m_inputPin1.Render();
	ImNode::EndNode();
}

InputPin* NorCircuit::GetInputPin(int index)
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

OutputPin* NorCircuit::GetOutputPin(int index)
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

void NorCircuit::updateOutput()
{
	bool b = false;
	if (m_inputPin0.ReadAt(0) || m_inputPin1.ReadAt(0))
	{
		setOutputData(0, &b);
		return;
	}

	b = true;
	setOutputData(0, &b);
}