#include "stdafx.h"
#include "Convert.h"
#include "IdExRegisterCircuit.h"


IdExRegisterCircuit::IdExRegisterCircuit()
	: Circuit("ID/EX Register", ECircuitType::IdEx, 16, 15, m_outBuf1, m_outBuf2, 152, 0.5f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_branch_in(*this, "branch", 1)
	, m_memRead_in(*this, "MemRead", 1)
	, m_memWrite_in(*this, "MemWrite", 1)
	, m_regDest_in(*this, "regDest", 1)
	, m_aluOp_in(*this, "aluOp", 2)
	, m_aluSrc_in(*this, "aluSrc", 1)
	, m_pc_in(*this, "pc", 32)
	, m_read1_in(*this, "read1", 32)
	, m_read2_in(*this, "read2", 32)
	, m_signExtended_in(*this, "sign ex", 32)
	, m_rs_in(*this, "rs", 5)
	, m_rt_in(*this, "rt", 5)
	, m_rd_in(*this, "rd", 5)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 151, 1)
	, m_memToReg_out(*this, "memToReg", 150, 1)
	, m_branch_out(*this, "branch", 149, 1)
	, m_memRead_out(*this, "memRead", 148, 1)
	, m_memWrite_out(*this, "memWrite", 147, 1)
	, m_regDest_out(*this, "regDest", 146, 1)
	, m_aluOp_out(*this, "aluOp", 144, 2)
	, m_aluSrc_out(*this, "aluSrc", 143, 1)
	, m_pc_out(*this, "pc", 111, 32)
	, m_read1_out(*this, "read1", 79, 32)
	, m_read2_out(*this, "read2", 47, 32)
	, m_signExtended_out(*this, "sign ex", 15, 32)
	, m_rs_out(*this, "rs", 10, 5)
	, m_rt_out(*this, "rt", 5, 5)
	, m_rd_out(*this, "rd", 0, 5)
	, m_bLastClock(false)
{
	memset(m_data, 0, sizeof(uint32_t) * GetOutputPinCount());
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
	renderIOGroup("======== MEM ========", 2, 3);
	renderIOGroup("======== EX ========", 5, 3);
	renderIOGroup("======== ID/EX ========", 8, 7);	
	m_clock.Render();

	ImNode::EndNode();
}

void IdExRegisterCircuit::RenderInspector()
{
	ImGui::Text("regWrite: %d", m_regWrite_out.Value());
	ImGui::Text("memToReg: %d", m_memToReg_out.Value());
	ImGui::Text("  branch: %d", m_branch_out.Value());
	ImGui::Text(" memRead: %d", m_memRead_out.Value());
	ImGui::Text("memWrite: %d", m_memWrite_out.Value());
	ImGui::Text(" regDest: %d", m_regDest_out.Value());
	ImGui::Text("   aluOp: %d", m_aluOp_out.Value());
	ImGui::Text("  aluSrc: %d", m_aluSrc_out.Value());
	ImGui::Text("      pc: %d", m_pc_out.Value());
	ImGui::Text("   read1: %d", m_read1_out.Value());
	ImGui::Text("   read2: %d", m_read2_out.Value());
	ImGui::Text("  signEx: %d", m_signExtended_out.Value());
	ImGui::Text("      rs: %d", m_rs_out.Value());
	ImGui::Text("      rt: %d", m_rt_out.Value());
	ImGui::Text("      rd: %d", m_rd_out.Value());
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
		return &m_branch_in;
	case 3:
		return &m_memRead_in;
	case 4:
		return &m_memWrite_in;
	case 5:
		return &m_regDest_in;
	case 6:
		return &m_aluOp_in;
	case 7:
		return &m_aluSrc_in;
	case 8:
		return &m_pc_in;
	case 9:
		return &m_read1_in;
	case 10:
		return &m_read2_in;
	case 11:
		return &m_rs_in;
	case 12:
		return &m_rt_in;
	case 13:
		return &m_rd_in;
	case 14:
		return &m_signExtended_in;
	case 15:
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
		return &m_branch_out;
	case 3:
		return &m_memRead_out;
	case 4:
		return &m_memWrite_out;
	case 5:
		return &m_regDest_out;
	case 6:
		return &m_aluOp_out;
	case 7:
		return &m_aluSrc_out;
	case 8:
		return &m_pc_out;
	case 9:
		return &m_read1_out;
	case 10:
		return &m_read2_out;
	case 11:
		return &m_rs_out;
	case 12:
		return &m_rt_out;
	case 13:
		return &m_rd_out;
	case 14:
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
		for (int i = 0; i < GetOutputPinCount(); i++)	// GetInputPinGount() 로 하면 Clock까지 포함되서 오버런된다. 
		{
			InputPin& in = *GetInputPin(i);
			m_data[i] = ReadToUint32(in, in.GetWireLineCount());

			if (m_data[i] > 100000000)
			{
				int a = 0;
				a++;
			}
		}

		// 입력이 변하지 않더라도 출력을 업데이트하도록
		// 남은 딜레이를 리셋시킨다. 
		resetDelay();
	}
}
