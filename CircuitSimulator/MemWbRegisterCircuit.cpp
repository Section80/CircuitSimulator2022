#include "stdafx.h"
#include "Convert.h"
#include "MemWbRegisterCircuit.h"

MemWbRegisterCircuit::MemWbRegisterCircuit()
	: Circuit("MEM/WB Register", ECircuitType::MemWb, 6, 5, m_outBuf1, m_outBuf2, 71, 0.5f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_readData_in(*this, "rData", 32)
	, m_aluResult_in(*this, "aluRes", 32)
	, m_writeReg_in(*this, "writeReg", 5)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 70, 1)
	, m_memToReg_out(*this, "memToReg", 69, 1)
	, m_readData_out(*this, "rData", 37, 32)
	, m_aluResult_out(*this, "aluRes", 5, 32)
	, m_writeReg_out(*this, "writeReg", 0, 5)
{
	memset(m_data, 0, sizeof(uint32_t) * 2);
	memset(m_strBuf, 0, sizeof(char) * 256);
}

MemWbRegisterCircuit::MemWbRegisterCircuit(float x, float y)
	: MemWbRegisterCircuit()
{
	SetPos(x, y);
}

void MemWbRegisterCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(180.0f);
	
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM/WB ========", 2, 3);
	m_clock.Render();

	ImNode::EndNode();
}

void MemWbRegisterCircuit::RenderInspector()
{
	sprintf_s(m_strBuf, "  regWrite: %d", m_regWrite_out.Value());
	ImGui::Text(m_strBuf);
	sprintf_s(m_strBuf, "  memToReg: %d", m_memToReg_out.Value());
	ImGui::Text(m_strBuf);
	sprintf_s(m_strBuf, "  readData: %d", m_readData_out.Value());
	ImGui::Text(m_strBuf);
	sprintf_s(m_strBuf, " aluResult: %d", m_aluResult_out.Value());
	ImGui::Text(m_strBuf);
	sprintf_s(m_strBuf, "m_writeReg: %d", m_writeReg_out.Value());
	ImGui::Text(m_strBuf);
}

InputPin* MemWbRegisterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0: 
		return &m_regWrite_in;
	case 1:
		return &m_memToReg_in;
	case 2:
		return &m_readData_in;
	case 3:
		return &m_aluResult_in;
	case 4:
		return &m_writeReg_in;
	case 5:
		return &m_clock;
	}
	return nullptr;
}

OutputPin* MemWbRegisterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_regWrite_out;
	case 1:
		return &m_memToReg_out;
	case 2:
		return &m_readData_out;
	case 3:
		return &m_aluResult_out;
	case 4:
		return &m_writeReg_out;
	}
	return nullptr;
}

void MemWbRegisterCircuit::updateOutput()
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

	// update edge triggred part here
	for (int i = 0; i < GetOutputPinCount(); i++)
	{
		bool* outBuf = getOutputDataBuffer(i);
		int len = GetOutputPin(i)->GetWireLineCount();
		Uint32ToBoolArray(m_data[i], outBuf, len);
	}
}
