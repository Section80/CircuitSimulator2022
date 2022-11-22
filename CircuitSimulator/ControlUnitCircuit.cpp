#include "stdafx.h"
#include "Convert.h"
#include "ControlUnitCircuit.h"

ControlUnitCircuit::ControlUnitCircuit()
	: Circuit(
		"Control Unit", ECircuitType::ControlUnit, 
		1, 9, m_outBuf1, m_outBuf2, 
		10, 0.1f)
	, m_instruction(*this, "instr", 32)
	, m_regWrite(*this, "regWrite", 9, 1)
	, m_memToReg(*this, "memToReg", 8, 1)
	, m_branch(*this, "branch", 7, 1)
	, m_jump(*this, "jump", 6, 1)
	, m_memRead(*this, "memRead", 5, 1)
	, m_memWrite(*this, "memWrite", 4, 1)
	, m_regDest(*this, "regDest", 3, 1)
	, m_aluOp(*this, "aluOp", 1, 2)
	, m_aluSrc(*this, "aluSrc", 0, 1)
{}

ControlUnitCircuit::ControlUnitCircuit(float x, float y)
	: ControlUnitCircuit()
{
	SetPos(x, y);
}

void ControlUnitCircuit::Render()
{
	ImNode::BeginNode(GetNodeId());
	ImGui::Text(GetName());

	ImGui::Text("==== WB ==== ");
	m_regWrite.Render();
	m_memToReg.Render();
	ImGui::Text("==== MEM ==== ");
	m_branch.Render();
	m_jump.Render();
	m_memRead.Render();
	m_memWrite.Render();
	ImGui::Text("==== EX ==== ");
	m_regDest.Render();
	m_aluOp.Render();
	m_aluSrc.Render();
	ImGui::Text("==== IN ==== ");
	m_instruction.Render();

	ImNode::EndNode();
}

void ControlUnitCircuit::RenderInspector()
{
	if (m_instruction.GetFrom() != nullptr)
	{
		bool isNop = (0 == ReadToUint32(
			m_instruction, 
			m_instruction.GetWireLineCount()
		));

		if (isNop)
		{
			ImGui::Text("      op: NOP");
		}
		else
		{
			uint32_t op = ReadToUint32(m_instruction, 26, 6);
			switch (op)
			{
			case 0b000000:		// R-Format: 0
				ImGui::Text("      op: R-Format");
				break;
			case 0b100011:		// lw: 35
				ImGui::Text("      op: lw");
				break;
			case 0b101011:		// sw: 43
				ImGui::Text("      op: sw");
				break;
			case 0b000100:		// beq: 4
				ImGui::Text("      op: beq");
				break;
			case 0b000010:		// j: 2
				ImGui::Text("      op: jump");
				break;
			case 0b001000:		// addi: 8
				ImGui::Text("      op: addi");
				break;
			default:
				assert(false);
			}
		}
	}
	else
	{
		ImGui::Text("      op: not connected");
	}

	ImGui::Text("regWrite: %#2d", m_regWrite.Value());
	ImGui::Text("memToReg: %#2d", m_memToReg.Value());

	ImGui::Text("  branch: %#2d", m_branch.Value());
	ImGui::Text("    jump: %#2d", m_jump.Value());
	ImGui::Text(" memRead: %#2d", m_memRead.Value());
	ImGui::Text("memWrite: %#2d", m_memWrite.Value());


	ImGui::Text(" regDest: %#2d", m_regDest.Value());
	
	int val = m_aluOp.Value();
	switch (val)
	{
	case 0b00: 
		ImGui::Text("   aluOp: 00");
		break;
	case 0b01:
		ImGui::Text("   aluOp: 01");
		break;
	case 0b10:
		ImGui::Text("   aluOp: 10");
		break;
	case 0b11:
		ImGui::Text("   aluOp: 11");
		break;
	default:
		assert(false);
		break;
	}
	ImGui::Text("  aluSrc: %#2d", m_aluSrc.Value());
}

InputPin* ControlUnitCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_instruction;
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
		return &m_jump;
	case 4:
		return &m_memRead;
	case 5:
		return &m_memWrite;
	case 6:
		return &m_regDest;
	case 7:
		return &m_aluOp;
	case 8:
		return &m_aluSrc;
	default:
		assert(false);
	}

	return nullptr;
}

void ControlUnitCircuit::updateOutput()
{
	bool* outBuf = getOutputDataBuffer(6);

	bool isNop = (0 == ReadToUint32(
		m_instruction,
		m_instruction.GetWireLineCount()
	));

	if (isNop)
	{
		// all zero
		setOutputDataByValue(m_regWrite, 0);
		setOutputDataByValue(m_memToReg, 0);

		setOutputDataByValue(m_branch, 0);
		setOutputDataByValue(m_jump, 0);
		setOutputDataByValue(m_memRead, 0);
		setOutputDataByValue(m_memWrite, 0);

		setOutputDataByValue(m_regDest, 0);
		setOutputDataByValue(m_aluOp, 0b00);
		setOutputDataByValue(m_aluSrc, 0);
	}
	else
	{
		uint32_t op = ReadToUint32(m_instruction, 26, 6);
		switch (op)
		{
		case 0b000000:		// R-Format: 0
			setOutputDataByValue(m_regWrite, 1);
			setOutputDataByValue(m_memToReg, 0);

			setOutputDataByValue(m_branch, 0);
			setOutputDataByValue(m_jump, 0);
			setOutputDataByValue(m_memRead, 0);
			setOutputDataByValue(m_memWrite, 0);

			setOutputDataByValue(m_regDest, 1);
			setOutputDataByValue(m_aluOp, 0b10);
			setOutputDataByValue(m_aluSrc, 0);
			break;
		case 0b100011:		// lw: 35
			setOutputDataByValue(m_regWrite, 1);
			setOutputDataByValue(m_memToReg, 1);

			setOutputDataByValue(m_branch, 0);
			setOutputDataByValue(m_jump, 0);
			setOutputDataByValue(m_memRead, 1);
			setOutputDataByValue(m_memWrite, 0);

			setOutputDataByValue(m_regDest, 0);
			setOutputDataByValue(m_aluOp, 0b00);
			setOutputDataByValue(m_aluSrc, 1);
			break;
		case 0b101011:		// sw: 43
			setOutputDataByValue(m_regWrite, 0);
			setOutputDataByValue(m_memToReg, 0);

			setOutputDataByValue(m_branch, 0);
			setOutputDataByValue(m_jump, 0);
			setOutputDataByValue(m_memRead, 0);
			setOutputDataByValue(m_memWrite, 1);

			setOutputDataByValue(m_regDest, 0);
			setOutputDataByValue(m_aluOp, 0b00);
			setOutputDataByValue(m_aluSrc, 1);
			break;
		case 0b000100:		// beq: 4
			setOutputDataByValue(m_regWrite, 0);
			setOutputDataByValue(m_memToReg, 0);

			setOutputDataByValue(m_branch, 1);
			setOutputDataByValue(m_jump, 0);
			setOutputDataByValue(m_memRead, 0);
			setOutputDataByValue(m_memWrite, 0);

			setOutputDataByValue(m_regDest, 0);
			setOutputDataByValue(m_aluOp, 0b01);
			setOutputDataByValue(m_aluSrc, 0);
			break;
		case 0b000010:		// j: 2
			setOutputDataByValue(m_regWrite, 0);
			setOutputDataByValue(m_memToReg, 0);

			setOutputDataByValue(m_branch, 1);
			setOutputDataByValue(m_jump, 1);
			setOutputDataByValue(m_memRead, 0);
			setOutputDataByValue(m_memWrite, 0);

			setOutputDataByValue(m_regDest, 0);
			setOutputDataByValue(m_aluOp, 0b00);
			setOutputDataByValue(m_aluSrc, 0);
			break;
		case 0b001000:		// addi: 8
			setOutputDataByValue(m_regWrite, 1);
			setOutputDataByValue(m_memToReg, 0);

			setOutputDataByValue(m_branch, 0);
			setOutputDataByValue(m_jump, 0);
			setOutputDataByValue(m_memRead, 0);
			setOutputDataByValue(m_memWrite, 0);

			setOutputDataByValue(m_regDest, 0);
			setOutputDataByValue(m_aluOp, 0b00);
			setOutputDataByValue(m_aluSrc, 1);
			break;
		default:
			assert(false);
		}
	}
}

