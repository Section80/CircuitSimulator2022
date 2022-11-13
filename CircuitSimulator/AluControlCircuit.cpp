#include "stdafx.h"
#include "Convert.h"
#include "AluControlCircuit.h"

AluControlCircuit::AluControlCircuit()
	: Circuit("ALU Control", ECircuitType::ALUControl, 2, 1, m_outBuf1, m_outBuf2, 4, 0.5f)
	, m_op(*this, "op", 2)
	, m_funct(*this, "funct", 6)
	, m_control(*this, "ALU Ctrl", 0, 4)
{}

AluControlCircuit::AluControlCircuit(float x, float y)
	: AluControlCircuit()
{
	SetPos(x, y);
}

void AluControlCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_op.Render();
	ImGui::SameLine();
	m_control.Render();
	m_funct.Render();

	ImNode::EndNode();
}

InputPin* AluControlCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_op;
	case 1:
		return &m_funct;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* AluControlCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_control;
	default:
		assert(false);
	}

	return nullptr;
}

void AluControlCircuit::updateOutput()
{
	bool buffer[4] = { 0 };

	// 00
	if (!m_op.ReadAt(0) && !m_op.ReadAt(1))
	{
		buffer[1] = true;
	}
	else if (m_op.ReadAt(0))	// x1
	{
		buffer[1] = true;
		buffer[2] = true;
	}
	else
	{
		bool f0 = m_funct.ReadAt(0);
		bool f1 = m_funct.ReadAt(1);
		bool f2 = m_funct.ReadAt(2);
		bool f3 = m_funct.ReadAt(3);
		// f4, f5ดย don't care

		if (!f3 && !f2 && !f1 && !f0)		// 0000
		{
			// 0010
			buffer[1] = true;
		}
		else if (!f3 && !f2 && f1 && !f0)	// 0010
		{
			// 0110
			buffer[1] = true;
			buffer[2] = true;
		}
		else if (!f3 && f2 && !f1 && !f0)	// 0100
		{
			// 0000
		}
		else if (!f3 && f2 && !f1 && f0)	// 0101
		{
			// 0001
			buffer[0] = true;
		}
		else if (f3 && !f2 && f1 && !f0)	// 1010
		{
			// 0111
			buffer[0] = true;
			buffer[1] = true;
			buffer[2] = true;
		}
	}

	setOutputData(0, buffer);
}
