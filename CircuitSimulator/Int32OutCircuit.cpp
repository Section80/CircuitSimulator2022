#include "stdafx.h"
#include <ctype.h>
#include "Convert.h"
#include "Int32OutCircuit.h"


Int32OutCircuit::Int32OutCircuit()
	:Circuit("Int32", 0, 1, m_outBuf1, m_outBuf2, 32, 0.0f)
	, m_outputPin(*this, "out", 0, 32)
	, m_textInputId(Identifiable::GetNewId())
	, m_val(0)
{}

Int32OutCircuit::Int32OutCircuit(float x, float y)
	: Int32OutCircuit()
{
	SetPos(x, y);
}

InputPin* Int32OutCircuit::GetInputPin(int index)
{
	assert(false);

	return nullptr;
}

OutputPin* Int32OutCircuit::GetOutputPin(int index)
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

void Int32OutCircuit::render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());

		ImGui::PushID(m_textInputId);
		
		ImGui::PushItemWidth(100.0f);
		bool bInput = ImGui::InputInt(
			"", &m_val, 1, 100
			, ImGuiInputTextFlags_EnterReturnsTrue
		);
		ImGui::PopItemWidth();

		if (bInput)
		{
			swapCircuitOutput();
			bool* outputBuffer = getOutputDataBuffer(0);
			Uint32ToBoolArray(m_val, outputBuffer);
			afterUpdateOutput();
		}
		ImGui::PopID();
		ImGui::SameLine();
		m_outputPin.Render();
	ImNode::EndNode();
}

void Int32OutCircuit::updateOutput()
{}
