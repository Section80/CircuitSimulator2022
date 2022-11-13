#include "stdafx.h"
#include "Convert.h"
#include "MemWbRegisterCircuit.h"

MemWbRegisterCircuit::MemWbRegisterCircuit()
	: Circuit("MEM/WB Register", ECircuitType::MemWb, 5, 4, m_outBuf1, m_outBuf2, 66, 0.1f)
	, m_regWrite_in(*this, "regWrite", 1)
	, m_memToReg_in(*this, "memToReg", 1)
	, m_readData_in(*this, "rData", 32)
	, m_aluResult_in(*this, "aluRes", 32)
	, m_clock(*this, "clock", 1)
	, m_regWrite_out(*this, "regWrite", 65, 1)
	, m_memToReg_out(*this, "memToReg", 64, 1)
	, m_readData_out(*this, "rData", 32, 32)
	, m_aluResult_out(*this, "aluRes", 0, 32)
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
	
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM/WB ========", 2, 2);
	m_clock.Render();

	ImNode::EndNode();
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
			for (int i = 0; i < GetOutputPinCount(); i++)	// GetInputPinGount() �� �ϸ� Clock���� ���ԵǼ� �������ȴ�. 
			{
				InputPin& in = *GetInputPin(i);
				m_data[i] = ReadToUint32(in, in.GetWireLineCount());
			}

			// �Է��� ������ �ʴ��� ����� ������Ʈ�ϵ���
			// ���� �����̸� ���½�Ų��. 
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
