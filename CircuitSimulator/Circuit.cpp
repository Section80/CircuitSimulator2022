#include "stdafx.h"
#include "Pin.h"
#include "Convert.h"
#include "Circuit.h"

std::vector<Circuit*> Circuit::s_pCircuits;

Circuit::Circuit(
	const char* name, ECircuitType type,
	int inputPinCount, int outputPinCount,
	bool* outputBuffer1, bool* outputBuffer2,
	int outputSize,
	float delay
)
	: Identifiable()
	, m_name(name)
	, m_type(type)
	, m_circuitOutput(outputBuffer1, outputBuffer2, outputSize)
	, m_inputPinCount(inputPinCount)
	, m_outputPinCount(outputPinCount)
	, m_delay(int(delay * 1000000))
	, m_leftDelay(-1)
	, m_isJustResolved(false)
{
	if (delay == 0.0f)
	{
		m_delay = 0;
		m_leftDelay = -1;
	}

	m_nodeId = ImNode::NodeId(GetId());
	s_pCircuits.push_back(this);
}

Circuit::~Circuit()
{
	// remove-erase
	s_pCircuits.erase(
		std::remove(s_pCircuits.begin(), s_pCircuits.end(), this),
		s_pCircuits.end()
	);
}

void Circuit::RenderWire()
{
	for (int i = 0; i < m_outputPinCount; i++)
	{
		OutputPin* op =GetOutputPin(i);
		assert(op != nullptr);
		
		op->RenderWire();
	}
}

void Circuit::RenderInspector()
{
	ImGui::Text("Inspector is not implemented. ");
	if (IsResolvable())
	{
		ImGui::Text("IsResolvable : true");
	}
	else
	{
		ImGui::Text("IsResolvable : false");
	}
}

bool Circuit::GetOutputData(int index)
{
	return m_circuitOutput.ReadAt(index);
}

bool Circuit::IsResolvable()
{
	for (int i = 0; i < m_inputPinCount; i++)
	{
		InputPin* ip = GetInputPin(i);
		OutputPin* op = ip->GetFrom();
		if (op == nullptr)
		{
			return false;
		}
	}

	return true;
}

void Circuit::SetPos(float x, float y)
{
	ImNode::SetNodePosition(GetNodeId(), ImVec2(x, y));
}

ImVec2 Circuit::GetPos()
{
	return ImNode::GetNodePosition(GetId());
}

void Circuit::Isolate()
{
	for (int i = 0; i < GetInputPinCount(); i++)
	{
		InputPin* in = GetInputPin(i);
		OutputPin* from = in->GetFrom();
		if (from != nullptr)
		{
			from->Disconnect(in);
		}
	}

	for (int i = 0; i < GetOutputPinCount(); i++)
	{
		OutputPin* out = GetOutputPin(i);
		for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++)
		{
			Wire& w = out->GetWire(j);
			if (w.GetTo() != nullptr)
			{
				out->Disconnect(w.GetTo());
			}
		}
	}
}

// static
void Circuit::UpdateAll(double _dt)
{
	// printf("UpdateAll \n");
	int dt = int(_dt * 1000000.0);

	while (dt != 0)
	{
		// 남은 딜레이의 최소값을 찾는다. 
		int minLeftDelay = INT_MAX;
		for (Circuit* pCircuit : s_pCircuits)
		{
			Circuit& c = *pCircuit;

			c.m_isJustResolved = false;

			bool c1 = c.IsResolvable();
			bool c2 = c.m_delay != 0;
			bool c3 = c.m_leftDelay != -1;
			bool c4 = c.m_leftDelay < minLeftDelay;
			bool condition = c1 && c2 && c3 && c4;
			if (condition)
			{
				minLeftDelay = c.m_leftDelay;
			}
			else {
				// printf("%s c1: %d c2: %d c3: %d c4: %d \n", c.GetName(), c1, c2, c3, c4);
			}
		}

		// printf("dt: %d \n", dt);
		// printf("minLeft: %d \n", minLeftDelay);

		if (minLeftDelay == 0.0f) return;
		if (minLeftDelay == INT_MAX) return;

		// leftDelay를 감소시키기만 하면 되는 경우
		if (dt < minLeftDelay)
		{
			for (Circuit* pCircuit : s_pCircuits)
			{
				bool condition =
					pCircuit->IsResolvable() &&
					pCircuit->m_delay != 0 &&
					pCircuit->m_leftDelay != -1;

				if (!condition)
					continue;
				
				pCircuit->m_leftDelay -= dt;
				
			}
			dt = 0;
		}
		else {
			for (Circuit* pCircuit : s_pCircuits)
			{
				Circuit& c = *pCircuit;

				bool condition =
					pCircuit->IsResolvable() &&
					pCircuit->m_delay != 0 &&
					pCircuit->m_leftDelay != -1;
				
				if (!condition)
					continue;

				// 출력을 업데이트해야 하는 경우
				if (minLeftDelay >= c.m_leftDelay)
				{
					// printf("UpdateAll updateOutput: %s \n", c.GetName());
					// 출력 업데이트 전 
					c.resolveConnected(minLeftDelay);
					c.m_leftDelay = -1;	// 중요! 반드시 여기에 있어야 함! 

					// 출력 업데이트 후 전파
					c.m_circuitOutput.Swap();
					c.updateOutput();
					c.afterUpdateOutput();
				}
				else {
					if (!c.m_isJustResolved)
					{
						c.m_leftDelay -= minLeftDelay;
					}
				}
			}

			dt -= minLeftDelay;
		}
	}
}

void Circuit::RenderAll()
{
	for (Circuit* pCircuit : s_pCircuits)
	{
		pCircuit->Render();
	}
}

void Circuit::RenderAllWires()
{
	for (Circuit* pCircuit : s_pCircuits)
	{
		pCircuit->RenderWire();
	}
}

Circuit* Circuit::GetCircuitById(ImNode::NodeId nodeId)
{
	auto it = std::find_if(
		s_pCircuits.begin(),
		s_pCircuits.end(),
		[nodeId](Circuit* pCircuit)
		{
			return pCircuit->GetNodeId() == nodeId;

		}
	);

	if (it == s_pCircuits.end()) {
		return nullptr;
	}

	return (*it);
}

void Circuit::setOutputData(int outputPinIndex, const bool* pData)
{
	OutputPin* op = GetOutputPin(outputPinIndex);
	assert(op != nullptr);

	m_circuitOutput.Write(
		pData,
		op->GetOutputOffset(),
		op->GetWireLineCount()
	);
}

void Circuit::setOutputDataByValue(int outputPinindex, uint32_t value)
{
	bool* outBuf = getOutputDataBuffer(outputPinindex);
	OutputPin* op = GetOutputPin(outputPinindex);
	Uint32ToBoolArray(value, outBuf, op->GetWireLineCount());
}

void Circuit::setOutputDataByValue(OutputPin& outputPin, uint32_t value)
{
	// 내 outputPin이 아니면 수정할 수 없다. 
	assert(&outputPin.GetOwner() == this);

	bool* outBuf = m_circuitOutput.GetCurrentBuffer();
	int offset= outputPin.GetOutputOffset();
	int count = outputPin.GetWireLineCount();
	Uint32ToBoolArray(value, &outBuf[offset], count);
}

bool* Circuit::getOutputDataBuffer(int outputPinIndex)
{
	bool* buffer = m_circuitOutput.GetCurrentBuffer();
	int offset = GetOutputPin(outputPinIndex)->GetOutputOffset();
	return &buffer[offset];
}

void Circuit::afterUpdateOutput()
{
	// printf("afterOutputChanged: %s \n", GetName());

	// outputPin을 순회하면서 해당하는 출력이 변했다면 연결된 circuit에게 통지한다. 
	for (int i = 0; i < m_outputPinCount; i++)
	{
		OutputPin* op = GetOutputPin(i);
		assert(op != nullptr);

		bool isChanged = m_circuitOutput.IsChanged(op->GetOutputOffset(), op->GetWireLineCount());
		if (isChanged || m_delay == 0)
		{
			for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++)
			{
				Wire& wire = op->GetWire(j);
				if (wire.GetTo() != nullptr)
				{
					Circuit& inputCircuit = wire.GetTo()->GetOwner();
					if (inputCircuit.IsResolvable())
					{
						inputCircuit.onInputChanged();
					}
				}
			}
		}
	}
}

void Circuit::resetDelay()
{
	m_leftDelay = m_delay;
}

void Circuit::renderDelay(float w)
{
	if (m_delay != 0)
	{
		if (m_leftDelay == -1)
		{
			ImGui::ProgressBar(0.0f, ImVec2(w, 0.0f));
			return;
		}

		float p = 0.0f;
		if (IsResolvable())
		{
			p = (float)(m_delay - m_leftDelay) / (float)m_delay;
		}
		ImGui::ProgressBar(p, ImVec2(w, 0.0f));
	}
}

void Circuit::renderIOGroup(const char* title, int index, int count)
{
	ImGui::Text(title);

	ImGui::BeginHorizontal("IO");
	ImGui::BeginVertical("Input");
	for (int i = index; i < index + count; i++)
	{
		InputPin* in = GetInputPin(i);
		in->Render();
	}
	ImGui::EndVertical();
	ImGui::BeginVertical("Output");
	for (int i = index; i < index + count; i++)
	{
		OutputPin* in = GetOutputPin(i);
		in->Render();
	}
	ImGui::EndVertical();
	ImGui::EndHorizontal();
}

void Circuit::updateOutput()
{}

void Circuit::onInputChanged()
{
	// printf("onInputChanged: %s \n", GetName());
	// Instance Circuit인 경우
	if (m_delay == 0)
	{
		// printf(" - update and alert \n");
		m_circuitOutput.Swap();
		// 출력이 업데이트되고 바로 전파된다. 
		updateOutput();
		afterUpdateOutput();
	}
	else {
		// delay를 초기화한다. 
		m_leftDelay = m_delay;
		// printf(" - delay reset: %d \n", m_leftDelay);
	}
}

void Circuit::resolveConnected(int dt_naosec)
{
	// 연결된 모든 Circuit을 순회한다. 
	for (int i = 0; i < m_outputPinCount; i++)
	{
		OutputPin* op = GetOutputPin(i);
		for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++) {
			Wire& wire = op->GetWire(j);
			if (wire.GetTo() != nullptr)
			{
				// 연결된 Circuit
				Circuit& connected = wire.GetTo()->GetOwner();
				
				// delay가 0인 circuit의 경우, 입력(caller의 출력)이 변하지 않았기 때문에 
				// 업데이트할 필요가 없다. 
				// if (connected.m_delay == 0)
				// {
				//	connected.swapCircuitOutput();
				// 	connected.updateOutput();
				// 	connected.afterUpdateOutput();
				// }

				if (connected.m_delay != 0) {
					bool condition =
						connected.IsResolvable() &&
						connected.m_leftDelay <= dt_naosec &&
						!connected.m_isJustResolved;

					if (condition)
					{
						connected.m_isJustResolved = true;
						connected.swapCircuitOutput();
						connected.updateOutput();
						connected.afterUpdateOutput();
					}
				}
				
			}
		}
	}
}
