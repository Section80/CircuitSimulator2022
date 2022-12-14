#include "stdafx.h"
#include "Convert.h"
#include "IfIdRegisterCircuit.h"
#include "IdExRegisterCircuit.h"
#include "ExMemRegisterCircuit.h"
#include "MemWbRegisterCircuit.h"
#include "InstructionMemory.h"

IdExRegisterCircuit* IdExRegisterCircuit::Instance = nullptr;

IdExRegisterCircuit::IdExRegisterCircuit()
	: Circuit("ID/EX Register", ECircuitType::IdEx, 14, 13, m_outBuf1, m_outBuf2, 119, 0.5f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_memRead_in(*this, "MemRead", 1)
	, m_memWrite_in(*this, "MemWrite", 1)
	, m_regDest_in(*this, "regDest", 1)
	, m_aluOp_in(*this, "aluOp", 2)
	, m_aluSrc_in(*this, "aluSrc", 1)
	, m_read1_in(*this, "read1", 32)
	, m_read2_in(*this, "read2", 32)
	, m_signExtended_in(*this, "sign ex", 32)
	, m_rs_in(*this, "rs", 5)
	, m_rt_in(*this, "rt", 5)
	, m_rd_in(*this, "rd", 5)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 118, 1)
	, m_memToReg_out(*this, "memToReg", 117, 1)
	, m_memRead_out(*this, "memRead", 116, 1)
	, m_memWrite_out(*this, "memWrite", 115, 1)
	, m_regDest_out(*this, "regDest", 114, 1)
	, m_aluOp_out(*this, "aluOp", 112, 2)
	, m_aluSrc_out(*this, "aluSrc", 111, 1)
	, m_read1_out(*this, "read1", 79, 32)
	, m_read2_out(*this, "read2", 47, 32)
	, m_signExtended_out(*this, "sign ex", 15, 32)
	, m_rs_out(*this, "rs", 10, 5)
	, m_rt_out(*this, "rt", 5, 5)
	, m_rd_out(*this, "rd", 0, 5)
	, m_bLastClock(false)
	, CurrentInstruction("nop")
{
	memset(m_data, 0, sizeof(uint32_t) * GetOutputPinCount());
	IdExRegisterCircuit::Instance = this;
}

IdExRegisterCircuit::IdExRegisterCircuit(float x, float y)
	: IdExRegisterCircuit()
{
	SetPos(x, y);
}

void IdExRegisterCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(190.0f);
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM ========", 2, 2);
	renderIOGroup("======== EX ========", 4, 3);
	renderIOGroup("======== ID/EX ========", 7, 6);	
	m_clock.Render();

	ImNode::EndNode();
}

void IdExRegisterCircuit::RenderWire(bool bSummary)
{
	if (bSummary)
	{
		m_read1_out.RenderWire();
		m_read2_out.RenderWire();
		m_signExtended_out.RenderWire();
		m_rs_out.RenderWire();
		m_rt_out.RenderWire();
		m_rd_out.RenderWire();
	}
	else
	{
		Circuit::RenderWire();
	}
}

void IdExRegisterCircuit::RenderInspector()
{
	ImGui::Text("Instruction: %s", CurrentInstruction.c_str());
	ImGui::Text("regWrite: %d", m_regWrite_out.Value());
	ImGui::Text("memToReg: %d", m_memToReg_out.Value());
	ImGui::Text(" memRead: %d", m_memRead_out.Value());
	ImGui::Text("memWrite: %d", m_memWrite_out.Value());
	ImGui::Text(" regDest: %d", m_regDest_out.Value());
	ImGui::Text("   aluOp: %d", m_aluOp_out.Value());
	ImGui::Text("  aluSrc: %d", m_aluSrc_out.Value());

	int read1Out = m_read1_out.Value();
	if (read1Out == 0)
	{
		ImGui::Text("   read1: 0x00000000");
	}
	else
	{
		ImGui::Text("   read1: %0#10x", read1Out);
	}

	int read2Out = m_read2_out.Value();
	if (read2Out == 0)
	{
		ImGui::Text("   read2: 0x00000000");
	}
	else
	{
		ImGui::Text("   read2: %0#10x", read2Out);
	}
	
	int signExtendedOut = m_signExtended_out.Value();
	if (signExtendedOut == 0)
	{
		ImGui::Text("  signEx: 0x00000000");
	}
	else
	{
		ImGui::Text("  signEx: %0#10x", signExtendedOut);
	}
	
	ImGui::Text("      rs: %d", m_rs_out.Value());
	ImGui::Text("      rt: %d", m_rt_out.Value());
	ImGui::Text("      rd: %d", m_rd_out.Value());

	InstructionMemoryCircuit* im = InstructionMemoryCircuit::Instance;
	int address = im->GetAddress();
	std::string& str = im->GetInstructionString(address);

	ImGui::NewLine();
	ImGui::Text(" IF: %s", str.c_str());
	ImGui::Text(" ID: %s", IfIdRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text(" EX: %s", IdExRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text("MEM: %s", ExMemRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text(" WB: %s", MemWbRegisterCircuit::Instance->CurrentInstruction.c_str());
}

InputPin* IdExRegisterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_regWrite_in;
	case 1:
		return &m_memToReg_in;
	case 2:
		return &m_memRead_in;
	case 3:
		return &m_memWrite_in;
	case 4:
		return &m_regDest_in;
	case 5:
		return &m_aluOp_in;
	case 6:
		return &m_aluSrc_in;
	case 7:
		return &m_read1_in;
	case 8:
		return &m_read2_in;
	case 9:
		return &m_rs_in;
	case 10:
		return &m_rt_in;
	case 11:
		return &m_rd_in;
	case 12:
		return &m_signExtended_in;
	case 13:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* IdExRegisterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_regWrite_out;
	case 1:
		return &m_memToReg_out;
	case 2:
		return &m_memRead_out;
	case 3:
		return &m_memWrite_out;
	case 4:
		return &m_regDest_out;
	case 5:
		return &m_aluOp_out;
	case 6:
		return &m_aluSrc_out;
	case 7:
		return &m_read1_out;
	case 8:
		return &m_read2_out;
	case 9:
		return &m_rs_out;
	case 10:
		return &m_rt_out;
	case 11:
		return &m_rd_out;
	case 12:
		return &m_signExtended_out;
	default:
		assert(false);
	}

	return nullptr;
}

void IdExRegisterCircuit::updateOutput()
{
	bool bClock = m_clock.ReadAt(0);
	bool bRisingEdge = false;
	bool bFallingEdge = false;
	if (m_bLastClock != bClock)
	{
		if (bClock)
		{
			// rising edge
			bRisingEdge = true;
		}
		else
		{
			// falling edge
			bFallingEdge = true;
		}
	}
	m_bLastClock = bClock;

	// update edge triggred part here
	for (int i = 0; i < GetOutputPinCount(); i++)
	{
		bool* outBuf = getOutputDataBuffer(i);
		int len = GetOutputPin(i)->GetWireLineCount();
		Uint32ToBoolArray(m_data[i], outBuf, len);
	}

	if (bRisingEdge)
	{
		// update edge triggred part here
		for (int i = 0; i < GetOutputPinCount(); i++)	// GetInputPinGount() ?? ???? Clock???? ???????? ??????????. 
		{
			InputPin& in = *GetInputPin(i);
			m_data[i] = ReadToUint32(in, in.GetWireLineCount());

			if (m_data[i] > 100000000)
			{
				int a = 0;
				a++;
			}
		}

		// ?????? ?????? ???????? ?????? ??????????????
		// ???? ???????? ??????????. 
		resetDelay();
	}
}
