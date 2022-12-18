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

	// https://ecampus.kookmin.ac.kr/mod/assign/view.php?id=1261943
	
	// $gp = 0x10008000
	m_registers[28] = 0x10008000;
	// $sp = 0x7ffffe40
	m_registers[29] = 0x7ffffe40;
	
}

RegisterCircuit::RegisterCircuit(float x, float y)
	: RegisterCircuit()
{
	SetPos(x,y);
}

void RegisterCircuit::Render()
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
	ImGui::Text("R0  [zr] = %0#10x", m_registers[0]);
	ImGui::Text("R1  [at] = %0#10x", m_registers[1]);
	ImGui::Text("R2  [v0] = %0#10x", m_registers[2]);
	ImGui::Text("R3  [v1] = %0#10x", m_registers[3]);
	ImGui::Text("R4  [a0] = %0#10x", m_registers[4]);
	ImGui::Text("R5  [a1] = %0#10x", m_registers[5]);
	ImGui::Text("R6  [a2] = %0#10x", m_registers[6]);
	ImGui::Text("R7  [a3] = %0#10x", m_registers[7]);
	ImGui::Text("R8  [t0] = %0#10x", m_registers[8]);
	ImGui::Text("R9  [t1] = %0#10x", m_registers[9]);
	ImGui::Text("R10 [t2] = %0#10x", m_registers[10]);
	ImGui::Text("R11 [t3] = %0#10x", m_registers[11]);
	ImGui::Text("R12 [t4] = %0#10x", m_registers[12]);
	ImGui::Text("R13 [t5] = %0#10x", m_registers[13]);
	ImGui::Text("R14 [t6] = %0#10x", m_registers[14]);
	ImGui::Text("R15 [t7] = %0#10x", m_registers[15]);
	ImGui::Text("R16 [s0] = %0#10x", m_registers[16]);
	ImGui::Text("R17 [s1] = %0#10x", m_registers[17]);
	ImGui::Text("R18 [s2] = %0#10x", m_registers[18]);
	ImGui::Text("R19 [s3] = %0#10x", m_registers[19]);
	ImGui::Text("R20 [s4] = %0#10x", m_registers[20]);
	ImGui::Text("R21 [s5] = %0#10x", m_registers[21]);
	ImGui::Text("R22 [s6] = %0#10x", m_registers[22]);
	ImGui::Text("R23 [s7] = %0#10x", m_registers[23]);
	ImGui::Text("R24 [t8] = %0#10x", m_registers[24]);
	ImGui::Text("R25 [t9] = %0#10x", m_registers[25]);
	ImGui::Text("R26 [k0] = %0#10x", m_registers[26]);
	ImGui::Text("R27 [k1] = %0#10x", m_registers[27]);
	ImGui::Text("R28 [gp] = %0#10x", m_registers[28]);
	ImGui::Text("R29 [sp] = %0#10x", m_registers[29]);
	ImGui::Text("R30 [fp] = %0#10x", m_registers[30]);
	ImGui::Text("R31 [ra] = %0#10x", m_registers[31]);
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
