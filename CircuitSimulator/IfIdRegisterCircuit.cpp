#include "stdafx.h"
#include "Convert.h"
#include "IfIdRegisterCircuit.h"

IfIdRegisterCircuit::IfIdRegisterCircuit()
	: Circuit("IF/ID Register", ECircuitType::IfId, 5, 7, m_outBuf1, m_outBuf2, 64, 0.5f)
	, m_pc_in(*this, "pc", 32)
	, m_instruction_in(*this, "instr", 32)
	, m_write(*this, "write", 1)
	, m_flush(*this, "flush", 1)
	, m_clock(*this, "clock", 1)
	, m_pc_out(*this, "pc", 32, 32)
	, m_instruction_out(*this, "instr", 0, 32)
	, m_rs_out(*this, "rs", 21, 5)
	, m_rt_out(*this, "rt", 16, 5)
	, m_rd_out(*this, "rd", 11, 5)
	, m_low16_out(*this, "low16", 0, 16)
	, m_addr_out(*this, "addr", 0, 26)
	, m_bLastClock(false)
	, m_pc(0)
	, m_instruction(0)
{
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

void IfIdRegisterCircuit::RenderWire(bool bSummary)
{
	if (bSummary)
	{
		m_pc_out.RenderWire();
		m_instruction_out.RenderWire();
		m_rs_out.RenderWire();
		m_rt_out.RenderWire();
		m_rd_out.RenderWire();
		m_low16_out.RenderWire();
		m_addr_out.RenderWire();
	}
	else 
	{
		Circuit::RenderWire();
	}
}

void IfIdRegisterCircuit::RenderInspector()
{
	ImGui::Text("pc: %0#10x", m_pc_out.Value());
	ImGui::Text("instruction: %0#10x", m_instruction_out.Value());
	ImGui::Text("rs: %d", m_rs_out.Value());
	ImGui::Text("rt: %d", m_rt_out.Value());
	ImGui::Text("rd: %d", m_rd_out.Value());
	ImGui::Text("low16: %d", m_low16_out.Value());
	ImGui::Text("addr: %d", m_addr_out.Value());
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
		return &m_write;
	case 3: 
		return &m_flush;
	case 4:
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
		return &m_instruction_out;
	case 2:
		return &m_rs_out;
	case 3:
		return &m_rt_out;
	case 4:
		return &m_rd_out;
	case 5:
		return &m_low16_out;
	case 6:
		return &m_addr_out;
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
	setOutputDataByValue(m_pc_out, m_pc);
	setOutputDataByValue(m_instruction_out, m_instruction);
	// 다른 출력들은 m_instruction과 버퍼를 공유하기 때문에 
	// m_instruction만 업데이트하면 된다. 

	if (bRisingEdge)
	{
		// update edge triggred part here
		if (m_write.Value() == 1)
		{
			uint32_t old_pc = m_pc;
			uint32_t old_instr = m_instruction;

			// m_pcOut;
			m_pc = ReadToUint32(
				m_pc_in,
				m_pc_in.GetWireLineCount()
			);

			// instruction 입력을 무시하고
			// nop를 쓴다. 
			if (m_flush.Value() == 1)
			{
				// m_op_out;
				m_instruction = 0;
			}
			else
			{
				// m_op_out;
				m_instruction = ReadToUint32(
					m_instruction_in,
					m_instruction_in.GetWireLineCount()
				);
			}

			// 입력이 변하지 않더라도 출력을 업데이트하도록
			// 남은 딜레이를 리셋시킨다. 
			// 출력이 변한 경우만.. 
			if (old_pc != m_pc || old_instr != m_instruction)
			{
				resetDelay();
			}
		}
	}
}
