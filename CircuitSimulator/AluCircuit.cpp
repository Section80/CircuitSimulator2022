#include "stdafx.h"
#include "Convert.h"
#include "AluCircuit.h"

AluCircuit::AluCircuit()
	: Circuit("ALU", 3, 2, m_outBuf1, m_outBuf2, 33, 0.5f)
	, m_in1(*this, "in1", 32)
	, m_in2(*this, "in2", 32)
	, m_operation(*this, "op", 4)
	, m_out(*this, "res", 0, 32)
	, m_zero(*this, "zero", 32, 1)
{}

AluCircuit::AluCircuit(float x, float y)
	: AluCircuit()
{
	SetPos(x, y);
}

void AluCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	m_in1.Render();
	ImGui::SameLine();
	m_out.Render();

	m_in2.Render();
	ImGui::SameLine();
	m_zero.Render();

	m_operation.Render();

	ImNode::EndNode();
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
	uint32_t op = ReadToUint32(m_operation, 4);

	uint32_t res = 0;

	switch (op)
	{
	case 0:		// AND
		printf("AND \n");
		res = in1 & in2;
		break;
	case 1:		// OR
		printf("OR \n");
		res = in1 | in2;
		break;
	case 2:		// ADD
		printf("ADD \n");
		res = in1 + in2;
		break;
	case 6:		// SUB
		printf("SUB \n");
		res = in1 - in2;
		break;
	case 7:		// SET LSS THAN
		printf("SLT \n");
		(in1 < in2) ? res = 1 : res = 0;
		break;
	case 12:	// NOR
		printf("NOR \n");
		res = ~(in1 | in2);
		break;
	default:
		break;
	}

	bool* outBuf = getOutputDataBuffer(0);
	Uint32ToBoolArray(res, outBuf);

	bool bVal = false;
	if (res == 0)
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
	: Circuit("ALU OP", 0, 1, m_outBuf1, m_outBuf2, 4, 0.0f)
	, m_out(*this, "out", 0, 4)
	, m_op(EAluOperation::Or)
	, m_buttonId(Identifiable::GetNewId())
{}

AluOperationCircuit::AluOperationCircuit(float x, float y)
	: AluOperationCircuit()
{
	SetPos(x, y);
}

void AluOperationCircuit::render()
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
