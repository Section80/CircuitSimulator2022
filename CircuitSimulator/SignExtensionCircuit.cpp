#include "stdafx.h"
#include "SignExtensionCircuit.h"

SignExtensionCircuit::SignExtensionCircuit()
	: Circuit("Sign Extension", 1, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_in(*this, "in", 16)
	, m_out(*this, "out", 0, 32)
{}

SignExtensionCircuit::SignExtensionCircuit(float x, float y)
	: SignExtensionCircuit()
{
	SetPos(x, y);
}

void SignExtensionCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in.Render();
	ImGui::SameLine();
	m_out.Render();

	ImNode::EndNode();
}

InputPin* SignExtensionCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* SignExtensionCircuit::GetOutputPin(int index)
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

void SignExtensionCircuit::updateOutput()
{
	bool buf[32] = { 0 };
	// 15 ~ 0: 입력과 동일
	for (int i = 0; i < 16; i++)
	{
		bool bVal = m_in.ReadAt(i);
		buf[i] = bVal;
	}
	
	// 31 ~ 16: 15를 복사한다.
	for (int i = 16; i < 32; i++)
	{
		bool bVal = m_in.ReadAt(15);
		buf[i] = bVal;
	}
	
	setOutputData(0, buf);
}
