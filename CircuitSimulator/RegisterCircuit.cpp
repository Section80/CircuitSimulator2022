#include "stdafx.h"
#include "Convert.h"
#include "RegisterCircuit.h"


RegisterCircuit::RegisterCircuit()
	: Circuit("Register File", ECircuitType::Register, 6, 2, m_outBuf1, m_outBuf2, 64, 0.5f)
	, m_rReg1(*this, "rReg1", 5)
	, m_rReg2(*this, "rReg2", 5)
	, m_wReg(*this, "wReg", 5)
	, m_wData(*this, "wData", 32)
	, m_clock(*this, "clock", 1)
	, m_regWrite(*this, "regWrite", 1)
	, m_readData1(*this, "rData1", 0, 32)
	, m_readData2(*this, "rData2", 32, 32)
	, m_bLastClock(false)
{
	memset(m_registers, 0, sizeof(uint32_t) * 32);
	memset(m_strBuffer, 0, sizeof(char) * 256);

	// https://ecampus.kookmin.ac.kr/mod/assign/view.php?id=1261943
	
	// $gp = 0x7ffffe40
	m_registers[28] = 0x7ffffe40;
	// $sp = 0x7ffffe40
	m_registers[29] = 0x7ffffe40;
	
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
		renderDelay(160.0f);

		m_rReg1.Render();
		ImGui::SameLine();
		m_readData1.Render();

		m_rReg2.Render();
		ImGui::SameLine();
		m_readData2.Render();

		m_wReg.Render();
		m_wData.Render();
		m_regWrite.Render();
		m_clock.Render();
	ImNode::EndNode();
}

void RegisterCircuit::RenderInspector()
{
	sprintf_s(m_strBuffer, "R0  [zr] = %d", m_registers[0]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R1  [at] = %d", m_registers[1]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R2  [v0] = %d", m_registers[2]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R3  [v1] = %d", m_registers[3]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R4  [a0] = %d", m_registers[4]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R5  [a1] = %d", m_registers[5]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R6  [a2] = %d", m_registers[6]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R7  [a3] = %d", m_registers[7]);
	ImGui::Text(m_strBuffer);

	sprintf_s(m_strBuffer, "R8  [t0] = %d", m_registers[8]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R9  [t1] = %d", m_registers[9]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R10 [t2] = %d", m_registers[10]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R11 [t3] = %d", m_registers[11]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R12 [t4] = %d", m_registers[12]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R13 [t5] = %d", m_registers[13]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R14 [t6] = %d", m_registers[14]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R15 [t7] = %d", m_registers[15]);
	ImGui::Text(m_strBuffer);

	sprintf_s(m_strBuffer, "R16 [s0] = %d", m_registers[16]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R17 [s1] = %d", m_registers[17]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R18 [s2] = %d", m_registers[18]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R19 [s3] = %d", m_registers[19]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R23 [s7] = %d", m_registers[23]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R20 [s4] = %d", m_registers[20]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R21 [s5] = %d", m_registers[21]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R22 [s6] = %d", m_registers[22]);
	ImGui::Text(m_strBuffer);

	sprintf_s(m_strBuffer, "R24 [t8] = %d", m_registers[24]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R25 [t9] = %d", m_registers[25]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R26 [k0] = %d", m_registers[26]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R27 [k1] = %d", m_registers[27]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R28 [gp] = %d", m_registers[28]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R29 [sp] = %d", m_registers[29]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R30 [fp] = %d", m_registers[30]);
	ImGui::Text(m_strBuffer);
	sprintf_s(m_strBuffer, "R31 [ra] = %d", m_registers[31]);
	ImGui::Text(m_strBuffer);
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
		return &m_regWrite;
	case 5:
		return &m_clock;
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

	if (bFallingEdge)
	{
		// 상태를 업데이트시키지만, 출력은 나중에 업데이트된다. 
		// 여기서 변화시킨 상태는 다음 입력 변화 시 출력으로 적용될 것이다. 
		// 그것은 falling edge일 수도 있고, 다른 입력일 수도 있다. 

		if (m_regWrite.ReadAt(0))
		{
			// update edge triggred part here
			uint32_t idx = ReadToUint32(m_wReg, 5);
			assert(idx < 32);
			if (idx != 0)	// zero register에는 값을 쓸 수 없다. 
			{
				int val = ReadToUint32(m_wData, 32);
				m_registers[idx] = val;
			}

			// 입력이 변하지 않더라도 출력을 업데이트하도록
			// 남은 딜레이를 리셋시킨다. 
			resetDelay();
		}
	}
}
