#include "stdafx.h"
#include "Convert.h"
#include "ClockCircuit.h"

ClockCircuit::ClockCircuit()
	: Circuit("Clock", ECircuitType::Clock, 0, 1, &m_outBuf1, &m_outBuf2, 1, 3.0f)
	, m_out(*this, "Clock", 0, 1)
	, m_bVal(false)
{
	resetDelay();
}

ClockCircuit::ClockCircuit(float x, float y)
	: ClockCircuit()
{
	SetPos(x, y);
}

void ClockCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(70.0f);

	if (m_bVal == false)
	{
		ImGui::Text("0");
	}
	else 
	{
		ImGui::Text("1");
	}

	m_out.Render();

	ImNode::EndNode();
}

void ClockCircuit::RenderWire(bool bSummary)
{
	if (!bSummary)
	{
		Circuit::RenderWire();
	}
}

InputPin* ClockCircuit::GetInputPin(int index)
{
	assert(false);
	return nullptr;
}

OutputPin* ClockCircuit::GetOutputPin(int index)
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

void ClockCircuit::afterUpdateOutput()
{
	Circuit::afterUpdateOutput();
}

void ClockCircuit::updateOutput()
{
	if (m_bVal)
	{
		m_bVal = false;
	}
	else 
	{
		m_bVal = true;
	}

	setOutputData(0, &m_bVal);

	resetDelay();	// 무한 반복
}


