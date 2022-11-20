#include "stdafx.h"
#include "Convert.h"
#include "AluCircuit.h"

AluCircuit::AluCircuit()
	: Circuit("ALU", ECircuitType::ALU, 3, 2, m_outBuf1, m_outBuf2, 33, 0.5f)
	, m_in1(*this, "in1", 32)
	, m_in2(*this, "in2", 32)
	, m_operation(*this, "op", 4)
	, m_out(*this, "res", 0, 32)
	, m_zero(*this, "zero", 32, 1)
	, m_lastOp(EAluOperation::And)
	, m_lastResult(0)
{}

AluCircuit::AluCircuit(float x, float y)
	: AluCircuit()
{
	SetPos(x, y);
}

void AluCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(140.0f); 

	m_in1.Render();
	ImGui::SameLine();
	m_zero.Render();

	m_in2.Render();
	ImGui::SameLine();
	m_out.Render();

	m_operation.Render();

	ImNode::EndNode();
}

void AluCircuit::RenderInspector()
{
	switch (m_lastOp)
	{
	case EAluOperation::And:
		ImGui::Text("op: And");
		break;
	case EAluOperation::Or:
		ImGui::Text("op: Or");
		break;
	case EAluOperation::Add:
		ImGui::Text("op: Add");
		break;
	case EAluOperation::Sub:
		ImGui::Text("op: Sub");
		break;
	case EAluOperation::Slt:
		ImGui::Text("op: Slt");
		break;
	case EAluOperation::Nor:
		ImGui::Text("op: Nor");
		break;
	default:
		ImGui::Text("op: None");
		break;
	}

	ImGui::Text("result: %d", m_lastResult);
}

InputPin* AluCircuit::GetInputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_in1;
	case 1:
		return &m_in2;
	case 2:
		return &m_operation;
	default:
		assert(false);
	}

	return nullptr;
}

OutputPin* AluCircuit::GetOutputPin(int index)
{
	switch (index)
	{
	case 0:
		return &m_out;
	case 1:
		return &m_zero;
	default:
		assert(false);
	}

	return nullptr;
}

void AluCircuit::updateOutput()
{
	uint32_t in1 = ReadToUint32(m_in1, 32);
	uint32_t in2 = ReadToUint32(m_in2, 32);
	m_lastOp = (EAluOperation)ReadToUint32(m_operation, 4);

	switch (m_lastOp)
	{
	case EAluOperation::And:
		m_lastResult = in1 & in2;
		break;
	case EAluOperation::Or:
		m_lastResult = in1 | in2;
		break;
	case EAluOperation::Add:
		m_lastResult = in1 + in2;
		break;
	case EAluOperation::Sub:
		m_lastResult = in1 - in2;
		break;
	case EAluOperation::Slt:
		(in1 < in2) ? m_lastResult = 1 : m_lastResult = 0;
		break;
	case EAluOperation::Nor:
		m_lastResult = ~(in1 | in2);
		break;
	default:
		break;
	}

	if (m_lastResult > 100000000)
	{
		int a = 0;
		a++;
	}

	bool* outBuf = getOutputDataBuffer(0);
	Uint32ToBoolArray(m_lastResult, outBuf);

	bool bVal = false;
	if (m_lastResult == 0)
	{
		bVal = true;
		setOutputData(1, &bVal);
	}
	else
	{
		setOutputData(1, &bVal);
	}
}

AluOperationCircuit::AluOperationCircuit()
	: Circuit("ALU OP", ECircuitType::ALUOperation, 0, 1, m_outBuf1, m_outBuf2, 4, 0.0f)
	, m_out(*this, "out", 0, 4)
	, m_op(EAluOperation::Or)
	, m_buttonId(Identifiable::GetNewId())
{}

AluOperationCircuit::AluOperationCircuit(float x, float y)
	: AluOperationCircuit()
{
	SetPos(x, y);
}

void AluOperationCircuit::Render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	ImGui::PushID(m_buttonId);
	switch (m_op)
	{
	case EAluOperation::And:
		if (ImGui::Button("And"))
		{
			m_op = EAluOperation::Or;
			bool buffer[4] = { 1, 0, 0, 0 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	case EAluOperation::Or:
		if (ImGui::Button("Or"))
		{
			m_op = EAluOperation::Add;
			bool buffer[4] = { 0, 1, 0, 0 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	case EAluOperation::Add:
		if (ImGui::Button("Add"))
		{
			m_op = EAluOperation::Sub;
			bool buffer[4] = { 0, 1, 1, 0 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	case EAluOperation::Sub:
		if (ImGui::Button("Sub"))
		{
			m_op = EAluOperation::Slt;
			bool buffer[4] = { 1, 1, 1, 0 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	case EAluOperation::Slt:
		if (ImGui::Button("Slt"))
		{
			m_op = EAluOperation::Nor;
			bool buffer[4] = { 0, 0, 1, 1 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	case EAluOperation::Nor:
		if (ImGui::Button("Nor"))
		{
			m_op = EAluOperation::And;
			bool buffer[4] = { 0, 0, 0, 0 };
			swapCircuitOutput();
			setOutputData(0, buffer);
			afterUpdateOutput();
		}
		break;
	default:
		break;
	}
	ImGui::PopID();
	m_out.Render();

	ImNode::EndNode();
}

InputPin* AluOperationCircuit::GetInputPin(int index)
{
	assert(0);
	return nullptr;
}

OutputPin* AluOperationCircuit::GetOutputPin(int index)
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
