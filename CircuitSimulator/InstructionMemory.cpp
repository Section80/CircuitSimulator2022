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
	// �� 0���� �ƴ϶� 3���� �����ñ�? 
	// 0���� offset�� 21�̴�. 
	// 21���� 32��Ʈ�� ���� �ڿ� �ִ� m_map�� ħ���Ѵ�. 
	// �׷��� ������ detor���� ������ �߻��ߴ�...
	// ������ ������ ��. 

	// #2
	// output�� m_op�� �߰��ϸ鼭 outputPin�� �ε����� 1�� �з�����. 
	// �׷��� ���̻� 3�� output�� offset�� 0�� �ƴϴ�. 
	// ������ 3���� ������ 4�� ���ų�,
	// �������� �߰��� 5�� �ᵵ �ȴ�. 5�� offset�� 0�̴�. 

	// # ����� 32bit �ϳ��� �ٲ��. 
	bool* buf = getOutputDataBuffer(0);
	Uint32ToBoolArray(m_val, buf);
}
