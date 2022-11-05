#include "stdafx.h"
#include "Conversion.h"
#include "RegisterCircuit.h"


RegisterCircuit::RegisterCircuit()
	: Circuit("RegisterFile", 6, 2, m_outBuffuer1, m_outBuffer2, 64, 2.0f)
	, m_rReg1(*this, "rReg1", 5)
	, m_rReg2(*this, "rReg2", 5)
	, m_wReg(*this, "wReg", 5)
	, m_wData(*this, "wData", 32)
	, m_clockInput(*this, "clock", 1)
	, m_regWrite(*this, "regWrite", 1)
	, m_readData1(*this, "rData1", 0, 32)
	, m_readData2(*this, "rData2", 0, 32)
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
	// 클락 시그널과 regWrite이 1이면
	if (m_clockInput.ReadAt(0) && m_regWrite.ReadAt(0))
	{
		uint32_t idx = ReadToUint32(m_wReg, 5);
		assert(idx < 32);
		int val = ReadToUint32(m_wData, 32);
		m_registers[idx] = val;
	}

	// update m_readData1
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
}
