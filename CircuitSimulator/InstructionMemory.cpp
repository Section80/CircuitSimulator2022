#include "stdafx.h"
#include <nfd.h>
#include "Convert.h"
#include "Instruction.h"
#include "InstructionMemory.h"
#include "PlayButton.h"
#include "Environment.h"
#include "Parse.h"


InstructionMemoryCircuit::InstructionMemoryCircuit()
	: Circuit("Instruction Memory", ECircuitType::InstructionMemory, 1, 1, m_outBuf1, m_outBuf2, 32, 0.5f)
	, m_addr(*this, "addr", 32)
	, m_out(*this, "out", 0, 32)
	, m_val(0)
	, m_currentOutAddr(0)
	, m_loadButtonId(Identifiable::GetNewId())
	, m_path("NULL")
{
}

InstructionMemoryCircuit::InstructionMemoryCircuit(float x, float y)
	: InstructionMemoryCircuit()
{
	SetPos(x, y);
}

InstructionMemoryCircuit::~InstructionMemoryCircuit()
{
}

void InstructionMemoryCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());
	renderDelay(160.0f);

	// char buffer[16];
	// _itoa_s(m_val, buffer, 10);
	// ImGui::Text(buffer);

	ImGui::BeginHorizontal("IO");

		ImGui::BeginVertical("Input");
		m_addr.Render();
		ImGui::EndVertical();

		ImGui::BeginVertical("output");
		m_out.Render();
		ImGui::EndVertical();

	ImGui::EndHorizontal();

	ImGui::PushID(m_loadButtonId);
	if (ImGui::Button("Load"))
	{
		PlayButton::Instance->Pause();

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("asm;", env::pwd.c_str(), &outPath);

		if (result == NFD_OKAY) {
			PlayButton::Instance->Pause();
			puts(outPath);
			LoadInstructions(outPath);
			
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

void InstructionMemoryCircuit::RenderInspector()
{
	for (const auto& pair : m_map)
	{
		if (pair.first == m_currentOutAddr)
		{
			// yellow
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[%0#10x] %0#10x", pair.first, pair.second);
		}
		else
		{
			ImGui::Text("[%0#10x] %0#10x", pair.first, pair.second);
		}
	}
}

InputPin* InstructionMemoryCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_addr;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* InstructionMemoryCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_out;
	default:
		assert(false);
	}

	return nullptr;
}

void InstructionMemoryCircuit::LoadInstructions(std::string path)
{
	bool bRes = ::LoadInstructions(path.c_str(), &m_map);
	if (bRes)
	{
		m_path.assign(path);
	}
	else
	{
		m_path.assign("NULL");
	}
}

void InstructionMemoryCircuit::updateOutput()
{
	m_currentOutAddr = ReadToUint32(m_addr, 32);
	m_currentOutAddr = m_currentOutAddr;
	
	m_val = 0;
	if (m_map.count(m_currentOutAddr))
	{
		m_val = m_map[m_currentOutAddr];
	}
	

	// #1
	// 왜 0번이 아니라 3번을 가져올까? 
	// 0번은 offset이 21이다. 
	// 21부터 32비트를 쓰면 뒤에 있는 m_map을 침범한다. 
	// 그래서 실제로 detor에서 에러가 발생했다...
	// 오버런 조심할 것. 

	// #2
	// output에 m_op를 추가하면서 outputPin의 인덱스가 1씩 밀려났다. 
	// 그래서 더이상 3번 output은 offset이 0이 아니다. 
	// 기존의 3번을 쓰려면 4를 쓰거나,
	// 마지막에 추가된 5를 써도 된다. 5도 offset이 0이다. 

	// # 출력을 32bit 하나로 바꿨다. 
	bool* buf = getOutputDataBuffer(0);
	Uint32ToBoolArray(m_val, buf);
}
