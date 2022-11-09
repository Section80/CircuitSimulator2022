#include "stdafx.h"
#include "MuxCircuit.h"

Mux21Circuit::Mux21Circuit()
	: Circuit("2 to 1 Mux", 3, 1, m_outBuf1, m_outBuf2, 32, 0.1f)
	, m_in0(*this, "in0", 1)
	, m_in1(*this, "in1", 1)
	, m_select(*this, "select", 1)
	, m_out(*this, "out", 0, 1)
	, m_wireLineCount(1)
	, m_intInputId(Identifiable::GetNewId())
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

	int oldCount = m_wireLineCount;
	ImGui::PushID(m_intInputId);
		ImGui::PushItemWidth(40.0f);
		bool bRet = ImGui::InputInt(
			"count",
			&m_wireLineCount, 0, 0,
			ImGuiInputTextFlags_EnterReturnsTrue
		);
		ImGui::PopItemWidth();
	ImGui::PopID();
	if (bRet)
	{
		if (m_wireLineCount < 1) m_wireLineCount = 1;
		if (m_wireLineCount > 32) m_wireLineCount = 32;

		if (m_wireLineCount != oldCount)
		{
			if (m_in0.GetFrom() != nullptr)
			{
				m_in0.GetFrom()->Disconnect(&m_in0);
			}
			m_in0.setWireLineCount(m_wireLineCount);

			if (m_in1.GetFrom() != nullptr)
			{
				m_in1.GetFrom()->Disconnect(&m_in1);
			}
			m_in1.setWireLineCount(m_wireLineCount);

			for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
			{
				Wire& w = m_out.m_wires[i];
				if (w.GetTo() != nullptr)
				{
					m_out.Disconnect(w.GetTo());
				}
			}
			m_out.setWireLineCount(m_wireLineCount);
		}
	}

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
