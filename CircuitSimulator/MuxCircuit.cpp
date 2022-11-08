#include "stdafx.h"
#include "MuxCircuit.h"

Mux21Circuit::Mux21Circuit()
	: Circuit("2 to 1 Mux", 3, 1, m_outBuf1, m_outBuf2, 32, 0.1f)
	, m_in0(*this, "in0", 32)
	, m_in1(*this, "in1", 32)
	, m_select(*this, "select", 1)
	, m_out(*this, "out", 0, 32)
{}

Mux21Circuit::Mux21Circuit(float x, float y)
	: Mux21Circuit()
{
	SetPos(x, y);
}

void Mux21Circuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in0.Render();
	ImGui::SameLine();
	m_out.Render();
	m_in1.Render();
	m_select.Render();

	ImNode::EndNode();
}

InputPin* Mux21Circuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in0;
	case 1:
		return &m_in1;
	case 2:
		return &m_select;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* Mux21Circuit::GetOutputPin(int index)
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

void Mux21Circuit::updateOutput()
{
	bool* outBuf = getOutputDataBuffer(0);

	if (m_select.ReadAt(0) == false)
	{
		for (int i = 0; i < 32; i++)
		{
			outBuf[i] = m_in0.ReadAt(i);
		}
	}
	else
	{
		for (int i = 0; i < 32; i++)
		{
			outBuf[i] = m_in1.ReadAt(i);
		}
	}
}
