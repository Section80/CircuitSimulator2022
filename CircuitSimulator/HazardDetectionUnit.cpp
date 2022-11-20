#include "HazardDetectionUnit.h"

HazardDetectionUnit::HazardDetectionUnit()
	: Circuit(
		"HazardDetectionUnit", ECircuitType::HarzardDetection,
		4, 3, m_outBuf1, m_outBuf2, 
		3, 0.1f)
	, m_ifIdRs(*this, "IF/ID.rs", 5)
	, m_ifIdRt(*this, "IF/ID.rt", 5)
	, m_idExMemRead(*this, "ID/EX.memRead", 1)
	, m_idExWriteReg(*this, "ID/EX.write", 5)
	, m_pcWrite(*this, "PC.write", 0, 1)
	, m_ifIdWrite(*this, "IF/ID.write", 1, 1)
	, m_select(*this, "select", 2, 1)
	, m_bStall(false)
{}

HazardDetectionUnit::HazardDetectionUnit(float x, float y)
	: HazardDetectionUnit()
{
	SetPos(x, y);
}

void HazardDetectionUnit::Render()
{
	ImNode::BeginNode(GetId());
		ImGui::Text(GetName());
		ImGui::BeginHorizontal("IO");
			ImGui::BeginVertical("Input");
				m_ifIdRs.Render();
				m_ifIdRt.Render();
				m_idExMemRead.Render();
				m_idExWriteReg.Render();
			ImGui::EndVertical();
			ImGui::BeginVertical("Output");
				m_pcWrite.Render();
				m_ifIdWrite.Render();
				m_select.Render();
			ImGui::EndVertical();
		ImGui::EndHorizontal();
	ImNode::EndNode();
}

void HazardDetectionUnit::RenderInspector()
{
	if (m_bStall)
	{
		ImGui::Text("Stall: true");
	}
	else
	{
		ImGui::Text("Stall: false");
	}
}

InputPin* HazardDetectionUnit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_ifIdRs;
	case 1:
		return &m_ifIdRt;
	case 2:
		return &m_idExMemRead;
	case 3: 
		return &m_idExWriteReg;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* HazardDetectionUnit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_pcWrite;
	case 1:
		return &m_ifIdWrite;
	case 2:
		return &m_select;
	default:
		assert(false);
	}

	return nullptr;
}

void HazardDetectionUnit::updateOutput()
{
	if (
		m_idExMemRead.ReadAt(0) == 1 &&
		(
			m_ifIdRs.Value() == m_idExWriteReg.Value() ||
			m_ifIdRt.Value() == m_idExWriteReg.Value()
		)
	)
	{
		// pc_write = 0;
		setOutputDataByValue(0, 0);
		// IFID_write = 0;
		setOutputDataByValue(1, 0);
		// Use All 0 instead of Control Unit output
		setOutputDataByValue(1, 1);

		return;
	}

	// pc_write = 1;
	setOutputDataByValue(0, 1);
	// IFID_write = 1;
	setOutputDataByValue(1, 1);
	// Use Control Unit output
	setOutputDataByValue(1, 0);
}
