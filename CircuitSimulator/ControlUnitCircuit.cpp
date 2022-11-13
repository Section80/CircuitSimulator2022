#include "stdafx.h"
#include "Convert.h"
#include "ControlUnitCircuit.h"

ControlUnitCircuit::ControlUnitCircuit()
	: Circuit("Control Unit", ECircuitType::ControlUnit, 1, 8, m_outBuf1, m_outBuf2, 7, 0.1f)
	, m_op(*this, "op", 6)
	, m_regDest(*this, "regDest", 8, 1)
	, m_aluSrc(*this, "aluSrc", 7, 1)
	, m_memToReg(*this, "memToReg", 6, 1)
	, m_regWrite(*this, "regWrite", 5, 1)
	, m_memRead(*this, "memRead", 7, 1)
	, m_memWrite(*this, "memWrite", 3, 1)
	, m_branch(*this, "branch", 2, 1)
	, m_aluOp(*this, "aluOp", 0, 2)
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

	ImGui::Text("==== WB ==== ");
	m_regWrite.Render();
	m_memToReg.Render();
	ImGui::Text("==== MEM ==== ");
	m_branch.Render();
	m_memRead.Render();
	m_memWrite.Render();
	ImGui::Text("==== EX ==== ");
	m_regDest.Render();
	m_aluOp.Render();
	m_aluSrc.Render();
	ImGui::Text("==== IN ==== ");
	m_op.Render();

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
		return &m_regWrite;
	case 1:
		return &m_memToReg;
	case 2:
		return &m_branch;
	case 3:
		return &m_memRead;
	case 4:
		return &m_memWrite;
	case 5:
		return &m_regDest;
	case 6:
		return &m_aluOp;
	case 7:
		return &m_aluSrc;
	default:
		assert(false);
	}

	return nullptr;
}

void ControlUnitCircuit::updateOutput()
{
	bool* outBuf = getOutputDataBuffer(0);

	uint32_t op = ReadToUint32(m_op, 6);
	uint32_t val = 0;
	switch (op)
	{
	case 0b000000:		// R-Format
		val = 0b100100010;
		break;
	case 0b100011:		// lw
		val = 0b011110000;
		break;
	case 0b101011:		// sw
		val = 0b010001000;
		break;
	case 0b000100:
		val = 0b000000101;
		break;
	default:
		assert(false);
	}
	Uint32ToBoolArray(val, outBuf, 9);
}

