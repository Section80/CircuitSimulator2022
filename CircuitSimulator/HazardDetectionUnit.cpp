#include "HazardDetectionUnit.h"

HazardDetectionUnit::HazardDetectionUnit()
	: Circuit(
		"HazardDetectionUnit", ECircuitType::HarzardDetection,
		6, 4, m_outBuf1, m_outBuf2,
		4, 0.1f)
	, m_IFID_rs(*this, "IF/ID.rs", 5)
	, m_IFID_rt(*this, "IF/ID.rt", 5)
	, m_IDEX_memRead(*this, "ID/EX.memRead", 1)
	, m_IDEX_writeReg(*this, "ID/EX.writeReg", 5)
	, m_EXMEM_memWrite(*this, "EX/MEM.memWrite", 1)
	, m_EXMEM_writeReg(*this, "EX/MEM.writeReg", 5)
	, m_pcWrite(*this, "PC.write", 0, 1)
	, m_ifIdWrite(*this, "IF/ID.write", 1, 1)
	, m_selectCtrl(*this, "selectCtrl", 2, 1)
	, m_selectRead1(*this, "selectRead1", 3, 1)
	, m_bStall(false)
	, m_bSaveUsed2(false)
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
				m_IFID_rs.Render();
				m_IFID_rt.Render();
				m_IDEX_memRead.Render();
				m_IDEX_writeReg.Render();
				m_EXMEM_memWrite.Render();
				m_EXMEM_writeReg.Render();
			ImGui::EndVertical();
			ImGui::BeginVertical("Output");
				m_pcWrite.Render();
				m_ifIdWrite.Render();
				m_selectCtrl.Render();
				m_selectRead1.Render();
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

	if (m_bSaveUsed2)
	{
		ImGui::Text("SaveUsed2: true");
	}
	else
	{
		ImGui::Text("SaveUsed2: false");
	}
}

InputPin* HazardDetectionUnit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_IFID_rs;
	case 1:
		return &m_IFID_rt;
	case 2:
		return &m_IDEX_memRead;
	case 3: 
		return &m_IDEX_writeReg;
	case 4:
		return &m_EXMEM_memWrite;
	case 5:
		return &m_EXMEM_writeReg;
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
		return &m_selectCtrl;
	case 3:
		return &m_selectRead1;
	default:
		assert(false);
	}

	return nullptr;
}

void HazardDetectionUnit::updateOutput()
{
	// Check Load Use(Save Used1)
	if (
		m_IDEX_memRead.ReadAt(0) == 1 &&
		(
			m_IFID_rs.Value() == m_IDEX_writeReg.Value() ||
			m_IFID_rt.Value() == m_IDEX_writeReg.Value()
		)
	)
	{
		m_bStall = true;

		// pc_write = 0;
		setOutputDataByValue(0, 0);
		// IFID_write = 0;
		setOutputDataByValue(1, 0);
		// Use All 0 instead of Control Unit output
		setOutputDataByValue(2, 1);
	}
	else
	{
		m_bStall = false;

		// pc_write = 1;
		setOutputDataByValue(0, 1);
		// IFID_write = 1;
		setOutputDataByValue(1, 1);
		// Use Control Unit output
		setOutputDataByValue(2, 0);
	}

	// Check Save Used2
	if (
		m_EXMEM_memWrite.ReadAt(0) == true &&
		m_IFID_rt.Value() == m_EXMEM_writeReg.Value()
		)
	{
		m_bSaveUsed2 = true;
		setOutputDataByValue(3, 1);
	}
	else
	{
		m_bSaveUsed2 = false;
		setOutputDataByValue(3, 0);
	}
}
