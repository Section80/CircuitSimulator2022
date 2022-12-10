#include "stdafx.h"
#include "Convert.h"
#include "ExMemRegisterCircuit.h"

ExMemRegisterCircuit::ExMemRegisterCircuit()
	: Circuit(
		"EX/MEM Register", ECircuitType::ExMem, 
		8, 7, m_outBuf1, m_outBuf2, 
		73, 0.5f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_memRead_in(*this, "memRead", 1)
	, m_memWrite_in(*this, "memWrite", 1)
	, m_aluResult_in(*this, "aluRes", 32)
	, m_read2_in(*this, "read2", 32)
	, m_writeReg_in(*this, "writeReg", 5)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 72, 1)
	, m_memToReg_out(*this, "memToReg",71, 1)
	, m_memRead_out(*this, "memRead", 70, 1)
	, m_memWrite_out(*this, "memWrite", 69, 1)
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

void ExMemRegisterCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(195.0f);
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM ========", 2, 2);
	renderIOGroup("======== EX/MEM ========", 4, 3);
	m_clock.Render();

	ImNode::EndNode();
}

void ExMemRegisterCircuit::RenderWire(bool bSummary)
{
	if (bSummary)
	{
		m_aluResult_out.RenderWire();
		m_read2_out.RenderWire();
		m_writeReg_out.RenderWire();
	}
	else
	{
		Circuit::RenderWire();
	}
}

void ExMemRegisterCircuit::RenderInspector()
{
	ImGui::Text(" regWrite: %d", m_regWrite_out.Value());
	ImGui::Text(" memToReg: %d", m_memToReg_out.Value());
	ImGui::Text("  memRead: %d", m_memRead_out.Value());
	ImGui::Text(" memWrite: %d", m_memWrite_out.Value());
	ImGui::Text("aluResult: %d", m_aluResult_out.Value());
	ImGui::Text("    read2: %d", m_read2_out.Value());
	ImGui::Text(" writeReg: %d", m_writeReg_out.Value());
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
		return &m_memRead_in;
	case 3:
		return &m_memWrite_in;
	case 4:
		return &m_aluResult_in;
	case 5:
		return &m_read2_in;
	case 6:
		return &m_writeReg_in;
	case 7:
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
		return &m_memRead_out;
	case 3:
		return &m_memWrite_out;
	case 4:
		return &m_aluResult_out;
	case 5:
		return &m_read2_out;
	case 6:
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
		for (int i = 0; i < GetOutputPinCount(); i++)	// GetInputPinGount() �� �ϸ� Clock���� ���ԵǼ� �������ȴ�. 
		{
			InputPin& in = *GetInputPin(i);
			m_data[i] = in.Value();
		}

		// �Է��� ������ �ʴ��� ����� ������Ʈ�ϵ���
		// ���� �����̸� ���½�Ų��. 
		resetDelay();
	}
}
