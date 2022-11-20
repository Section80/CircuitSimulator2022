#include "stdafx.h"
#include "BitBulbCircuit.h"

BitBulbCircuit::BitBulbCircuit()
	: Circuit("BitBulb", ECircuitType::BitBulb, 1, 0, nullptr, nullptr, 0, 0.0f)
	, m_inputPin(*this, "in1", 1)
{
}

BitBulbCircuit::BitBulbCircuit(float x, float y)
	: BitBulbCircuit()
{
	SetPos(x, y);
}

void BitBulbCircuit::Render()
{
	ImNode::BeginNode(GetNodeId());
		ImGui::Text("Bulb");
		if (m_inputPin.GetFrom() != nullptr)
		{
			if (m_inputPin.ReadAt(0))
			{
				ImGui::Text("1");
			}
			else
			{
				ImGui::Text("0");
			}
		}
		else
		{
			ImGui::Text("None");
		}
		m_inputPin.Render();
	ImNode::EndNode();
}

InputPin* BitBulbCircuit::GetInputPin(int index)
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

OutputPin* BitBulbCircuit::GetOutputPin(int index)
{
	assert(false);

	return nullptr;
}

void BitBulbCircuit::updateOutput()
{
}
