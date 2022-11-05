#include "stdafx.h"
#include <ctype.h>
#include "Conversion.h"
#include "Int32OutCircuit.h"


Int32OutCircuit::Int32OutCircuit()
	:Circuit("Int32", 0, 1, m_outBuf1, m_outBuf2, 32, 0.0f)
	, m_outputPin(*this, "out", 0, 32)
	, m_textInputId(Identifiable::GetNewId())
	, m_val(0)
{}

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

		ImGui::PushItemWidth(100.0f);
		bool bInput = ImGui::InputInt(
			"", &m_val, 1, 100
			, ImGuiInputTextFlags_EnterReturnsTrue
		);
		if (bInput)
		{
			swapCircuitOutput();
			bool* outputBuffer = getOutputDataBuffer(0);
			Uint32ToBoolArray(m_val, outputBuffer);
			afterUpdateOutput();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		m_outputPin.Render();
	ImNode::EndNode();
}

void Int32OutCircuit::updateOutput()
{}


int Int32OutCircuit::inputTextCallback(ImGuiInputTextCallbackData* pData)
{
	if (isdigit(pData->Buf[0]))
	{
		/*
		Int32OutCircuit* c = (Int32OutCircuit*)pData->UserData;
		c->swapCircuitOutput();
		int val = atoi(c->m_inputBuffer);
		printf("Int32Out Val: %d\n", val);
		bool* outputBuffer = c->getOutputDataBuffer(0);
		Uint32ToBoolArray(val, outputBuffer);
		c->afterUpdateOutput();
		*/
	}

	return 0;
}