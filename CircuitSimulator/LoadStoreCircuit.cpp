#include "stdafx.h"
#include "LoadStoreCircuit.h"

LoadStoreCircuit::LoadStoreCircuit()
	: Circuit(
		"Load/Store Unit", ECircuitType::LoadStoreUnit, 
		3, 1, &m_outBuf1, &m_outBuf2, 1, 0.1f )
	, m_memWbRegWrite(*this, "MEM/WB.regW", 1)
	, m_exMemWriteReg(*this, "EX/MEM.wReg", 5)
	, m_memWbWriteReg(*this, "MEM/WB.wReg", 5)
	, m_select(*this, "select", 0, 1)
{}

LoadStoreCircuit::LoadStoreCircuit(float x, float y)
	: LoadStoreCircuit()
{
	SetPos(x, y);
}

void LoadStoreCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_memWbRegWrite.Render();
	ImGui::SameLine();
	m_select.Render();

	m_exMemWriteReg.Render();
	m_memWbWriteReg.Render();

	ImNode::EndNode();
}

void LoadStoreCircuit::RenderInspector()
{
	if (m_memWbRegWrite.GetFrom() != nullptr)
	{
		if (m_memWbRegWrite.Value() == 1)
		{
			ImGui::Text("EX/MEM RegisterWrite: true");
		}
		else
		{
			ImGui::Text("EX/MEM RegisterWrite: false");
		}
	}
	else
	{
		ImGui::Text("EX/MEM RegisterWrite: false");
	}

	
	if (m_exMemWriteReg.GetFrom() != nullptr)
	{
		ImGui::Text("EX/MEM WriteRegister: %d", m_exMemWriteReg.Value());
	}
	else
	{
		ImGui::Text("EX/MEM WriteRegister: 0");
	}
	
	if (m_memWbWriteReg.GetFrom() != nullptr)
	{
		ImGui::Text("MEM/WB WriteRegister: %d", m_memWbWriteReg.Value());
	}
	else
	{
		ImGui::Text("MEM/WB WriteRegister: 0");
	}
}

InputPin* LoadStoreCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_memWbRegWrite;
	case 1:
		return &m_exMemWriteReg;
	case 2:
		return &m_memWbWriteReg;
	}
	return nullptr;
}

OutputPin* LoadStoreCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_select;
	}
	return nullptr;
}

void LoadStoreCircuit::updateOutput()
{
	if (
		m_memWbRegWrite.Value() == 1 && 
		m_exMemWriteReg.Value() == m_memWbWriteReg.Value()
	)
	{
		setOutputDataByValue(0, 1);
	}
	else
	{
		setOutputDataByValue(0, 0);
	}
}


