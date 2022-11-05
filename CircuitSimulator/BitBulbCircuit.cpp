#include "BitBulbCircuit.h"

BitBulbCircuit::BitBulbCircuit()
	: Circuit("BitBulb", 1, 0, nullptr, nullptr, 0, 0.0f)
	, m_inputPin(*this, "in1", 1)
{
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
