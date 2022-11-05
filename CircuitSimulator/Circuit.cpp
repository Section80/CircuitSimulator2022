#include "Pin.h"
#include "Circuit.h"

std::vector<Circuit*> Circuit::s_pCircuits;

Circuit::Circuit(
	const char* name,
	int inputPinCount, int outputPinCount,
	bool* outputBuffer1, bool* outputBuffer2,
	int outputSize,
	float delay
)
	: Identifiable()
	, m_name(name)
	, m_circuitOutput(outputBuffer1, outputBuffer2, outputSize)
	, m_inputPinCount(inputPinCount)
	, m_outputPinCount(outputPinCount)
	, m_delay(int(delay * 1000000))
	, m_leftDelay(-1.0f)
	, m_isJustAwaked(false)
	, m_isJustResolved(false)
{
	if (delay == 0.0f)
	{
		m_delay = 0;
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

// static
void Circuit::UpdateAll(double _dt)
{
	// printf("UpdateAll \n");
	int dt = _dt * 1000000;

	while (dt != 0)
	{
		// ���� �������� �ּҰ��� ã�´�. 
		int minLeftDelay = INT_MAX;
		for (Circuit* pCircuit : s_pCircuits)
		{
			Circuit& c = *pCircuit;

			c.m_isJustAwaked = false;
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

		// leftDelay�� ���ҽ�Ű�⸸ �ϸ� �Ǵ� ���
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

				// ����� ������Ʈ�ؾ� �ϴ� ���
				if (minLeftDelay >= c.m_leftDelay)
				{
					printf("UpdateAll updateOutput: %s \n", c.GetName());
					// ��� ������Ʈ �� 
					c.resolveConnected(minLeftDelay);
					c.m_leftDelay = -1;	// �߿�! �ݵ�� ���⿡ �־�� ��! 

					// ��� ������Ʈ �� ����
					c.m_circuitOutput.Swap();
					c.updateOutput();
					c.afterUpdateOutput();
					c.m_isJustAwaked = true;
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

void Circuit::afterUpdateOutput()
{
	printf("afterOutputChanged: %s \n", GetName());

	// outputPin�� ��ȸ�ϸ鼭 �ش��ϴ� ����� ���ߴٸ� ����� circuit���� �����Ѵ�. 
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
					inputCircuit.onInputChanged();
				}
			}
		}
	}
}

void Circuit::onInputChanged()
{
	printf("onInputChanged: %s \n", GetName());
	// Instance Circuit�� ���
	if (m_delay == 0)
	{
		printf(" - update and alert \n");
		m_circuitOutput.Swap();
		// ����� ������Ʈ�ǰ� �ٷ� ���ĵȴ�. 
		updateOutput();
		afterUpdateOutput();
	}
	else {
		// delay�� �ʱ�ȭ�Ѵ�. 
		m_leftDelay = m_delay;
		printf(" - delay reset: %d \n", m_leftDelay);
	}
}

void Circuit::resolveConnected(float _dt)
{
	int dt = int(_dt * 1000000);

	// ����� ��� Circuit�� ��ȸ�Ѵ�. 
	for (int i = 0; i < m_outputPinCount; i++)
	{
		OutputPin* op = GetOutputPin(i);
		for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++) {
			Wire& wire = op->GetWire(j);
			if (wire.GetTo() != nullptr)
			{
				// ����� Circuit
				Circuit& connected = wire.GetTo()->GetOwner();
				if (connected.m_delay == 0)
				{
					connected.swapCircuitOutput();
					connected.updateOutput();
					connected.afterUpdateOutput();
				}
				else {
					bool condition =
						connected.m_leftDelay <= dt &&
						!connected.m_isJustAwaked &&
						connected.m_isJustResolved;

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