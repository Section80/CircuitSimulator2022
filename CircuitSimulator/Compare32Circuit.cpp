#include "stdafx.h"
#include "Compare32Circuit.h"

Compare32Circuit::Compare32Circuit()
	: Circuit(
		"Compare32", ECircuitType::Compare32, 
		2, 1, &m_outBuf1, &m_outBuf2, 1, 0.1f)
	, m_in1(*this, "in1", 32)
	, m_in2(*this, "in2", 32)
	, m_equal(*this, "equal", 0, 1)
{}

Compare32Circuit::Compare32Circuit(float x, float y)
	: Compare32Circuit()
{
	SetPos(x, y);
}

void Compare32Circuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());
	
	m_in1.Render();
	ImGui::SameLine();
	m_equal.Render();
	m_in2.Render();

	ImNode::EndNode();
}

void Compare32Circuit::RenderInspector()
{
	if (m_in1.GetFrom() != nullptr)
	{
		ImGui::Text("in1: %d", m_in1.Value());
	}
	else
	{
		ImGui::Text("in1: not connected");
	}

	if (m_in2.GetFrom() != nullptr)
	{
		ImGui::Text("in2: %d", m_in2.Value());
	}
	else
	{
		ImGui::Text("in2: not connected");
	}

	if (m_equal.Value() == 1)
	{
		ImGui::Text("equal: true");
	}
	else
	{
		ImGui::Text("equal: false");
	}
}

InputPin* Compare32Circuit::GetInputPin(int index)
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

OutputPin* Compare32Circuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_equal;
	default:
		assert(false);
	}

	return nullptr;
}

void Compare32Circuit::updateOutput()
{
	int val1 = m_in1.Value();
	int val2 = m_in2.Value();

	if (val1 == val2)
		setOutputDataByValue(m_equal, 1);
	else
		setOutputDataByValue(m_equal, 0);
}
