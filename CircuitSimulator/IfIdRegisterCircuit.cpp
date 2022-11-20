#include "stdafx.h"
#include "Convert.h"
#include "IfIdRegisterCircuit.h"

IfIdRegisterCircuit::IfIdRegisterCircuit()
	: Circuit("IF/ID Register", ECircuitType::IfId, 3, 6, m_outBuf1, m_outBuf2, 64, 0.5f)
	, m_pc_in(*this, "pc", 32)
	, m_instruction_in(*this, "instruction", 32)
	, m_clock(*this, "clock", 1)
	, m_pc_out(*this, "pc", 32, 32)
	, m_op_out(*this, "op", 26, 6)
	, m_rs_out(*this, "rs", 21, 5)
	, m_rt_out(*this, "rt", 16, 5)
	, m_rd_out(*this, "rd", 11, 5)
	, m_low16_out(*this, "low16", 0, 16)
	, m_bLastClock(false)
{
	memset(m_data, 0, sizeof(uint32_t) * GetOutputPinCount());
}

IfIdRegisterCircuit::IfIdRegisterCircuit(float x, float y)
	: IfIdRegisterCircuit()
{
	SetPos(x, y);
}

void IfIdRegisterCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(180.0f);
	ImGui::BeginHorizontal("IO");
	ImGui::BeginVertical("in");

	for (int i = 0; i < GetInputPinCount(); i++)
	{
		InputPin* in = GetInputPin(i);
		in->Render();
	}

	ImGui::EndVertical();
	ImGui::BeginVertical("out");

	for (int i = 0; i < GetOutputPinCount(); i++)
	{
		OutputPin* in = GetOutputPin(i);
		in->Render();
	}

	ImGui::EndVertical();
	ImGui::EndHorizontal();

	ImNode::EndNode();
}

void IfIdRegisterCircuit::RenderInspector()
{
	ImGui::Text("pc: %d", m_pc_out.Value());
	ImGui::Text("op: %d", m_op_out.Value());
	ImGui::Text("rs: %d", m_rs_out.Value());
	ImGui::Text("rt: %d", m_rt_out.Value());
	ImGui::Text("rd: %d", m_rd_out.Value());
	ImGui::Text("low16: %d", m_low16_out.Value());
}

InputPin* IfIdRegisterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_pc_in;
	case 1:
		return &m_instruction_in;
	case 2:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* IfIdRegisterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_pc_out;
	case 1:
		return &m_op_out;
	case 2:
		return &m_rs_out;
	case 3:
		return &m_rt_out;
	case 4:
		return &m_rd_out;
	case 5:
		return &m_low16_out;
	default:
		assert(false);
	}

	return nullptr;
}

void IfIdRegisterCircuit::updateOutput()
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
		InputPin* in = GetInputPin(0);

		// m_pcOut;
		m_data[0] = ReadToUint32(*in, in->GetWireLineCount());

		in = GetInputPin(1);

		// m_op_out;
		m_data[1] = ReadToUint32(*in, 26, 6);
		// m_rs_out;
		m_data[2] = ReadToUint32(*in, 21, 5);
		// m_rt_out;
		m_data[3] = ReadToUint32(*in, 16, 5);
		// m_rd_out;
		m_data[4] = ReadToUint32(*in, 11, 5);
		// m_low16_out
		m_data[5] = ReadToUint32(*in, 0, 16);

		// 입력이 변하지 않더라도 출력을 업데이트하도록
		// 남은 딜레이를 리셋시킨다. 
		resetDelay();
	}
}
