#include "stdafx.h"
#include "Convert.h"
#include "IfIdRegisterCircuit.h"
#include "IdExRegisterCircuit.h"
#include "ExMemRegisterCircuit.h"
#include "MemWbRegisterCircuit.h"
#include "InstructionMemory.h"

MemWbRegisterCircuit* MemWbRegisterCircuit::Instance = nullptr;

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
	, CurrentInstruction("nop")
{
	memset(m_data, 0, sizeof(uint32_t) * 2);
	MemWbRegisterCircuit::Instance = this;
}

MemWbRegisterCircuit::MemWbRegisterCircuit(float x, float y)
	: MemWbRegisterCircuit()
{
	SetPos(x, y);
}

void MemWbRegisterCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(180.0f);
	
	renderIOGroup("======== WB ========", 0, 2);
	renderIOGroup("======== MEM/WB ========", 2, 3);
	m_clock.Render();

	ImNode::EndNode();
}

void MemWbRegisterCircuit::RenderWire(bool bSummary)
{
	if (bSummary)
	{
		m_readData_out.RenderWire();
		m_aluResult_out.RenderWire();
		m_writeReg_out.RenderWire();
	}
	else 
	{
		Circuit::RenderWire();
	}


}

void MemWbRegisterCircuit::RenderInspector()
{
	ImGui::Text("Instruction: %s", CurrentInstruction.c_str());
	ImGui::Text("  regWrite: %d", m_regWrite_out.Value());
	ImGui::Text("  memToReg: %d", m_memToReg_out.Value());

	int readDataOut = m_readData_out.Value();
	if (readDataOut == 0)
	{
		ImGui::Text("  readData: 0x00000000");
	}
	else
	{
		ImGui::Text("  readData: %0#10x", m_readData_out.Value());
	}
	
	int aluResultOut = m_aluResult_out.Value();
	if (aluResultOut == 0)
	{
		ImGui::Text(" aluResult: 0x00000000");
	}
	else
	{
		ImGui::Text(" aluResult: %0#10x", aluResultOut);
	}

	ImGui::Text("  writeReg: %d", m_writeReg_out.Value());

	InstructionMemoryCircuit* im = InstructionMemoryCircuit::Instance;
	int address = im->GetAddress();
	std::string& str = im->GetInstructionString(address);

	ImGui::NewLine();
	ImGui::Text(" IF: %s", str.c_str());
	ImGui::Text(" ID: %s", IfIdRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text(" EX: %s", IdExRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text("MEM: %s", ExMemRegisterCircuit::Instance->CurrentInstruction.c_str());
	ImGui::Text(" WB: %s", MemWbRegisterCircuit::Instance->CurrentInstruction.c_str());
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
