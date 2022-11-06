#include "stdafx.h"
#include "Convert.h"
#include "DisplayInt32Circuit.h"

DisplayInt32Circuit::DisplayInt32Circuit()
	: Circuit("Display Int32", 1, 0, nullptr, nullptr, 0, 0.0f)
	, m_inputPin(*this, "in", 32)
{
	memset(m_buffer, 0, sizeof(char) * 32);
}

DisplayInt32Circuit::DisplayInt32Circuit(float x, float y)
	: DisplayInt32Circuit()
{
	SetPos(x, y);
}

InputPin* DisplayInt32Circuit::GetInputPin(int index)
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

OutputPin* DisplayInt32Circuit::GetOutputPin(int index)
{
	assert(false);

	return nullptr;
}

void DisplayInt32Circuit::render()
{
	ImNode::BeginNode(GetNodeId());
		ImGui::Text(GetName());
		if (m_inputPin.GetFrom() != nullptr)
		{
			uint32_t val = ReadToUint32(m_inputPin, 32);
			_itoa_s(val, m_buffer, 10);
			ImGui::Text(m_buffer);
		}
		else 
		{
			ImGui::Text("None");
		}
		m_inputPin.Render();
	ImNode::EndNode();
}
