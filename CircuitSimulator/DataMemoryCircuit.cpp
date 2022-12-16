#include "stdafx.h"
#include <nfd.h>
#include "Convert.h"
#include "DataMemoryCircuit.h"
#include "PlayButton.h"
#include "Global.h"
#include "Parse.h"
#include "ClockCircuit.h"
#include "ProgramCounterCircuit.h"

DataMemoryCircuit::DataMemoryCircuit()
	: Circuit("Data Memory", ECircuitType::DataMemory, 5, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_addr(*this, "addr", 32)
	, m_wData(*this, "wData", 32)
	, m_clock(*this, "clock", 1)
	, m_memRead(*this, "memRead",1 )
	, m_memWrite(*this, "memWrite", 1)
	, m_rData(*this, "rData", 0, 32)
	, m_bLastClock(false)
	, m_loadButtonId(Identifiable::GetNewId())
	, m_lastChanged(-1)
	, m_lastRData(0)
	, m_path("NULL")
{}

DataMemoryCircuit::DataMemoryCircuit(float x, float y)
	: DataMemoryCircuit()
{
	SetPos(x, y);
}

void DataMemoryCircuit::Render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
		renderDelay(160.0f);

		m_addr.Render();
		ImGui::SameLine();
		m_rData.Render();

		m_wData.Render();
		m_memRead.Render();
		m_memWrite.Render();
		m_clock.Render();

		ImGui::PushID(m_loadButtonId);
		if (ImGui::Button("Load"))
		{
			PlayButton::Instance->Pause();

			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("asm;", global::pwd.c_str(), &outPath);

			if (result == NFD_OKAY) {
				PlayButton::Instance->Pause();
				puts(outPath);
				this->LoadData(outPath);

				ClockCircuit::Instance->ResetDelay();
				ProgramCounterCircuit::Instance->SetAddress(0x00400024);
				global::updated_time = 0;
				Circuit::UpdateAll(2.9f);
				global::updated_time += 2.9;
				ProgramCounterCircuit::Instance->SetAddress(0x00400024);
				
				free(outPath);
			}
			else if (result == NFD_CANCEL) {
				puts("User pressed cancel.");
			}
			else {
				printf("Error: %s\n", NFD_GetError());
			}
		}
		ImGui::PopID();

	ImNode::EndNode();
}

void DataMemoryCircuit::RenderInspector()
{
	m_dataMemory.RenderInspector();
}

InputPin* DataMemoryCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_addr;
	case 1:
		return &m_wData;
	case 2:
		return &m_memRead;
	case 3:
		return &m_memWrite;
	case 4:
		return &m_clock;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* DataMemoryCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_rData;
	default:
		assert(false);
	}

	return nullptr;
}

void DataMemoryCircuit::LoadData(std::string path)
{
	bool bRet = ::LoadData(path.c_str(), &m_dataMemory);
	if (bRet)
	{
		m_path.assign(path);
	}
	else
	{
		m_path.assign("NULL");
	}
}

void DataMemoryCircuit::updateOutput()
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

	if (m_memRead.ReadAt(0))
	{
		uint32_t addr = ReadToUint32(m_addr, 32);
		uint32_t bits = m_dataMemory.ReadWord(addr);

		setOutputDataByValue(m_rData, bits);
		m_lastRData = bits;
	}
	else 
	{
		setOutputDataByValue(m_rData, m_lastRData);
	}

	if (bFallingEdge)
	{
		if (m_memWrite.ReadAt(0))
		{
			// update edge triggred part here
			uint32_t addr = ReadToUint32(m_addr, 32);

			m_lastChanged = addr;
			int val = ReadToUint32(m_wData, 32);
			m_dataMemory.SetWord(addr, val);
			m_dataMemory.SetLastChangd(addr);

			// 입력이 변하지 않더라도 출력을 업데이트하도록
			// 남은 딜레이를 리셋시킨다. 
			resetDelay();
		}
	}
}
