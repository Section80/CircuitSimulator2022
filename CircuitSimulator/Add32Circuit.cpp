#include "stdafx.h"
#include "Convert.h"
#include "Add32Circuit.h"

Add32Circuit::Add32Circuit()
	: Circuit("Add", 2, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_in1(*this, "in1", 32)
	, m_in2(*this, "in2", 32)
	, m_out(*this, "out", 0, 32)
{}

Add32Circuit::Add32Circuit(float x, float y)
	: Add32Circuit()
{
	SetPos(x, y);
}

void Add32Circuit::render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
		renderDelay(140.0f);
		m_in1.Render();
		ImGui::SameLine();
		m_out.Render();
		m_in2.Render();
	ImNode::EndNode();
}

InputPin* Add32Circuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in1;
	case 1:
		return &m_in2;
	default:
		assert(false);
	}
	return nullptr;
}

OutputPin* Add32Circuit::GetOutputPin(int index)
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

void Add32Circuit::updateOutput()
{
	uint32_t in1 = ReadToUint32(m_in1, 32);
	uint32_t in2 = ReadToUint32(m_in2, 32);

	bool* buf = getOutputDataBuffer(0);
	Uint32ToBoolArray(in1 + in2, buf);
}
