#include "stdafx.h"
#include "Convert.h"
#include "ClockCircuit.h"

ClockCircuit::ClockCircuit()
	: Circuit("Clock", 0, 1, &m_outBuf1, &m_outBuf2, 1, 3.0f)
	, m_out(*this, "Clock", 0, 1)
	, m_bVal(false)
{
	resetDelay();
}

ClockCircuit::ClockCircuit(float x, float y)
	: ClockCircuit()
{
	SetPos(x, y);
}

void ClockCircuit::render()
{
	ImNode::BeginNode(GetId());
	ImGui::Text(GetName());

	renderDelay(70.0f);

	if (m_bVal == false)
	{
		ImGui::Text("0");
	}
	else 
	{
		ImGui::Text("1");
	}

	m_out.Render();

	ImNode::EndNode();
}

InputPin* ClockCircuit::GetInputPin(int index)
{
	assert(false);
	return nullptr;
}

OutputPin* ClockCircuit::GetOutputPin(int index)
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

void ClockCircuit::afterUpdateOutput()
{
	// clock signal이 true일 때만 출력을 전파한다. 
	// 왜? falling edge에 rising edge triggered 회로의 delay를 초기화하지 않기 위함이다. 
	// 대신 falling edge triggered 회로는 falling edge를 전파받을 수 없다. 
	// double date rate나 falling edge triggred는 구현할 생각이 없으니 당장 해결할 필요는 없다. 
	// 그래도 문제를 해결하려면, 신호를 보내는 쪽(clock)이 아닌 받는 쪽에서 onInputChanged()에서
	// 딜레이 초기화 여부를 판단하게 할 수도 있다. 
	// onInputChanged()를 override하는 클래스(EdgeTriggeredCircuit)를 만들면 좋을 것 같다. 
	// 해봤는데 생각보다 어렵다. 오래 걸릴 것 같아서 패스.. 
	if (m_bVal == true)
	{
		Circuit::afterUpdateOutput();
	}
}

void ClockCircuit::updateOutput()
{
	if (m_bVal)
	{
		m_bVal = false;
	}
	else 
	{
		m_bVal = true;
	}

	setOutputData(0, &m_bVal);

	resetDelay();	// 무한 반복
}


