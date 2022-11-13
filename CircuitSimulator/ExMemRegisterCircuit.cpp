#include "stdafx.h"
#include "Convert.h"
#include "ExMemRegisterCircuit.h"

ExMemRegisterCircuit::ExMemRegisterCircuit()
	: Circuit("EX/MEM Register", ECircuitType::ExMem, 11, 10, m_outBuf1, m_outBuf2, 107, 0.1f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_branch_in(*this, "branch", 1)
	, m_memRead_in(*this, "memRead", 1)
	, m_memWrite_in(*this, "memWrite", 1)
	, m_bta_in(*this, "bta", 32)
	, m_zero_in(*this, "zero", 1)
	, m_aluResult_in(*this, "aluRes", 32)
	, m_read2_in(*this, "read2", 32)
	, m_writeReg_in(*this, "writeReg", 5)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 106, 1)
	, m_memToReg_out(*this, "memToReg",105, 1)
	, m_branch_out(*this, "branch", 104, 1)
	, m_memRead_out(*this, "memRead", 103, 1)
	, m_memWrite_out(*this, "memWrite", 102, 1)
	, m_bta_out(*this, "bta", 70, 32)
	, m_zero_out(*this, "zero", 69, 1)
	, m_aluResult_out(*this, "aluRes", 37, 32)
	, m_read2_out(*this, "read2", 5, 32)
	, m_writeReg_out(*this, "writeReg", 0, 5)
	, m_bLastClock(false)
{
	memset(m_data, 0, sizeof(uint32_t) * GetOutputPinCount());
}

ExMemRegisterCircuit::ExMemRegisterCircuit(float x, float y)
	: ExMemRegisterCircuit()
{
	SetPos(x, y);
}

void ExMemRegisterCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(195.0f);
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM ========", 2, 3);
	renderIOGroup("======== EX/MEM ========", 5, 5);
	m_clock.Render();

	ImNode::EndNode();
}

InputPin* ExMemRegisterCircuit::GetInputPin(int index)
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
		return &m_bta_in;
	case 6:
		return &m_zero_in;
	case 7:
		return &m_aluResult_in;
	case 8:
		return &m_read2_in;
	case 9:
		return &m_writeReg_in;
	case 10:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* ExMemRegisterCircuit::GetOutputPin(int index)
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
		return &m_bta_out;
	case 6:
		return &m_zero_out;
	case 7:
		return &m_aluResult_out;
	case 8:
		return &m_read2_out;
	case 9:
		return &m_writeReg_out;
	default:
		assert(false);
	}

	return nullptr;
}

void ExMemRegisterCircuit::updateOutput()
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

	if (bRisingEdge)
	{
		if (false)
		{
			// update edge triggred part here
			for (int i = 0; i < GetOutputPinCount(); i++)	// GetInputPinGount() 로 하면 Clock까지 포함되서 오버런된다. 
			{
				InputPin& in = *GetInputPin(i);
				m_data[i] = ReadToUint32(in, in.GetWireLineCount());
			}

			// 입력이 변하지 않더라도 출력을 업데이트하도록
			// 남은 딜레이를 리셋시킨다. 
			resetDelay();
		}
	}

	// update edge triggred part here
	for (int i = 0; i < GetOutputPinCount(); i++)
	{
		bool* outBuf = getOutputDataBuffer(i);
		int len = GetOutputPin(i)->GetWireLineCount();
		Uint32ToBoolArray(m_data[i], outBuf, len);
	}
}
