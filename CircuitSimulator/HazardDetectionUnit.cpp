#include "HazardDetectionUnit.h"

HazardDetectionUnit::HazardDetectionUnit()
	: Circuit(
		"HazardDetectionUnit", ECircuitType::HarzardDetection,
		11, 3, m_outBuf1, m_outBuf2,
		3, 0.1f)
	, m_branch(*this, "branch", 1)
	, m_jump(*this, "jump", 1)
	, m_memWrite(*this, "memWrite", 1)
	, m_IFID_rs(*this, "IF/ID.rs", 5)
	, m_IFID_rt(*this, "IF/ID.rt", 5)
	, m_IDEX_memRead(*this, "ID/EX.memRead", 1)
	, m_IDEX_regWrite(*this, "ID/EX.regWrite", 1)
	, m_IDEX_writeReg(*this, "ID/EX.writeReg", 5)
	, m_EXMEM_memRead(*this, "EX/MEM.memRead", 1)
	, m_EXMEM_regWrite(*this, "EX/MEM.regWrite", 1)
	, m_EXMEM_writeReg(*this, "EX/MEM.writeReg", 5)
	, m_stall(*this, "stall", 0, 1)
	, m_forwardA(*this, "forwardA", 1, 1)
	, m_forwardB(*this, "forwardB", 2, 1)
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
				m_branch.Render();
				m_jump.Render();
				m_memWrite.Render();

				m_IFID_rs.Render();
				m_IFID_rt.Render();

				m_IDEX_memRead.Render();
				m_IDEX_regWrite.Render();
				m_IDEX_writeReg.Render();
				
				m_EXMEM_memRead.Render();
				m_EXMEM_regWrite.Render();
				m_EXMEM_writeReg.Render();
			ImGui::EndVertical();
			ImGui::BeginVertical("Output");
				m_stall.Render();
				m_forwardA.Render();
				m_forwardB.Render();
			ImGui::EndVertical();
		ImGui::EndHorizontal();
	ImNode::EndNode();
}

void HazardDetectionUnit::RenderInspector()
{
	if (m_stall.Value() == 1)
	{
		ImGui::Text("Stall: true");
	}
	else
	{
		ImGui::Text("Stall: false");
	}

	if (m_forwardA.Value() == 1)
	{
		ImGui::Text("ForwardA: true");
	}
	else
	{
		ImGui::Text("ForwardA: false");
	}

	if (m_forwardB.Value() == 1)
	{
		ImGui::Text("ForwardB: true");
	}
	else
	{
		ImGui::Text("ForwardB: false");
	}
}

InputPin* HazardDetectionUnit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_branch;
	case 1:
		return &m_jump;
	case 2: 
		return &m_memWrite;
	case 3:
		return &m_IFID_rs;
	case 4:
		return &m_IFID_rt;
	case 5:
		return &m_IDEX_memRead;
	case 6:
		return &m_IDEX_regWrite;
	case 7:
		return &m_IDEX_writeReg;
	case 8:
		return &m_EXMEM_memRead;
	case 9:
		return &m_EXMEM_regWrite;
	case 10:
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
		return &m_stall;
	case 1:
		return &m_forwardA;
	case 2:
		return &m_forwardB;
	default:
		assert(false);
	}

	return nullptr;
}

void HazardDetectionUnit::updateOutput()
{
	bool bStall = false;

	// Check Load Use
	if (
		m_memWrite.Value() == 0 &&
		m_IDEX_memRead.Value() == 1 &&
		(
			m_IFID_rs.Value() == m_IDEX_writeReg.Value() ||
			m_IFID_rt.Value() == m_IDEX_writeReg.Value()
		)
	)
	{
		int memWrite = m_memWrite.Value();
		int rs = m_IFID_rs.Value();
		int rt = m_IFID_rt.Value();
		int writeReg = m_IDEX_writeReg.Value();

		bStall = true;
	}
	else if (    // Branch Hazard(alu)
		(
			m_branch.Value() == 1 &&
			m_jump.Value() == 0
		) && 
		m_IDEX_regWrite.Value() == 1 &&
		(
			m_IFID_rs.Value() == m_IDEX_writeReg.Value() ||
			m_IFID_rt.Value() == m_IDEX_writeReg.Value()
		)
	)
	{
		bStall = true;
	}
	else if (    // Branch Hazard(mem)
		(
			m_branch.Value() == 1 &&
			m_jump.Value() == 0
		) && 
		(
			(
				m_IDEX_memRead.Value() == 1 &&
				(
					m_IDEX_writeReg.Value() == m_IFID_rs.Value() ||
					m_IDEX_writeReg.Value() == m_IFID_rt.Value()
				)
			) || 
			(
				m_EXMEM_memRead.Value() == 1 &&
				(
					m_EXMEM_writeReg.Value() == m_IFID_rs.Value() ||
					m_EXMEM_writeReg.Value() == m_IFID_rt.Value()
				)
			)
		)
	)
	{
		bStall = true;
	}

	if (bStall)
	{
		setOutputDataByValue(m_stall, 1);
	}
	else
	{
		setOutputDataByValue(m_stall, 0);
	}

	// printf("b: %d \n", m_branch.Value());
	// printf("j: %d \n", m_jump.Value());
	// printf("IDEX.regWrite: %d \n", m_IDEX_regWrite.Value());
	// printf("IFID.rs: %d \n", m_IFID_rs.Value());
	// printf("IFID.rt: %d \n", m_IFID_rt.Value());

	// ForwardA
	// Check Save Used2
	if (
		m_EXMEM_regWrite.ReadAt(0) == true &&
		m_IFID_rs.Value() == m_EXMEM_writeReg.Value()
		)
	{
		setOutputDataByValue(m_forwardA, 1);
	}
	else
	{
		setOutputDataByValue(m_forwardA, 0);
	}

	// ForwardB
	if (
		m_EXMEM_regWrite.ReadAt(0) == true &&
		m_IFID_rt.Value() == m_EXMEM_writeReg.Value()
		)
	{
		setOutputDataByValue(m_forwardB, 1);
	}
	else
	{
		setOutputDataByValue(m_forwardB, 0);
	}
}
