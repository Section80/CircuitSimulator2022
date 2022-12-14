#include "stdafx.h"
#include "Convert.h"
#include "MuxCircuit.h"

Mux21Circuit::Mux21Circuit()
	: Circuit("2 to 1 Mux", ECircuitType::Mux21, 3, 1, m_outBuf1, m_outBuf2, 32, 0.1f)
	, m_in0(*this, "in0", 1)
	, m_in1(*this, "in1", 1)
	, m_select(*this, "select", 1)
	, m_out(*this, "out", 0, 1)
	, m_wireLineCount(1)
	, m_intInputBuffer(1)
	, m_intInputId(Identifiable::GetNewId())
{}

Mux21Circuit::Mux21Circuit(float x, float y)
	: Mux21Circuit()
{
	SetPos(x, y);
}

void Mux21Circuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in0.Render();
	ImGui::SameLine();
	m_out.Render();
	m_in1.Render();
	m_select.Render();

	ImGui::PushID(m_intInputId);
		ImGui::PushItemWidth(40.0f);
		bool bRet = ImGui::InputInt(
			"count",
			&m_intInputBuffer, 0, 0,
			ImGuiInputTextFlags_EnterReturnsTrue
		);
		ImGui::PopItemWidth();
	ImGui::PopID();
	if (bRet)
	{
		SetWireLineCount(m_intInputBuffer);
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

void Mux21Circuit::SetWireLineCount(int count)
{
	if (m_wireLineCount < 1) m_wireLineCount = 1;
	if (m_wireLineCount > 32) m_wireLineCount = 32;

	if (m_wireLineCount != count)
	{
		m_wireLineCount = count;
		m_intInputBuffer = count;

		for (int i = 0; i < 2; i++)
		{
			InputPin* in = GetInputPin(i);
			OutputPin* from = in->GetFrom();
			if (from != nullptr)
			{
				from->Disconnect(in);
			}
			in->setWireLineCount(m_wireLineCount);
		}

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

void Mux21Circuit::updateOutput()
{
	int select = m_select.Value();
	if (select == 0)
	{
		int val = m_in0.Value();
		if (val > 100000000)
		{
			int a = 0;
			a++;
		}
		setOutputDataByValue(0, val);
	}
	else
	{
		int val = m_in1.Value();
		if (val > 100000000)
		{
			// ?????? ???????? ??????. 
			// m_in1?? SignExtension????. 
			int a = 0;
			a++;
		}
		setOutputDataByValue(0, val);
	}
}

Mux31Circuit::Mux31Circuit()
	: Circuit("3 to 1 Mux", ECircuitType::Mux31, 4, 1, m_outBuf1, m_outBuf2, 32, 0.1f)
	, m_in0(*this, "in0", 1)
	, m_in1(*this, "in1", 1)
	, m_in2(*this, "in2", 1)
	, m_select(*this, "select", 2)
	, m_out(*this, "out", 0, 1)
	, m_intInputId(Identifiable::GetNewId())
	, m_intInputBuffer(1)
	, m_wireLineCount(1)

{}

Mux31Circuit::Mux31Circuit(float x, float y)
	: Mux31Circuit()
{
	SetPos(x, y);
}

void Mux31Circuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in0.Render();
	ImGui::SameLine();
	m_out.Render();
	m_in1.Render();
	m_in2.Render();
	m_select.Render();

	ImGui::PushID(m_intInputId);
	ImGui::PushItemWidth(40.0f);
	bool bRet = ImGui::InputInt(
		"count",
		&m_intInputBuffer, 0, 0,
		ImGuiInputTextFlags_EnterReturnsTrue
	);
	ImGui::PopItemWidth();
	ImGui::PopID();
	if (bRet)
	{
		SetWireLineCount(m_intInputBuffer);
	}

	ImNode::EndNode();
}

InputPin* Mux31Circuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in0;
	case 1:
		return &m_in1;
	case 2:
		return &m_in2;
	case 3:
		return &m_select;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* Mux31Circuit::GetOutputPin(int index)
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

void Mux31Circuit::SetWireLineCount(int count)
{
	if (m_wireLineCount < 1) m_wireLineCount = 1;
	if (m_wireLineCount > 32) m_wireLineCount = 32;

	if (m_wireLineCount != count)
	{
		m_wireLineCount = count;
		m_intInputBuffer = count;

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

		if (m_in2.GetFrom() != nullptr)
		{
			m_in2.GetFrom()->Disconnect(&m_in2);
		}
		m_in2.setWireLineCount(m_wireLineCount);

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

void Mux31Circuit::updateOutput()
{
	int select = m_select.Value();
	if (select == 0b00)
	{
		int val = m_in0.Value();
		if (val > 100000000)
		{
			int a = 0;
			a++;
		}
		setOutputDataByValue(0, val);
	}
	else if(select == 0b01)
	{
		int val = m_in1.Value();
		if (val > 100000000)
		{
			int a = 0;
			a++;
		}
		setOutputDataByValue(0, val);
	}
	else if (select == 0b10)
	{
		int val = m_in2.Value();
		if (val > 100000000)
		{
			int a = 0;
			a++;
		}
		setOutputDataByValue(0, val);
	}
}
