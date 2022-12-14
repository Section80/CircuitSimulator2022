#include "stdafx.h"
#include "ForwardCircuit.h"
#include "Convert.h"

ForwardCircuit::ForwardCircuit()
	: Circuit("Forward Unit", ECircuitType::ForwardUnit, 6, 2, m_outBuf1, m_outBuf2, 4, 0.1f)
	, m_rs(*this, "rs", 5)
	, m_rt(*this, "rt", 5)
	, m_exMemAluReg(*this, "EX/MEM Reg", 5)
	, m_exMemRegWrite(*this, "EX/MEM RegW", 1)
	, m_memWbAluReg(*this, "MEM/WB Reg", 5)
	, m_memWbRegWrite(*this, "MEM/WB RegW", 1)
	, m_forwardA(*this, "A", 0, 2)
	, m_forwardB(*this, "B", 2, 2)
	, m_iForwardA(0)
	, m_iForwardB(0)
{}

ForwardCircuit::ForwardCircuit(float x, float y)
	: ForwardCircuit()
{
	SetPos(x, y);
}

void ForwardCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_rs.Render();
	ImGui::SameLine();
	m_forwardA.Render();

	m_rt.Render();
	ImGui::SameLine();
	m_forwardB.Render();
	
	m_exMemAluReg.Render();
	m_exMemRegWrite.Render();
	m_memWbAluReg.Render();
	m_memWbRegWrite.Render();

	ImNode::EndNode();
}

void ForwardCircuit::RenderInspector()
{
	if (m_iForwardA == 0b00)
	{
		ImGui::Text("ForwardA: 00");
	}
	else if (m_iForwardA == 0b01)
	{
		ImGui::Text("ForwardA: 01");
	}
	else if (m_iForwardA == 0b10)
	{
		ImGui::Text("ForwardA: 10");
	}

	if (m_iForwardB == 0b00)
	{
		ImGui::Text("ForwardB: 00");
	}
	else if (m_iForwardB == 0b01)
	{
		ImGui::Text("ForwardB: 01");
	}
	else if (m_iForwardB == 0b10)
	{
		ImGui::Text("ForwardB: 10");
	}
}

InputPin* ForwardCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_rs;
	case 1:
		return &m_rt;
	case 2:
		return &m_exMemAluReg;
	case 3:
		return &m_exMemRegWrite;
	case 4:
		return &m_memWbAluReg;
	case 5:
		return &m_memWbRegWrite;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* ForwardCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_forwardA;
	case 1:
		return &m_forwardB;
	}
	return nullptr;
}

void ForwardCircuit::updateOutput()
{
	// printf("rs: %d \n", m_rs.Value());
	// printf("rt: %d \n", m_rt.Value());
	// printf("ex/mem: %d \n", m_exMemAluReg.Value());
	// printf("mem/wb: %d \n", m_memWbAluReg.Value());

	// update ForwardA
	if (
		m_exMemRegWrite.Value() == 1 &&
		m_exMemAluReg.Value() != 0 &&
		m_exMemAluReg.Value() == m_rs.Value()
	)	// EX/MEM에서 가져와야 하는 경우
	{
		setOutputDataByValue(0, 0b10);
		m_iForwardA = 0b10;
	}
	else if (
		m_memWbRegWrite.Value() == 1 &&
		m_memWbAluReg.Value() != 0 &&
		m_memWbAluReg.Value() == m_rs.Value()
	)	// MEM/WB에서 가져와야 하는 경우
	{
		setOutputDataByValue(0, 0b01);
		m_iForwardA = 0b01;
	}
	else   // 레지스터에 읽은 값 그대로 사용
	{
		setOutputDataByValue(0, 0b00);
		m_iForwardA = 0b00;
	}

	// update ForwardB
	if (
		m_exMemRegWrite.Value() == 1 &&
		m_exMemAluReg.Value() != 0 &&
		m_exMemAluReg.Value() == m_rt.Value()
	)	// EX/MEM에서 가져와야 하는 경우
	{
		setOutputDataByValue(1, 0b10);
		m_iForwardB = 0b10;
	}
	else if (
		m_memWbRegWrite.Value() == 1 &&
		m_memWbAluReg.Value() != 0 &&
		m_memWbAluReg.Value() == m_rt.Value()
	)	// MEM/WB에서 가져와야 하는 경우
	{
		setOutputDataByValue(1, 0b01);
		m_iForwardB = 0b01;
	}
	else   // 레지스터에 읽은 값 그대로 사용
	{
		setOutputDataByValue(1, 0b00);
		m_iForwardB = 0b00;
	}
}
