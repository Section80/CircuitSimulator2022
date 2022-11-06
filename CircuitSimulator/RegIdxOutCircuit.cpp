#include "stdafx.h"
#include "RegIdxOutCircuit.h"
#include "Convert.h"

RegIdxOutCircuit::RegIdxOutCircuit()
	: Circuit("Register Index", 0, 1, m_outBuf1, m_outBuf2, 5, 0.0f)
	, m_outputPin(*this, "out", 0, 5)
	, m_textInputId(Identifiable::GetNewId())
	, m_val(0)
{}

RegIdxOutCircuit::RegIdxOutCircuit(float x, float y)
	: RegIdxOutCircuit()
{
	SetPos(x, y);
}

InputPin* RegIdxOutCircuit::GetInputPin(int index)
{
	assert(false);

	return nullptr;
}

OutputPin* RegIdxOutCircuit::GetOutputPin(int index)
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

void RegIdxOutCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	ImGui::PushID(m_textInputId);
	ImGui::PushItemWidth(100.0f);
	bool bInput = ImGui::InputInt(
		"", (int*)&m_val, 1, 1
		, ImGuiInputTextFlags_EnterReturnsTrue
	);
	if (bInput)
	{
		if (m_val < 0) m_val = 0;
		if (m_val > 31) m_val = 31;

		swapCircuitOutput();
		bool* outputBuffer = getOutputDataBuffer(0);
		Uint32ToBoolArray(m_val, outputBuffer, 5);
		afterUpdateOutput();
	}
	ImGui::PopItemWidth();
	ImGui::PopID();
	ImGui::SameLine();
	m_outputPin.Render();
	ImNode::EndNode();
}
