#include "stdafx.h"
#include "Convert.h"
#include "FunctCircuit.h"


FunctCircuit::FunctCircuit()
	: Circuit("Funct", ECircuitType::Funct, 1, 1, m_outBuf1, m_outBuf2, 6, 0.1f)
	, m_signEx(*this, "signEx", 32)
	, m_funct(*this, "funct", 0 , 6)
{
}

FunctCircuit::FunctCircuit(float x, float y)
	: FunctCircuit()
{
	SetPos(x, y);
}

void FunctCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_signEx.Render();
	ImGui::SameLine();
	m_funct.Render();

	ImNode::EndNode();
}

InputPin* FunctCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_signEx;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* FunctCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_funct;
	default:
		assert(false);

	}
	return nullptr;
}

void FunctCircuit::updateOutput()
{
	bool* outBuf = getOutputDataBuffer(0);
	uint32_t val = ReadToUint32(m_signEx, 6);
	Uint32ToBoolArray(
		val, 
		outBuf, 
		GetOutputPin(0)->GetWireLineCount()
	);
}
