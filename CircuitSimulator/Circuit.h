#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <imgui_node_editor.h>
#include "CircuitType.h"
#include "CircuitOutput.h"
#include "Wire.h"

namespace ImNode = ax::NodeEditor;

class Pin;
class OutputPin;
class ClockBufferCircuit;

// �����ϱ�
// �����̰� 1���� and gate�� 1�� �Է��� true, 2�� �Է��� 0.1�ʸ��� true, false�� �ٲ�ٰ� �غ���. 
// ���ǿ��� �� ȸ���� ����� ��� �ɱ�? 
// undefined�ϱ�? �ƴϸ� 1�ʰ� ���� �� ���ʹ� 0.1�ʸ��� ����� ���ұ�? 
// ���� �������� �𵨿����� ���� delay���� ������ �Է��� ���� ���, ������ �Է��� ���õȴ�. ����������.  
// �׸��� ���ο� �Է¿� ���� ������Ʈ�� ������ delay��ŭ �ð��� �帥 �Ŀ��� ����ȴ�. 

class Circuit abstract : public Identifiable
{
public:
	friend class OutputPin;
	// friend void ::ClockBufferCircuit::afterUpdateOutput(); <= �̰ŷ� �ȵ�
	friend class ClockBufferCircuit;

public:
	Circuit(
		const char* name, ECircuitType circuitType,
		int inputPinCount, int outputPinCount,
		bool* outputBuffer1, bool* outputBuffer2,
		int outputSize, 
		float delay
	);
	~Circuit();

	// [Pure Virtual] 
	// GUI(��ư ��) �Է¿� ���� ����� ��ȭ�� ���, �������� afterUpdateOutput()
	// �� �������� ȣ���ؾ� �Ѵ�. SwitchCircuit::render() ����
	virtual void render() = 0;
	void renderWire();
	virtual void RenderInspector();

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
	inline int GetOutputPinCount() { return m_outputPinCount; }
	inline int GetInputPinCount() { return m_inputPinCount; }
	bool GetOutputData(int index);

	// �Է����� ��� ����Ǿ��ִ��� Ȯ���Ѵ�. 
	bool IsResolvable();

	void SetPos(float x, float y);
	ImVec2 GetPos();
	inline ECircuitType GetType() { return m_type; }

	void Isolate();

public:
	static void UpdateAll(double dt);
	static void RenderAll();
	static void RenderAllWires();

	static Circuit* GetCircuitById(ImNode::NodeId nodeId);

protected:
	inline void swapCircuitOutput() { m_circuitOutput.Swap(); }

	// �ش��ϴ� outputPin�� ����� ����. 
	// pData�� ���̴� �ش� OutputPin�� WireLineCount �̻��̿��� �Ѵ�. 
	void setOutputData(int outputPinindex, const bool* pData);
	
	void setOutputDataByValue(int outputPinindex, uint32_t value);

	// setOutputBuffer()�� pData�� OutputBufferData�� �����Ѵ�. 
	// ��, pData�� �̸� �Ҵ��صξ�� �Ѵ�. 
	// �׷��� ���� �ʰ�, outputBufferData�� ���� �����ϰ� ���� �� �� �Լ��� ����Ѵ�. 
	// [WARNING]�������� �����ؾ� �Ѵ�!! 
	// [WARNING]outputPin�� WireLineCount���� ���� ���� �ȵȴ�!!
	bool* getOutputDataBuffer(int outputPinIndex);

	// �� virtual�ϱ�? EdgeTriggeredCircuit ����
	virtual void onInputChanged();

	// �� outputPin�� ����� ���� ���, ����� circuit�� onInputChagned()�� ȣ���Ѵ�. 
	// �� virtual�ϱ�? ClockCircuit::afterUpdateOutput() ����
	virtual void afterUpdateOutput();

	// Edge-Triggered ȸ�� ������ ����Ѵ�. 
	void resetDelay();

	void renderDelay(float w = 100.0f);
	void renderIOGroup(const char* title, int index, int count);

private:
	static std::vector<Circuit*> s_pCircuits;

private:
	// [Pure Virtual]
	// ���� �Է¿� ����� outputPin�� ��� ������Ʈ�Ѵ�. 
	// InputPin::ReadAt(int wireLinIndex), Circuit::setOutputData(int outputPinindex, const bool* pData) �Լ��� ����� �����Ѵ�. 
	// ����: afterUpdateOutput()�� ȣ������ �� ��! �ش� �Լ��� �ƶ��� ���� �˾Ƽ� ȣ���. 
	// Default: �ƹ��͵� ����.
	virtual void updateOutput();

	// ��´ܿ� ����� m_delay�� 0�� �ƴ� circuit �� m_leftDelay�� dt���� ���� ���� ������
	// ����� ������Ʈ�ϰ� ���Ľ�Ų��. 
	void resolveConnected(int dt_naosec);

private:
	ImNode::NodeId m_nodeId;
	const char* m_name;
	ECircuitType m_type;
	CircuitOutput m_circuitOutput;

	int m_inputPinCount;
	int m_outputPinCount;

	int m_delay;
	int m_leftDelay;

	// Circuit::UpdateAll(float dt)���� ���Ǵ� ��
	bool m_isJustResolved;
};

#endif
