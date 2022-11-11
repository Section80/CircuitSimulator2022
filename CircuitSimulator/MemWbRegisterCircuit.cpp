#include "stdafx.h"
#include "Convert.h"
#include "MemWbRegisterCircuit.h"

MemWbRegisterCircuit::MemWbRegisterCircuit()
	: Circuit("MEM/WB Register", 3, 2, m_outBuf1, m_outBuf2, 64, 0.1f)
	, m_readData_in(*this, "rData", 32)
	, m_aluResult_in(*this, "aluRes", 32)
	, m_clock(*this, "clock", 1)
	, m_readData_out(*this, "rData", 0, 32)
	, m_aluResult_out(*this, "aluRes", 32, 32)
{
	memset(m_data, 0, sizeof(uint32_t) * 2);
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

InputPin* MemWbRegisterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_readData_in;
	case 1:
		return &m_aluResult_in;
	case 2:
		return &m_clock;
	}
	return nullptr;
}

OutputPin* MemWbRegisterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_readData_out;
	case 1:
		return &m_aluResult_out;
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
