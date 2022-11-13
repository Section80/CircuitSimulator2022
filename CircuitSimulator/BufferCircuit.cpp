#include "stdafx.h"
#include "BufferCircuit.h"

BufferCircuit::BufferCircuit()
	: Circuit("Buffer", ECircuitType::Buffer, 1, 1, m_outBuf1, m_outBuf2, 32, 0.0f)
	, m_inputPin(*this, "in", 1)
	, m_outputPin(*this, "out", 0, 1)
	, m_reverse(false)
	, m_wireLineCount(1)
	, m_inputIntId(Identifiable::GetNewId())
{}

BufferCircuit::BufferCircuit(float x, float y)
	: BufferCircuit()
{
	SetPos(x, y);
}

void BufferCircuit::render()
{
	ImNode::BeginNode(GetNodeId());
		ImGui::Text(GetName());
		if (m_reverse)
		{
			m_outputPin.Render(true);
			ImGui::SameLine();
			m_inputPin.Render(true);
		}
		else
		{
			m_inputPin.Render(false);
			ImGui::SameLine();
			m_outputPin.Render(false);
		}

		if (ImGui::Button("Swap"))
		{
			m_reverse = !m_reverse;
		}
		ImGui::SameLine();

		int oldCount = m_wireLineCount;
		ImGui::PushID(m_inputIntId);
			ImGui::PushItemWidth(40.0f);
			bool bRet = ImGui::InputInt(
				"count",
				&m_wireLineCount, 0, 0,
				ImGuiInputTextFlags_EnterReturnsTrue
			);
			ImGui::PopItemWidth();
		ImGui::PopID();
		if (bRet)
		{
			if (m_wireLineCount < 1) m_wireLineCount = 1;
			if (m_wireLineCount > 32) m_wireLineCount = 32;

			if (m_wireLineCount != oldCount)
			{
				if (m_inputPin.GetFrom() != nullptr)
				{
					m_inputPin.GetFrom()->Disconnect(&m_inputPin);
				}
				m_inputPin.setWireLineCount(m_wireLineCount);

				for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
				{
					Wire& w = m_outputPin.m_wires[i];
					if (w.GetTo() != nullptr)
					{
						m_outputPin.Disconnect(w.GetTo());
					}
				}
				m_outputPin.setWireLineCount(m_wireLineCount);
			}
		}

	ImNode::EndNode();
}

InputPin* BufferCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_inputPin;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* BufferCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_outputPin;
	default:
		assert(false);
	}

	return nullptr;
}

void BufferCircuit::updateOutput()
{
	bool buf[32] = { 0 };
	for (int i = 0; i < m_wireLineCount; i ++)
	{
		buf[i] = m_inputPin.ReadAt(i);
	}
	setOutputData(0, buf);
}

void BufferCircuit::SetWireLineCount(int count)
{
	if (count < 1) count = 1;
	else if (count > 32) count = 32;
	m_wireLineCount = count;
}
