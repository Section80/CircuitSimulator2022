#include "stdafx.h"
#include "SwitchCircuit.h"

SwitchCircuit::SwitchCircuit()
	: Circuit("Switch", ECircuitType::Switch, 0, 1, &m_outBuf1, &m_outBuf2, 1, 0.0f)
	, m_outputPin(*this, "out", 0, 1)
	, m_val(false)
	, m_outBuf1(false)
	, m_outBuf2(false)
	, m_buttonId(Identifiable::GetNewId())
{}

SwitchCircuit::SwitchCircuit(float x, float y)
	: SwitchCircuit()
{
	SetPos(x, y);
}

void SwitchCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());
	
	ImGui::PushID(m_buttonId);
	if (m_val == true)
	{
		if (ImGui::Button("1"))
		{
			m_val = false;
			swapCircuitOutput();
			setOutputData(0, &m_val);
			afterUpdateOutput();
		}
	}
	else {
		if (ImGui::Button("0"))
		{
			m_val = true;
			swapCircuitOutput();
			setOutputData(0, &m_val);
			afterUpdateOutput();
		}
	}
	ImGui::PopID();

	m_outputPin.Render();
	ImNode::EndNode();
}

InputPin* SwitchCircuit::GetInputPin(int index)
{
	assert(false);

	return nullptr;
}

OutputPin* SwitchCircuit::GetOutputPin(int index)
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

void SwitchCircuit::updateOutput()
{}
