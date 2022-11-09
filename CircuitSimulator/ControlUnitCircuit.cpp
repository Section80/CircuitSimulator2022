#include "stdafx.h"
#include "ControlUnitCircuit.h"

ControlUnitCircuit::ControlUnitCircuit()
	: Circuit("Control Unit", 1, 8, m_outBuf1, m_outBuf2, 7, 0.1f)
	, m_op(*this, "op", 5)
	, m_regDest(*this, "regDest", 0, 1)
	, m_regWrite(*this, "regWrite", 1, 1)
	, m_aluSrc(*this, "aluSrc", 2, 1)
	, m_branch(*this, "branch", 3, 1)
	, m_memRead(*this, "memRead", 4, 1)
	, m_memWrite(*this, "memWrite", 5, 1)
	, m_memToReg(*this, "memToReg", 6, 1)
	, m_aluOp(*this, "aluOp", 5, 2)
{}

ControlUnitCircuit::ControlUnitCircuit(float x, float y)
	: ControlUnitCircuit()
{
	SetPos(x, y);
}

void ControlUnitCircuit::render()
{
	ImNode::BeginNode(GetNodeId());
	ImGui::Text(GetName());

	// ImGui::BeginHorizontal("IO");

	// ImGui::BeginVertical("Input");
	m_op.Render();
	// ImGui::EndVertical();
	
	// ImGui::BeginVertical("Output");
	m_regDest.Render();
	m_regWrite.Render();
	m_aluSrc.Render();
	m_branch.Render();
	m_memRead.Render();
	m_memWrite.Render();
	m_memToReg.Render();
	m_aluOp.Render();
	// ImGui::EndVertical();
	
	// ImGui::EndHorizontal();

	ImNode::EndNode();
}

InputPin* ControlUnitCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_op;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* ControlUnitCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_regDest;
	case 1:
		return &m_regWrite;
	case 2:
		return &m_aluSrc;
	case 3:
		return &m_branch;
	case 4:
		return &m_memRead;
	case 5:
		return &m_memWrite;
	case 6:
		return &m_memToReg;
	case 7:
		return &m_aluOp;
	default:
		assert(false);
	}

	return nullptr;
}

void ControlUnitCircuit::updateOutput()
{
}

