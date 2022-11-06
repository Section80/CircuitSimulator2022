#include "stdafx.h"
#include "Convert.h"
#include "RegisterCircuit.h"


RegisterCircuit::RegisterCircuit()
	: Circuit("RegisterFile", 6, 2, m_outBuffuer1, m_outBuffer2, 64, 1.0)
	, m_rReg1(*this, "rReg1", 5)
	, m_rReg2(*this, "rReg2", 5)
	, m_wReg(*this, "wReg", 5)
	, m_wData(*this, "wData", 32)
	, m_clockInput(*this, "clock", 1)
	, m_regWrite(*this, "regWrite", 1)
	, m_readData1(*this, "rData1", 0, 32)
	, m_readData2(*this, "rData2", 32, 32)
	, m_bLastClock(false)
{
	memset(m_registers, 0, sizeof(uint32_t) * 32);
}

RegisterCircuit::RegisterCircuit(float x, float y)
	: RegisterCircuit()
{
	SetPos(x,y);
}

void RegisterCircuit::render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
	
		m_rReg1.Render();
		ImGui::SameLine();
		m_readData1.Render();

		m_rReg2.Render();
		ImGui::SameLine();
		m_readData2.Render();

		m_wReg.Render();
		m_wData.Render();
		m_clockInput.Render();
		m_regWrite.Render();
	ImNode::EndNode();
}

InputPin* RegisterCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_rReg1;
	case 1:
		return &m_rReg2;
	case 2:
		return &m_wReg;
	case 3:
		return &m_wData;
	case 4:
		return &m_clockInput;
	case 5:
		return &m_regWrite;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* RegisterCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_readData1;
	case 1:
		return &m_readData2;
	default:
		assert(false);
	}

	return nullptr;
}

void RegisterCircuit::updateOutput()
{
	bool bClock = m_clockInput.ReadAt(0);
	bool bRisingEdge = false;
	bool bFallingEdge = false;
	if (m_bLastClock != bClock)
	{
		if (bClock)
		{
			if (m_regWrite.ReadAt(0))	// rising edge
			{
				bRisingEdge = true;
			}
		}
		else {	// falling edge
			bFallingEdge = true;
		}
	}
	m_bLastClock = bClock;

	// rising edge든 falling edge든 상관없이, 항상 출력은 써둬야 한다. 

	// read index of register
	uint32_t regIdx1 = ReadToUint32(m_rReg1, 5);
	assert(0 <= regIdx1 && regIdx1 <= 31);

	// data stored in register
	uint32_t bits = m_registers[regIdx1];

	// set output data
	bool* data_buffer = getOutputDataBuffer(0);
	Uint32ToBoolArray(bits, data_buffer);


	// update m_readData2
	// read index of register
	uint32_t regIdx2 = ReadToUint32(m_rReg2, 5);
	assert(0 <= regIdx2 && regIdx2 <= 31);

	// data stored in register
	bits = m_registers[regIdx2];

	// set output data
	data_buffer = getOutputDataBuffer(1);
	Uint32ToBoolArray(bits, data_buffer);

	if (bRisingEdge)
	{
		// 상태를 업데이트시키지만, 출력은 나중에 업데이트된다. 
		// 여기서 변화시킨 상태는 다음 입력 변화 시 출력으로 적용될 것이다. 
		// 그것은 falling edge일 수도 있고, 다른 입력일 수도 있다. 

		// update edge triggred part here
		uint32_t idx = ReadToUint32(m_wReg, 5);
		assert(idx < 32);
		int val = ReadToUint32(m_wData, 32);
		m_registers[idx] = val;

		// 입력이 변하지 않더라도 출력을 업데이트하도록
		// 남은 딜레이를 리셋시킨다. 
		resetDelay();
	}
}
