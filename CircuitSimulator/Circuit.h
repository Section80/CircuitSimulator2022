#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <imgui_node_editor.h>
#include "CircuitOutput.h"
#include "Wire.h"

namespace ImNode = ax::NodeEditor;

class Pin;
class OutputPin;

class Circuit abstract : public Identifiable
{
public:
	friend class OutputPin;

public:
	Circuit(
		const char* name,
		int inputPinCount, int outputPinCount,
		bool* outputBuffer1, bool* outputBuffer2,
		int outputSize, 
		float delay
	);
	~Circuit();

	// [Pure Virtual] 
	// GUI(��ư ��) �Է¿� ���� ����� ��ȭ�� ���, �������� afterUpdateOutput()
	// �� �������� ȣ���ؾ� �Ѵ�. SwitchCircuit::Render() ����
	virtual void Render() = 0;
	void RenderWire();

	// [Pure Virtual] 
	// �������� �ʴ� index�� ��� assert�Ѵ�. 
	// @param index: 0���� �����Ѵ�. 
	virtual InputPin* GetInputPin(int index) = 0;
	// [Pure Virtual] 
	// �������� �ʴ� index�� ��� assert�Ѵ�. 
	// @param index: 0���� �����Ѵ�. 
	virtual OutputPin* GetOutputPin(int index) = 0;

	inline const char* GetName() { return m_name; }
	inline ImNode::NodeId GetNodeId() { return m_nodeId; }
	bool GetOutputData(int index);

	// �Է����� ��� ����Ǿ��ִ��� Ȯ���Ѵ�. 
	bool IsResolvable();

public:
	static void UpdateAll(double dt);
	static void RenderAll();
	static void RenderAllWires();

protected:
	inline void swapCircuitOutput() { m_circuitOutput.Swap(); }

	// �ش��ϴ� outputPin�� ����� ����. 
	// pData�� ���̴� �ش� OutputPin�� WireLineCount �̻��̿��� �Ѵ�. 
	void setOutputData(int outputPinindex, const bool* pData);

	void afterUpdateOutput();

private:
	static std::vector<Circuit*> s_pCircuits;

private:
	// [Pure Virtual]
	// ���� �Է¿� ����� outputPin�� ��� ������Ʈ�Ѵ�. 
	// InputPin::ReadAt(int wireLinIndex), Circuit::setOutputData(int outputPinindex, const bool* pData) �Լ��� ����� �����Ѵ�. 
	// ����: afterUpdateOutput()�� ȣ������ �� ��! �ش� �Լ��� �ƶ��� ���� �˾Ƽ� ȣ���. 
	virtual void updateOutput() = 0;

	void onInputChanged();

	void resolveConnected(float dt);

private:
	ImNode::NodeId m_nodeId;
	const char* m_name;
	CircuitOutput m_circuitOutput;
	
	int m_inputPinCount;
	int m_outputPinCount;

	int m_delay;
	int m_leftDelay;

	// Circuit::UpdateAll(float dt)���� ���Ǵ� ��
	bool m_isJustAwaked;
	bool m_isJustResolved;
};

#endif
